#pragma once

#include <QObject>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString name() const { return m_name; }
    Q_SIGNAL void nameChanged();
    void setName(const QString &name) { m_name = name; Q_EMIT nameChanged();}

private:
    QString m_name = QStringLiteral("I can change this");
};