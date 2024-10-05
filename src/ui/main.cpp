#include "app.hpp"
#include "KwinScreenshot.hpp"
int main(int argc, char *argv[])
{   
    takeScreenshotKwin("/home/rahulvadhyar/KoolIntelligence/screenshot.png");
    App app(argc, argv);
    return app.run();
}