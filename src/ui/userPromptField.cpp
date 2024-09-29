#include "userPromptField.hpp"
#include "logging.hpp"
#include "app.hpp"

void UserPromptField::handleTextChange(const QString& text)
{
    LOG_DEBUG("UserPromptField", "User Prompt: " + text.toStdString());
    ((App*)this->app)->userPrompt(text);
}

void UserPromptField::setApp(void* app)
{
    this->app = app;
}