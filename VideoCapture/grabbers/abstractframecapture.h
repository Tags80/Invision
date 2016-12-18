#ifndef ABSTRACTFRAMECAPTURE_H
#define ABSTRACTFRAMECAPTURE_H

#include <QObject>
#include <QRect>
#include <QPixmap>
#include <QImage>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter> //may move to openglpainter for hw accel
#include <QCursor>
#include <QPoint>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <QtMath>
#include <QDebug>


#ifdef Q_OS_LINUX
#include <QtX11Extras/QX11Info>
#include <QtX11Extras/qx11info_x11.h>
#endif




class AbstractFrameCapture : public QObject
{
    Q_OBJECT
public:
    AbstractFrameCapture(QRect * recordingRegion, double fps = 25);
    ~AbstractFrameCapture();
    double getFPS();
    virtual QList<double> getAllFPS() = 0;
    virtual double getPreferredFPS() = 0;
    void setFPS(double);
    QRect *getRegion();
    void setRegion(QRect *);
    void setDrawCursor(bool);
    void setPreviewCount(int);
    int getFrameCount();
    virtual bool startRecording() = 0;
    virtual bool stopRecording() = 0;

    virtual QString stringID() = 0;
    static AbstractFrameCapture* fromStringID();


signals:
    ///
    /// \brief QImagePrepped Pass a BGRA8888 QImage to the Encoder, the time of the capture.
    /// This will allow a simple subtraction to determine the time in the video e.g. time (in ms) = timestamp - starttime. a zero will indicate a constant frame rate
    /// \param timestamp time since epoch in ms
    ///
    void QImagePrepped(QImage &, qint64 timestamp);
protected:
    QPixmap capture;
    QTimer *timer;
    QList<double> allFPS;
    QRect *recordingRegion;
    bool drawCursor;
    bool isRecording;
    double fps;
    int previewCount;
    int framesCaptured;
    //CaptureMode mode;

protected slots:
    virtual void captureFrame() = 0;



};

#endif // ABSTRACTFRAMECAPTURE_H
