#include "app.hpp"
#include "file_search.hpp"
#include "logging.hpp"
#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KonsoleQML.h>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>

#include <pwd.h>
#include <string>
#include <unistd.h>

template<typename T>
inline void registerInstance(T *instance, const char *name)
{
    qmlRegisterSingletonInstance("org.kde.koolintelligence", 1, 0, name, instance);
    LOG_DEBUG("App", "Registered singleton instance: " + std::string(name));
}

App::App(int argc, char *argv[])
{
    this->setUserName();
    KonsoleQML::registerTypes("org.kde.konsoleqml");
    KIconTheme::initTheme();
    this->app = new QApplication(argc, argv);
    LOG_DEBUG("App", "Starting QApplication");
    KLocalizedString::setApplicationDomain("koolintelligence");
    // TODO: Add this back in when we have a proper organization name and domain
    //  QApplication::setOrganizationName(QStringLiteral("KDE"));
    //  QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationName(QStringLiteral("Kool Intelligence"));
    QApplication::setDesktopFileName(QStringLiteral("org.kde.koolintelligence"));
    LOG_DEBUG("App", "Finished QAppplication intialization");

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
    LOG_INFO("App", "Finished loading QApplication and QQuickStyle");
    this->engine = new QQmlApplicationEngine();
    this->chatHistoryModel = new ChatHistoryModel();
    this->model_api_instance = new ModelApi();
    this->userPromptField = new UserPromptField();
    this->userPromptField->setApp((void *)this);
    qmlRegisterSingletonType<TerminalTabModel>("org.kde.koolintelligence", 1, 0, "TerminalTabModel", [](QQmlEngine *, QJSEngine *) -> QObject * {
        const auto self = TerminalTabModel::self();
        QQmlEngine::setObjectOwnership(self, QQmlEngine::CppOwnership);
        return self;
    });
    LOG_DEBUG("App", "Registered TerminalTabModel");
    registerInstance(this->chatHistoryModel, "ChatHistoryModel");
    registerInstance(this->userPromptField, "UserPromptField");

    this->engine->rootContext()->setContextObject(new KLocalizedContext(this->engine));
    this->engine->loadFromModule("org.kde.koolintelligence", "Main");
}

int App::run()
{
    // LOG_INFO("app", this->model_api_instance->transcriptionService());
    // std::vector<std::string> results = fileSearchKDE("main.cpp");
    // for (const std::string &result : results) {
    //     LOG_INFO("App", "File Search Result: " + result);
    // }

    //this->model_api_instance->speak("hello, i am baymax, your personal healthcare companion");
    if (this->engine->rootObjects().isEmpty()) {
        return -1;
    }
    LOG_INFO("App", "Finished loading QML engine... Running app.exec()");
    int return_code = this->app->exec();
    LOG_INFO("App", "Exiting QApplication");
    return return_code;
}

static inline std::string getTimestamp()
{
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void App::userPrompt(const QString &text)
{
    LOG_DEBUG("App", "User Prompt: " + text.toStdString());
    this->chatHistoryModel->addChatMessage(text, QString::fromStdString(this->user_name), QString::fromStdString(getTimestamp()), true);
    std::future<std::string> futureMessage = std::async(std::launch::async, &ModelApi::getResponse, this->model_api_instance, text.toStdString());
    LOG_DEBUG("App", "Waiting for model response...");
    int chatIndex = this->chatHistoryModel->addChatMessage(QString().fromStdString("Waiting for response..."),
                                                           QString::fromStdString("Kool Intelligence"),
                                                           QString::fromStdString(getTimestamp()),
                                                           false);
    std::thread(&App::getModelResponse, this, std::move(futureMessage), chatIndex).detach();
}

void App::getModelResponse(std::future<std::string> futureMessage, int chatIndex)
{
    std::string message = futureMessage.get();
    LOG_DEBUG("App", "Model response: " + message);
    this->chatHistoryModel->removeChatMessage(chatIndex);
    this->chatHistoryModel->addChatMessage(QString().fromStdString(message),
                                           QString::fromStdString("Kool Intelligence"),
                                           QString::fromStdString(getTimestamp()),
                                           false);
}

void App::setUserName()
{
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        this->user_name = std::string(pw->pw_name);
        LOG_INFO("App", "Detected user name: " + this->user_name);
    } else {
        LOG_ERROR("App", "Could not detect user name");
    }
}

App::~App()
{
    delete this->chatHistoryModel;
    delete this->engine;
    delete this->app;
    delete this->model_api_instance;
    delete this->userPromptField;
}