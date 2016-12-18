#ifndef STAGE_H
#define STAGE_H

#include <QObject>
#include <QFile>
#include <QStandardPaths>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "grabbers/abstractframecapture.h"

///A struct to wrap the source and its location on stage
typedef struct Actor{
    AbstractFrameCapture* source;
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
    /// \param rect the rectangle of the size of the source
    ///
    void addActor(AbstractFrameCapture* source, QRect rect);

    ///
    /// \brief moveActor sets the position of a source to a new source
    /// \param source the source to be moved
    /// \param rect new location of the source
    ///
    void moveActor(AbstractFrameCapture* source, QRect rect);
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

    ///
    /// \brief readFromFile reads a stage configuration from and XML file
    ///
    static void readFromFile(QString name);

    ///
    /// \brief getSavedStages returns all stages saved in the stage save dir location
    /// \return a list of all of the saved stages
    ///
    static QList<Stage *> getSavedStages();

private:
    ///the name of this scene
    QString name;
    QList<Actor*> sources;
    ///
    /// \brief writeToFile writes the current stage configuration to an XML file
    ///
    void writeToFile();

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);


signals:

public slots:
    void startRecording(QFile *outputFile);
    void stopRecording();
};

#endif // STAGE_H
