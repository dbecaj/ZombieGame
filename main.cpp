#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFontDatabase>

#include "core/window.h"
#include "main_menu/mainmenu.h"

Window* w = nullptr;
bool debug = false;
int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/fonts/Pixeled.ttf");

    int screenWidth = 1280;
    int screenHeight = 720;

    w = new Window(screenWidth, screenHeight, "ZombieGame");
    w->init();
    w->show();

    return a.exec();
}
