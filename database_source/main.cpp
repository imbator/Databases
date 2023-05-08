#include "interface/mainwindow.h"
#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto style = QStyleFactory::create("Fusion");
    QApplication::setStyle(style);

    MainWindow w;
    w.show();
    return a.exec();
}
