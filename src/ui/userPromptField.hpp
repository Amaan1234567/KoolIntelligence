#pragma once
#include <QObject>

class UserPromptField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enableChatField READ getEnableChatField WRITE setEnableChatField NOTIFY enableChatFieldChanged)

public:
    explicit UserPromptField(QObject *parent = nullptr): QObject(parent)
    {
    }

    bool getEnableChatField() const;

    void setEnableChatField(bool value);

    Q_SLOT void handleTextChange(const QString &text);
    Q_SLOT void resetChat();
    void setApp(void *app);

Q_SIGNAL void enableChatFieldChanged();

private:
    void *app;
    bool enableChatField = true;
};