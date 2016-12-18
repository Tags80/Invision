#include "mcroplabel.h"



mCropLabel::mCropLabel(QWidget *parent)
{

}

QPixmap mCropLabel::crop()
{
    QPoint topLeft(dx * cropRect.topLeft().x(), dy * cropRect.topLeft().y());
    QPoint bottomRight( dx * cropRect.bottomRight().x(), dy *cropRect.bottomRight().y());
    QRect rect = QRect(topLeft,bottomRight);
//    qDebug() << "Is Empty:" << rect.isEmpty() << "Is Valid:"<<rect.isValid() << endl;
//    qDebug() << "Top: " << rect.top() << "Bottom:" << rect.bottom() << "Left:" << rect.left() << "Right:" << rect.right() <<endl;
    return currentPixmap.copy(rect.normalized());//This is magic to me. I don't know why reversing the verticies works but fuck it.
}

void mCropLabel::mousePressEvent(QMouseEvent *event)
{
    //start tracking
    isDown = true;
    startDrag = event->pos();
    qDebug() << "mouse down: (" << event->pos().x() << "," << event->pos().y() << ")" <<endl;
}
void mCropLabel::mouseReleaseEvent(QMouseEvent *event){
    //stop tracking
    isDown = false;
    stopDrag = event->pos();
    cropRect = getRect(startDrag,stopDrag);
    paintRect(cropRect);
    qDebug() << "mouse up: (" << event->pos().x() << "," << event->pos().y() << ")" <<endl;
}
void mCropLabel::mouseMoveEvent(QMouseEvent *event){
    if(!isDown) return;

    stopDrag = event->pos();
    cropRect = getRect(startDrag,stopDrag);
    paintRect(cropRect);
    qDebug() << "mouse move: (" << event->pos().x() << "," << event->pos().y() << ")" <<endl;

}

void mCropLabel::setPixmap(QPixmap pmap){
    resized = pmap.scaled(this->size());
    QLabel::setPixmap(resized);
    currentPixmap = pmap;
    dx = currentPixmap.width()*1.0f / this->width();
    dy = currentPixmap.height()*1.0f / this->height();

}

void mCropLabel::resizeEvent(QResizeEvent *){
    resized = currentPixmap.scaled(this->size());
    QLabel::setPixmap(resized);
    dx = currentPixmap.width() *1.0f / this->width();
    dy = currentPixmap.height()*1.0f / this->height();
}
void mCropLabel::paintRect(QRect rect){
    resized = currentPixmap.scaled(this->size());
    QPainter p(&resized);
    p.setBrush(Qt::NoBrush);
    p.setPen(Qt::red);
    p.drawRect(rect);
    p.end();
    QLabel::setPixmap(resized);

}

QRect mCropLabel::getRect(QPoint p1, QPoint p2){
    if(p1.x() < p2.x() && p1.y() > p2.y()){ //x increasing y decreasing
        return QRect(p1,p2);
    }
    else if(p1.x() > p2.x() && p1.y() > p2.y()){ //x decreasing y decreasing
        return(QRect(QPoint(p2.x(),p1.y()),QPoint(p1.x(),p2.y())));
    }
    else if(p1.x() > p2.x() && p1.y() < p2.y()){ //x decreasing y increasing
        return QRect(p2,p1);
    }
    else if(p1.x() < p2.x() && p1.y() < p2.y()){ //x increasing y increasing
        return QRect(QPoint(p1.x(),p2.y()),QPoint(p2.x(),p1.y()));
    }
    else return QRect(p1,p1); //just a simple point
}
