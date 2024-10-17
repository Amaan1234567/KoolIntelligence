#pragma once
#include "chatHistoryModel.hpp"
#include "model_api.hpp"
#include "terminaltabmodel.hpp"
#include "userPromptField.hpp"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <future>
class App
{
public:
    App(int argc, char *argv[]);
    int run();
    void userPrompt(const QString &text);
    void resetChat();
    ~App();

private:
    void setUserName();
    void getModelResponse(std::future<std::string> futureMessage, int chatIndex);
    QApplication *app;
    QQmlApplicationEngine *engine;
    ChatHistoryModel *chatHistoryModel;
    ModelApi *model_api_instance;
    UserPromptField *userPromptField;
    std::string user_name;
};