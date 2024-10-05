/**
Taken from https://invent.kde.org/plasma/xdg-desktop-portal-kde/-/blob/30b9ac6a169ee9f7c36aabbec4038a8a715ac2cb/src/screenshotdialog.cpp
and modified to work with our codebase

Also with help from Vlad Zahorodnii(vlad.zahorodnii@kde.org), Aleix Pol(aleixpol@kde.org)

Note: This code is licensed under LGPL-2.0-or-later as per the original codebase
*/

#include <QtDBus/QtDBus>
#include <QDBusUnixFileDescriptor>
#include <QImage>
#include <QtConcurrentRun>
#include <qplatformdefs.h>
#include "logging.hpp"
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

static const QString screenShotService = QString().fromStdString("org.kde.KWin.ScreenShot2");
static const QString screenShotObjectPath = QString().fromStdString("/org/kde/KWin/ScreenShot2");
static const QString screenShotInterface = QString().fromStdString("org.kde.KWin.ScreenShot2");

static int readData(int fd, QByteArray &data)
{
    char buffer[4096];
    pollfd pfds[1];
    pfds[0].fd = fd;
    pfds[0].events = POLLIN;

    while (true) {
        // give user 30 sec to click a window, afterwards considered as error
        const int ready = poll(pfds, 1, 30000);
        if (ready < 0) {
            if (errno != EINTR) {
                LOG_ERROR("takeScreenshotKwin", "Failed to read screenshot");
                return -1;
            }
        } else if (ready == 0) {
            LOG_ERROR("takeScreenshotKwin", "Failed to read screenshot: timeout");
            return -1;
        } else if (pfds[0].revents & POLLIN) {
            const int n = QT_READ(fd, buffer, sizeof(buffer));

            if (n < 0) {
                LOG_ERROR("takeScreenshotKwin", "Failed to read screenshot");
                return -1;
            } else if (n == 0) {
                return 0;
            } else if (n > 0) {
                data.append(buffer, n);
            }
        } else if (pfds[0].revents & POLLHUP) {
            return 0;
        } else {
            LOG_ERROR("takeScreenshotKwin", "Failed to read screenshot");
            return -1;
        }
    }
}

static QImage allocateImage(const QVariantMap &metadata)
{
    bool ok;

    const uint width = metadata.value(QStringLiteral("width")).toUInt(&ok);
    if (!ok) {
        return QImage();
    }

    const uint height = metadata.value(QStringLiteral("height")).toUInt(&ok);
    if (!ok) {
        return QImage();
    }

    const uint format = metadata.value(QStringLiteral("format")).toUInt(&ok);
    if (!ok) {
        return QImage();
    }

    return QImage(width, height, QImage::Format(format));
}

static QImage readImage(int pipeFd, const QVariantMap &metadata)
{
    QByteArray content;
    if (readData(pipeFd, content) != 0) {
        close(pipeFd);
        return QImage();
    }
    close(pipeFd);

    QImage result = allocateImage(metadata);
    LOG_DEBUG("takeScreenshotKwin", "Image allocated");
    if (result.isNull()) {
        LOG_ERROR("takeScreenshotKwin", "Failed to allocate image");
        return QImage();
    }

    QDataStream ds(content);
    ds.readRawData(reinterpret_cast<char *>(result.bits()), result.sizeInBytes());
    LOG_DEBUG("takeScreenshotKwin", "Image read");
    return result;
}

QFuture<QImage> kwinScreenshotConnect(){
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
        close(pipeFds[0]);
        return QFuture<QImage>();
    }
    LOG_DEBUG("takeScreenshotKwin", "Screenshot taken");
    return QtConcurrent::run(readImage, pipeFds[0], reply);
}

void takeScreenshotKwin(std::string path) {
    QFuture<QImage> future = kwinScreenshotConnect();
    if (!future.isStarted()) {
        return;
    }
    LOG_DEBUG("takeScreenshotKwin", "Waiting for screenshot");
    future.waitForFinished();
    QImage m_image = future.result();
    LOG_DEBUG("takeScreenshotKwin", "Screenshot received");
    if (m_image.isNull()) {
        LOG_ERROR("takeScreenshotKwin", "Failed to take screenshot");
        return;
    }
    m_image.save(QString::fromStdString(path));
    LOG_INFO("takeScreenshotKwin", "Screenshot saved");
}