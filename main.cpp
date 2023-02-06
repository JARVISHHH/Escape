#include "Engine/window.h"
#include "Game/app.h"

extern std::shared_ptr<App> app;

int main(int argc, char *argv[])
{
    std::cout<<"Hello World"<<std::endl;

    Window m_window = Window(app);
}