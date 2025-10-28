#include <QApplication>
#include "MetroPlannerWindow.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("MetroRouteOptimizerOrg");
    QCoreApplication::setApplicationName("MetroRouteOptimizer");
    MetroPlannerWindow window;
    window.show();
    return app.exec();
}
