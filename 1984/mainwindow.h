#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QImageWriter>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QDir>
#include "videocapture.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void on_actionExit_triggered();

    void on_actionCrop_Tool_triggered();

    void on_actionCapture_triggered();

    void on_saveButton_clicked();

    void on_cropButton_clicked();

    void on_cancelButton_2_clicked();

    void on_cancelButton_clicked();

    void updateDisplayConfiguration();
    void on_actionHide_notTriggered(bool hide);
private:
    Ui::MainWindow *ui;
    QPixmap capture;
    QPixmap crop;
    void shootScreen();
    void savePixmap();
    void createSystemTrayIcon();
    QSystemTrayIcon * trayIcon;
};

#endif // MAINWINDOW_H
