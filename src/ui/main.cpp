#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include "logging.hpp"
#include "model_api.hpp"
#include "ui_backend.hpp"
#include "chatHistoryModel.hpp"

int main(int argc, char* argv[])
{

    KIconTheme::initTheme();
    QApplication app(argc, argv);
    LOG_DEBUG("UI", "Starting QApplication");
    KLocalizedString::setApplicationDomain("koolintelligence");
    //TODO: Add this back in when we have a proper organization name and domain
    // QApplication::setOrganizationName(QStringLiteral("KDE"));
    // QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationName(QStringLiteral("Kool Intelligence"));
    QApplication::setDesktopFileName(QStringLiteral("org.kde.koolintelligence"));
    LOG_DEBUG("UI", "Finished QAppplication intialization");

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
    LOG_INFO("UI", "Finished loading QApplication and QQuickStyle");
    QQmlApplicationEngine engine;
    
    Backend backend;
    qmlRegisterSingletonInstance("org.kde.koolintelligence", 1, 0, "Backend", &backend);
    
    ChatHistoryModel chatHistoryModel;
    qmlRegisterSingletonInstance("org.kde.koolintelligence", 1, 0, "ChatHistoryModel", &chatHistoryModel);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.kde.koolintelligence", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    LOG_INFO("UI", "Finished loading QML engine... Running app.exec()");

    chatHistoryModel.addChatMessage(QStringLiteral("This is a new chat"), QStringLiteral("User"), QStringLiteral("13:00"), true);
    int return_code = app.exec();

    LOG_INFO("UI", "Exiting QApplication");
    return return_code;
}