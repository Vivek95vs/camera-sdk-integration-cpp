#include "httpwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HttpWindow w;
    w.show();

    return a.exec();
}
