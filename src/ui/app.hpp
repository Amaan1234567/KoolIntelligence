#pragma once
#include <QApplication>
#include <QQmlApplicationEngine>
#include "model_api.hpp"
#include "chatHistoryModel.hpp"
#include "userPromptField.hpp"
class App
{
public:
    App(int argc, char* argv[]);
    int run();
    void userPrompt(const QString& text);
    ~App();

private:
    void setUserName();
    QApplication* app;
    QQmlApplicationEngine* engine;
    ChatHistoryModel* chatHistoryModel;
    model_api* model_api_instance;
    UserPromptField* userPromptField;
    std::string user_name;
};