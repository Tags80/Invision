#include "stage.h"

Stage::Stage(QObject *parent, QString name) : QObject(parent)
{
    this->name = name;
}

void Stage::addActor(AbstractFrameCapture *source, QRect rect)
{
    Actor actor;
    actor.source = source;
    actor.rect = rect;
    sources.push_back(&actor);
}

void Stage::removeActor(AbstractFrameCapture *source)
{
    for(int i = 0; i < sources.size(); i++){
        if(source->stringID() == sources.at(i)->source->stringID())
            sources.removeAt(i);

    }
}

QList<Actor *> Stage::getSources()
{
    return sources;
}

QList<Stage *> Stage::getSavedStages()
{
    QList<Stage*> list;
    //get QDir.
    //get all files
    //parse xml and create stages
    return list;
}

void Stage::writeToFile()
{


    /*
    <!DOCTYPE stage>
    <stage name = "name">
        <Actor>
            <source>standardframecapture....</source>
            <rect>0 0 1920 1080</rect>
        </Actor>
        .
        .
        .
    </stage>
    */

    QFile * file = new QFile(dataDir + "/" + name + ".xml");
    QXmlStreamWriter writer(file);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE stage>");
    writer.writeStartElement("stage");
    writer.writeAttribute("name", name);
    foreach(Actor* a , getSources()){
        writer.writeStartElement("Actor");
        writer.writeStartElement("source", a->source->stringID());
        writer.writeEndElement();
        writer.writeStartElement("rect", QString::number(a->rect.topLeft().x()) + " " + QString::number(a->rect.y()) + " " +QString::number(a->rect.width()) + " " + QString::number(a->rect.height()));
        writer.writeEndElement();
        writer.writeEndElement();
    }
    writer.writeEndElement();



    writer.writeEndDocument();

}

void Stage::startRecording(QFile *outputFile)
{

}

void Stage::stopRecording()
{

}
