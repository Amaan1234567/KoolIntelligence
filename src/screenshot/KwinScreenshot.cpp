#include <QtDBus/QtDBus>
#include <QDBusUnixFileDescriptor>
#include "logging.hpp"
#include <fcntl.h>
static const QString screenShotService = QString().fromStdString("org.kde.KWin.ScreenShot2");
static const QString screenShotObjectPath = QString().fromStdString("/org/kde/KWin/ScreenShot2");
static const QString screenShotInterface = QString().fromStdString("org.kde.KWin.ScreenShot2");
void takeScreenshotKwin(std::string path) {
    
    int pipeFds[2];
    if (pipe2(pipeFds, O_CLOEXEC | O_NONBLOCK) != 0) {
        LOG_ERROR("takeScreenshotKwin", "Failed to create pipe");
    }

    QVariantMap options;
    options.insert(QStringLiteral("native-resolution"), true);

    QString method = QStringLiteral("CaptureWorkspace");

    QVariantList arguments;
    arguments.append(options);
    arguments.append(QVariant::fromValue(QDBusUnixFileDescriptor(pipeFds[1])));

    QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.kde.KWin.ScreenShot2"),
                                                          QStringLiteral("/org/kde/KWin/ScreenShot2"),
                                                          QStringLiteral("org.kde.KWin.ScreenShot2"),
                                                          method);
    message.setArguments(arguments);

    QDBusReply<QVariantMap> reply = QDBusConnection::sessionBus().call(message);
    ::close(pipeFds[1]);
    if (!reply.isValid()) {
        LOG_ERROR("takeScreenshotKwin", "Failed to call method: " + reply.error().message().toStdString());
        return;
    }
    LOG_INFO("takeScreenshotKwin", "Screenshot taken");


}