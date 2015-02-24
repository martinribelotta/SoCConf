#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc == 1) {
        MainWindow *w = new MainWindow();
        w->show();
    } else {
        for(int i=1; i<argc; i++) {
            MainWindow *w = new MainWindow();
            if (!w->newFrom(argv[i]))
                delete w;
            else
                w->show();
        }
    }
    return a.exec();
}
