#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    appSettings=new QSettings(QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),QApplication::applicationName(),this);
    data=new ExperimentData(this);
    plot=new Plot(this,data);


    ui->plotLayout->addWidget(plot);

    connect(ui->actionFullscreen,SIGNAL(triggered(bool)),this,SLOT(setFullscreen(bool)));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionConnect_to,SIGNAL(triggered()),this,SLOT(connectTo()));
    connect(ui->actionDisconnect,SIGNAL(triggered()),this,SLOT(socketDisconnect()));
    connect(ui->actionViewData,SIGNAL(triggered()),this,SLOT(showDataViewWindow()));
    connect(ui->actionReplot,SIGNAL(triggered()),plot,SLOT(replot()));
    connect(ui->actionClear_plot,SIGNAL(triggered()),plot,SLOT(clear()));
    connect(ui->actionInitialize,SIGNAL(triggered()),plot,SLOT(initialize()));
    connect(ui->actionSelect_points,SIGNAL(triggered(bool)),plot,SLOT(selectPointsMode(bool)));
    connect(plot,SIGNAL(message(QString)),statusBar(),SLOT(showMessage(QString)));



    connect(&tcpClient,SIGNAL(connected()),this,SLOT(socketConnectedToServer()));
    connect(&tcpClient,SIGNAL(disconnected()),this,SLOT(socketDisconnectedFromServer()));
    connect(&tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(&tcpClient,SIGNAL(dataLine(QByteArray&)),data,SLOT(parseLine(QByteArray&)));
    connect(&tcpClient,SIGNAL(initialData()),plot,SLOT(clear()));
    connect(&tcpClient,SIGNAL(initialData()),data,SLOT(resetData()));

//FIXME: al least bytesRead indicates not the total number of bytes that was read from network. signalled value should be added to current number of bytes read
    connect(&tcpClient,SIGNAL(bytesWritten(int)),&bytesWrittenLabel,SLOT(setNum(int)));
    connect(&tcpClient,SIGNAL(bytesRead(int)),&bytesReadLabel,SLOT(setNum(int)));

    connect(data,SIGNAL(initialized()),plot,SLOT(initialize()));
    connect(data,SIGNAL(pointCount(int)),&pointCountLabel,SLOT(setNum(int)));
    connect(data,SIGNAL(pointCount(int)),plot,SLOT(drawLastPoint(int)));



    connectionLabel.setText("*");
    connectionLabel.setToolTip(tr("Connection status:\nGreen - connected\nRed - disconnected"));
    connectionLabel.setStyleSheet("QLabel {color:red; font-weight:bold;}");
    pointCountLabel.setText("0");
    pointCountLabel.setToolTip(tr("Number of points"));
    bytesWrittenLabel.setText("0");
    bytesWrittenLabel.setToolTip("Bytes written to network");
    bytesReadLabel.setText("0");
    bytesReadLabel.setToolTip("Bytes read from network");

    ui->statusBar->addPermanentWidget(&bytesReadLabel);
    ui->statusBar->addPermanentWidget(&bytesWrittenLabel);
    ui->statusBar->addPermanentWidget(&pointCountLabel);
    ui->statusBar->addPermanentWidget(&connectionLabel);

}

MainWindow::~MainWindow()
{
    delete plot;
    delete data;
    delete appSettings;
    delete ui;

}

void MainWindow::setFullscreen(bool a) {
    if (a)
     setWindowState(Qt::WindowFullScreen);
    else
     setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (event->spontaneous()) event->accept();
    QMessageBox dialog(this);
    dialog.setText("Really exit?");
    dialog.setIcon(QMessageBox::Question);
    dialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    dialog.setDefaultButton(QMessageBox::Cancel);
    dialog.setFocus();

    if (dialog.exec()==QMessageBox::Ok)
    {

    //FIXME: Here should be some actions to do befor closing app
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::connectTo() {
    bool ok;
    QString hostname=QInputDialog::getText(this,"Specify host name or ip","Host to connect to",QLineEdit::Normal,appSettings->value("Connect to").toString(),&ok,0);
    if (ok) {
        appSettings->setValue("Connect to",QVariant(hostname));
        qDebug()<<"Connecting to"<<hostname<<"port"<<25050;
        this->tcpClient.connectToHost(hostname,25050);
    }

}
void MainWindow::socketConnectedToServer() {
    qDebug()<<"Connected!";
    ui->actionDisconnect->setDisabled(false);
    connectionLabel.setStyleSheet("QLabel {color:green;font-weight:bold;}");
}

void MainWindow::socketDisconnectedFromServer() {
    qWarning()<<"Disconnected from server!";
    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText("Disconnected from server");
    msgBox.setIcon(QMessageBox::Warning);
    //msgBox.exec();
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState state) {
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        qWarning()<<"Unconnected";
        break;
    case QAbstractSocket::HostLookupState:
        qDebug()<<"Lookup for host";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug()<<"Connecting";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug()<<"Connected!";
        break;
    case QAbstractSocket::ClosingState:
        qWarning()<<"Closing socket!";
        break;
    case QAbstractSocket::BoundState:
        qDebug()<<"Socket is bound";
        break;
    case QAbstractSocket::ListeningState:
        qDebug()<<"Socket is listening";
        break;
    }
}

void MainWindow::socketDisconnect(void) {
    tcpClient.disconnectFromHost();
    ui->actionDisconnect->setDisabled(true);
    connectionLabel.setStyleSheet("QLabel {color:red;font-weight:bold;}");
}

void MainWindow::showDataViewWindow() {
    DataViewWindow *dataView=new DataViewWindow(this);
    dataView->setAscii(data->getAscii());
    dataView->setItem(data);
    dataView->show();
}

