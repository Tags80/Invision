#include "stage.h"

Stage::Stage(QObject *parent, QString name) : QObject(parent)
{
    Q_UNUSED(name);
}

void Stage::addActor(AbstractFrameCapture *source, QPoint topLeft, QRect rect)
{
    Actor actor;
    actor.source = source;
    actor.topLeft = topLeft;
    actor.rect = rect;
    sources.push_back(&actor);
}

void Stage::removeActor(AbstractFrameCapture *source)
{
    for(int i = 0; i < sources.size(); i++){

    }
}

QList<Actor *> Stage::getSources()
{
    return sources;
}

void Stage::writeToFile()
{

}

void Stage::readFromFile()
{

}
