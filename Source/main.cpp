#include "UI/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
