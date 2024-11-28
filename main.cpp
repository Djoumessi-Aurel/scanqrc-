#include "mainwindow.h"
#include <QApplication>
#include <QCameraInfo>

// Pour reconnaître QCameraInfo comme un type métaobjet
Q_DECLARE_METATYPE(QCameraInfo)

int main(int argc, char *argv[])
{
    // Pour reconnaître QCameraInfo comme un type métaobjet
    qRegisterMetaType<QCameraInfo>();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
