#ifndef MCROPLABEL_H
#define MCROPLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QLabel>

#include <QDebug>

class mCropLabel : public QLabel
{
public:
    mCropLabel(QWidget* parent);
    QPixmap crop();
    void setPixmap(QPixmap pmap);
protected:
    void mousePressEvent(QMouseEvent* event);
    bool isDown = false;
    QRect cropRect;
    QPixmap currentPixmap, resized, cropped;
    QPoint startDrag, stopDrag;
    float dx, dy;
    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void paintRect(QRect rect);
    QRect getRect(QPoint p1, QPoint p2);
};

#endif // MCROPLABEL_H
