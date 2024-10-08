#pragma once
#include "chatHistoryModel.hpp"
#include "model_api.hpp"
#include "userPromptField.hpp"
#include <QApplication>
#include <QQmlApplicationEngine>
class App{
public:
    App(int argc, char *argv[]);
    int run();
    void userPrompt(const QString &text);
    ~App();

private:
    void setUserName();
    QApplication *app;
    QQmlApplicationEngine *engine;
    ChatHistoryModel *chatHistoryModel;
    ModelApi *model_api_instance;
    UserPromptField *userPromptField;
    std::string user_name;
};