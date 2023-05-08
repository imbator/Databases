#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextStream out(stdout, QIODevice::WriteOnly);
    out.setEncoding(QStringConverter::Utf8);

    auto style = QStyleFactory::create("Fusion");
    QApplication::setStyle(style);

    MainWindow w;
    w.show();
    return a.exec();
}
