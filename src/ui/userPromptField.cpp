#include "userPromptField.hpp"
#include "app.hpp"
#include "logging.hpp"

void UserPromptField::handleTextChange(const QString &text)
{
    LOG_DEBUG("UserPromptField", "User Prompt: " + text.toStdString());
    ((App *)this->app)->userPrompt(text);
}

void UserPromptField::setApp(void *app)
{
    this->app = app;
}

void UserPromptField::resetChat()
{
    ((App *)this->app)->resetChat();
}

bool UserPromptField::getEnableChatField() const
{
    return enableChatField;
}

void UserPromptField::setEnableChatField(bool value)
{
    if (enableChatField != value) {
        enableChatField = value;
        Q_EMIT enableChatFieldChanged();
    }
}