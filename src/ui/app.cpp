#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>
#include <pwd.h>
#include <string>
#include <unistd.h>

#include "app.hpp"
#include "logging.hpp"

template<typename T>
inline void registerInstance(T *instance, const char *name)
{
    qmlRegisterSingletonInstance("org.kde.koolintelligence", 1, 0, name, instance);
    LOG_DEBUG("App", "Registered singleton instance: " + std::string(name));
}

App::App(int argc, char *argv[])
{
    this->setUserName();
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
    this->model_api_instance = new model_api();
    this->userPromptField = new UserPromptField();
    this->userPromptField->setApp((void *)this);
    this->terminalPanel = new TerminalPanel();
    registerInstance(this->chatHistoryModel, "ChatHistoryModel");
    registerInstance(this->userPromptField, "UserPromptField");
    registerInstance(this->terminalPanel, "TerminalPanel");
    this->engine->rootContext()->setContextObject(new KLocalizedContext(this->engine));
    this->engine->loadFromModule("org.kde.koolintelligence", "Main");
}

int App::run()
{
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
    std::string message = this->model_api_instance->get_response(text.toStdString());
    LOG_DEBUG("App", "Model Response: " + message);
    this->chatHistoryModel->addChatMessage(QString::fromStdString(message),
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
    delete this->terminalPanel;
}