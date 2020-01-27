#include "mainwindow.h"
#include <QApplication>

#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":qdarkstyle/style.qss");

    if (!styleFile.exists())
    {
        qWarning("Unable to set stylesheet, file not found\n");
    }
    else
    {
        styleFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&styleFile);
        app.setStyleSheet(ts.readAll());
    }


    MainWindow w;
    w.show();

    return app.exec();
}
