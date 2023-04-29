#include "Engine/window.h"
#include "Game/app.h"

#include <random>

extern std::shared_ptr<App> app;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    std::cout<<"Hello World"<<std::endl;

    Window m_window = Window(app);
}