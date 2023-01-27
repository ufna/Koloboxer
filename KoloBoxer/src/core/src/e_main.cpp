#include "mainWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(GameResources);

    QApplication app(argc, argv);
	
	app.quitOnLastWindowClosed();

    mainWidget mainWidget(0);
    mainWidget.show();

    return app.exec();
}
