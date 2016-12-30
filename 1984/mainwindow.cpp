#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //always start on screenshot page
    ui->stackedWidget->setCurrentIndex(0);

    updateDisplayConfiguration();
    createSystemTrayIcon();


    //setup the combobox with the image avaliable image types
    for(int i = 0; i < QImageWriter::supportedImageFormats().size(); i++)
    {
        ui->comboBox->insertItem(i,QImageWriter::supportedImageFormats().at(i));
        // needs to have the selections slimmed down a bit.
    }






//    QDesktopWidget *widget = QApplication::desktop();
//    //connect signals and slots that need to be monitored
//    connect(widget,SIGNAL(screenCountChanged(int)), this, SLOT(updateDisplayConfiguration())); //this should be tested...but I don't feel like it right now. //Shit works. Just wanted to leave this comment.

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    if(ui->stackedWidget->currentIndex() == 0){
        QPixmap temp = capture.scaled(ui->labelScreenShot->size());
        ui->labelScreenShot->setPixmap(temp);
    }


}

void MainWindow::createSystemTrayIcon()
{
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        qDebug() << "SystemTrayIcon Avaliable";
        trayIcon = new QSystemTrayIcon ();
        trayIcon->setIcon(this->windowIcon());


        //construct trayicon menu
        QMenu *trayMenu = new QMenu();

        //construct enabled menu
        QAction *enabledAction = new QAction(this);
        enabledAction->setText("Enabled");
        enabledAction->setCheckable(true);
        enabledAction->setChecked(true);
        enabledAction->setEnabled(false);
        trayMenu->addAction(enabledAction);

        //show and hide
        QAction *hideAction = new QAction(this);
        hideAction->setText("Show in Taskbar");
        hideAction->setCheckable(true);
        hideAction->setChecked(true);
        connect(hideAction,SIGNAL(toggled(bool)),this,SLOT(on_actionHide_notTriggered(bool)));
        trayMenu->addAction(hideAction);


        //construct terminate
        QAction *exitAction = new QAction(this);
        exitAction->setText("Terminate");
        exitAction->setEnabled(true);
        connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
        trayMenu->addAction(exitAction);


        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();

    }
    else
        qDebug() << "SystemTrayIcon NOT Avaliable";
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}



void MainWindow::on_actionCrop_Tool_triggered()
{
    if(capture.isNull()) return;

    ui->stackedWidget->setCurrentIndex(1);
    ui->labelCrop->setPixmap(capture);

}

void MainWindow::on_actionCapture_triggered()
{
    shootScreen();
}

void MainWindow::shootScreen(){
    ui->stackedWidget->setCurrentIndex(0);
    QDesktopWidget *widget = QApplication::desktop();
    if(!widget->isVirtualDesktop()){
        capture = QGuiApplication::primaryScreen()->grabWindow(0);
    }
    else{
        QList<QScreen*> screens = QGuiApplication::primaryScreen()->virtualSiblings();
        QList<QPixmap> scrs;
        int w = 0, h = 0, p = 0;

        foreach (auto scr, screens)
        {
            QRect g = scr->geometry();
            qDebug() << scr->name() << endl;
            QPixmap pix = scr->grabWindow(0, g.x(), g.y(), g.width(), g.height());
            w += pix.width();
            h = qMax(h, pix.height());
            scrs.append(pix);
        }

        QPixmap final(w, h);
        QPainter painter(&final);
        final.fill(Qt::black);
        foreach (auto scr, scrs)
        {
            painter.drawPixmap(QPoint(p, 0), scr);
            p += scr.width();
        }
        capture = final;
    }
    QPixmap temp = capture.scaled(ui->labelScreenShot->size());
    ui->labelScreenShot->setPixmap(temp);

    QString defaultFileName = "Screenshot From " + QDateTime::currentDateTime().toString();
    defaultFileName.replace(":","-");
    ui->lineEdit->setText(defaultFileName);


}

void MainWindow::savePixmap()
{
    if(capture.isNull())
    {
        return;
    }
    QString pictureDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString absoluteFilePath = pictureDir  + QDir::separator() + ui->lineEdit->text() + "." + ui->comboBox->currentText();
    bool exists = QFile::exists(absoluteFilePath);
    if(exists){
        QMessageBox box;
        box.setText("File exists! Overwrite?");

        int result = box.exec();
        if(result == 1024){
            capture.save(absoluteFilePath);
        }
    }
    else
        this->capture.save(absoluteFilePath);//check for overwrite.

    ui->labelScreenShot->clear();
    ui->lineEdit->clear();

}

void MainWindow::on_saveButton_clicked()
{
    savePixmap();
}

void MainWindow::on_cropButton_clicked()
{
    capture = ui->labelCrop->crop();
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap temp = capture.scaled(ui->labelScreenShot->size());
    ui->labelScreenShot->setPixmap(temp);
}

void MainWindow::on_cancelButton_2_clicked()
{
    ui->labelCrop->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_cancelButton_clicked()
{
    ui->lineEdit->clear();
    ui->labelScreenShot->clear();
    capture = QPixmap();
}

void MainWindow::updateDisplayConfiguration()
{

}

void MainWindow::on_actionHide_notTriggered(bool hide)
{
    this->setHidden(!hide);
}


