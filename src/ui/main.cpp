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

int main(int argc, char* argv[])
{

    model_api api;

    // Set additional options if needed
    api.set_option("seed", 42); // Example of setting an option

    // Define the prompt and image path
    std::string prompt = "describe this image";
    std::string image_path = "/home/amaan/Pictures/backgrounds/raiden epic background.png";

    // Get the synchronous response from the model and print it
    std::string response = api.generation_with_image(prompt, image_path);
    std::cout << "Response: " << response << std::endl;

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

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.kde.koolintelligence", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    LOG_INFO("UI", "Finished loading QML engine... Running app.exec()");

    int return_code = app.exec();

    LOG_INFO("UI", "Exiting QApplication");
    return return_code;
}