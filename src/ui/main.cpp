#include "app.hpp"
#include "KwinScreenshot.hpp"
#include "terminal.hpp"
int main(int argc, char *argv[])
{   
    App app(argc, argv);
    return app.run();
}