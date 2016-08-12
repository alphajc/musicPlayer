#include "playerwidget.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/image/background/background4.jpg");
    QSplashScreen splash(pixmap);
    splash.show();

    PlayerWidget w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
