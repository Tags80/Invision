#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videocapture.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //setup the combobox with the image avaliable image types
       for(int i = 0; i < QImageWriter::supportedImageFormats().size(); i++)
       {
           ui->comboBox->insertItem(i,QImageWriter::supportedImageFormats().at(i));
       // nees to have the selections slimmed down a bit.
       }


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}



void MainWindow::on_actionCrop_Tool_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->labelCrop->setPixmap(capture);

}

void MainWindow::on_actionCapture_triggered()
{
    shootScreen();
}

void MainWindow::shootScreen(){
    ui->stackedWidget->setCurrentIndex(0);
    capture = StandardScreenCapture::getSingleFrame(0);

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
