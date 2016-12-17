#include "standardscreencapture.h"


///
/// \brief StandardScreenCapture::StandardScreenCapture
/// \param recordingRegion the region of the screen to be captured
/// \param drawCursor draw the cursor on the screen shot captured, turing this parameter off can in crease speed
/// \param fps frames per second
/// \param monitorNum the screen which you want to record. -1 for virtual display
///
StandardScreenCapture::StandardScreenCapture(QRect * recordingRegion, bool drawCursor, double fps, int monitorNum = 0) :
    AbstractFrameCapture(recordingRegion,fps)
{
    this->drawCursor = drawCursor;
    this->monitorNum = monitorNum;
    timer = new QTimer(this);
    timer->setInterval(qFloor(fps/1000.0));
    timer->setTimerType(Qt::PreciseTimer);
    fps = getPreferredFPS();
    allFPS.push_back(10.0);
    allFPS.push_back(15.0);
    allFPS.push_back(20.0);
    allFPS.push_back(25.0);
    allFPS.push_back(29.97);
    allFPS.push_back(30.0);
    allFPS.push_back(48.0);
    allFPS.push_back(59.94);
    allFPS.push_back(60.0);

    connect(timer, &QTimer::timeout,this, &StandardScreenCapture::captureFrame);
}

///
/// \brief StandardScreenCapture::getAllFPS
/// \return a list of all common fps for screen captures
///
QList<double> StandardScreenCapture::getAllFPS(){

    return allFPS;
}

double StandardScreenCapture::getPreferredFPS()
{
    return 30.0;
}


bool StandardScreenCapture::startRecording(){
    if(isRecording){
        return true;
    }

    isRecording = true;
    timer->start(qFloor(fps/1000.0));
    return timer->isActive();

}



///
/// \brief StandardFrameCapture::captureDesktop
/// This is the capture desktop function implemented in purely Qt c++
/// This is not the fasted method by any means but it will always work.
///
void StandardScreenCapture::captureFrame()
{

    //TODO implement the recording region


    //Don't let the small amount of code fool you. This is not that fast.

    //This method is suprisingly slow. There are many platform specific ways of doing this that will go much faster or use HW acceleration.
    //Thats why this is a virtual method in a child class. Inheritance is lovely.
    this->capture = QPixmap();
    capture = screens.at(this->monitorNum)->grabWindow(0);



    QImage img = capture.toImage();



// TODO
//    if(this->drawCursor){
//        paintCursor(&img);
//    }




    emit QImagePrepped(img, 0);
}

