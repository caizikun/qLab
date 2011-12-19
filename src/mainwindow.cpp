#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qgpibdevice.h"

#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



//    QSystemTrayIcon *systemTrayIcon=new QSystemTrayIcon(QIcon::QIcon(":/global/qt-logo.png"),this);
//
//    QMenu trayMenu;
//    QAction exitAct=trayMenu.addAction("Exit");
//
//    trayIcon->setContextMenu(&trayMenu);
//    trayIcon->show();

    QByteArray shortname="k2000";
    QGpibDevice *dev = new QGpibDevice(shortname,this);
    QByteArray arr;
    dev->readValue(0,arr);
    ui->label->setText(arr);
    delete dev;


}

MainWindow::~MainWindow()
{
    delete ui;
}
