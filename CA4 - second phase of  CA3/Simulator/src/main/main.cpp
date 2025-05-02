#include <QCoreApplication>
#include <QDebug>

#include "ApplicationContext/ApplicationContext.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    ApplicationContext applicationContext(":/assets/main/config.json");
    applicationContext.run();

    return app.exec();
}
