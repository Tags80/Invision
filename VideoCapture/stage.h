#ifndef STAGE_H
#define STAGE_H

#include <QObject>
#include <QFile>
#include "grabbers/abstractframecapture.h"

///A struct to wrap the source and its location on stage
typedef struct Actor{
    AbstractFrameCapture* source;
    QPoint topLeft;
    QRect rect;
}Actor;

class Stage : public QObject
{
    Q_OBJECT
public:
    explicit Stage(QObject *parent = 0, QString name = "Stage");

    ///
    /// \brief addCharacter adds a source to the stage. The sources will be layered first to last, back to front respectively.
    /// Make sure that the primary source is added first. (ie add screen capture or game capture first)
    /// \param source the source to be added
    /// \param topLeft top left point in the whole source. Main source should be QPoint(0,0)
    /// \param rect the rectangle of the size of the source
    ///
    void addActor(AbstractFrameCapture* source, QPoint topLeft, QRect rect);

    ///
    /// \brief removeCharacter remove a character from the source
    /// \param source the character to be removed
    ///
    void removeActor(AbstractFrameCapture* source);

    ///
    /// \brief getSources gets all the currently added sources
    /// \return list of all of the sources
    ///
    QList<Actor*> getSources();

private:
    QList<Actor*> sources;
    ///
    /// \brief writeToFile writes the current stage configuration to an XML file
    ///
    void writeToFile();
    ///
    /// \brief readFromFile reads a stage configuration from and XML file
    ///
    void readFromFile();

signals:

public slots:
    void startRecording(QFile *outputFile);
    void stopRecording();
};

#endif // STAGE_H
