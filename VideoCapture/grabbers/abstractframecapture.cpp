#include "abstractframecapture.h"


AbstractFrameCapture::AbstractFrameCapture(QRect *recordingRegion, double fps)
{

    //TODO:validate recording region width height even and > 1

    this->recordingRegion = recordingRegion;
    this->fps = fps;
}

///
/// \brief AbstractFrameCapture::~AbstractFrameCapture
/// Deletes all the objects this class points to and then deletes the class
AbstractFrameCapture::~AbstractFrameCapture()
{

}

double AbstractFrameCapture::getFPS(){
    return this->fps;
}


void AbstractFrameCapture::setFPS(double FPS){
    this->fps = FPS;
}

QRect* AbstractFrameCapture::getRegion(){
    return this->recordingRegion;
}

void AbstractFrameCapture::setRegion(QRect* region){
    this->recordingRegion = region;
}

void AbstractFrameCapture::setDrawCursor(bool draw){
    this->drawCursor = draw;
}
int AbstractFrameCapture::getFrameCount()
{
    return this->framesCaptured;
}


