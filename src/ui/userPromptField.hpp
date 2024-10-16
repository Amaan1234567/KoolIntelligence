#pragma once
#include <QObject>

class UserPromptField : public QObject
{
    Q_OBJECT
public:
    explicit UserPromptField(QObject *parent = nullptr)
    {
    }
    Q_SLOT void handleTextChange(const QString &text);

    Q_SLOT void resetChat();
    void setApp(void *app);

private:
    void *app;
};