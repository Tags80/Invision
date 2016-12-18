#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QImageWriter>
#include <QStandardPaths>
#include <QDir>
#include "grabbers/standardscreencapture.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionCrop_Tool_triggered();

    void on_actionCapture_triggered();

    void on_saveButton_clicked();

    void on_cropButton_clicked();

    void on_cancelButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap capture;
    QPixmap crop;

    void shootScreen();
    void savePixmap();
};

#endif // MAINWINDOW_H
