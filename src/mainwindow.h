#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "devicefarm.h"
#include "experimentconfigurationselectdialog.h"
#include "confirmexitdialog.h"
#include "qexperiment.h"
#include <QSettings>
#include <QLabel>
#include <QLocale>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSettings *experimentSettings;
    QExperiment *experiment;
    QLabel experimentLabel;
    QStringList devices;
    QList <QStringList> parameters;
    QList <QAbstractDevice *> deviceList;
    void initDevices();

private slots:
    void setFullscreen(bool isFullscreen);
    void getExperiment();
    void showAboutQt();

public slots:
    void setExperiment(QString experiment);
};

#endif // MAINWINDOW_H
