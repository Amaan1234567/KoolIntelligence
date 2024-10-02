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