#ifndef STANDARDFRAMECAPTURE_H
#define STANDARDFRAMECAPTURE_H

#include "standardscreencapture.h"
#include "abstractframecapture.h"


class StandardScreenCapture : public AbstractFrameCapture
{
    Q_OBJECT
public:
    StandardScreenCapture(QRect * recordingRegion, bool drawCursor = true, double fps = 25, int monitorNum = 0);
    QList<double> getAllFPS();
    double getPreferredFPS();
    bool startRecording();
    bool stopRecording();
    QString stringID();

    static QPixmap getSingleFrame(int monitorNumber);

    bool isAvailable();
protected slots:
    virtual void captureFrame();


private:
    int monitorNum;
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QList<QScreen*> screens = primaryScreen->virtualSiblings();
};

#endif // STANDARDFRAMECAPTURE_H
