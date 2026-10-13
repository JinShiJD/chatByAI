#include "mainwindow.h"

#include <QApplication>

#include <QSslSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::sslLibraryVersionString();
    return a.exec();
}
