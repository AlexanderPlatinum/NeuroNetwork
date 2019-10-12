#pragma once

#include <QFile>
#include <QDebug>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

#include "ProjectSymbol.h"
#include "ProjectStructure.h"

class ProjectReader
{
private:
    QFile *file;
    QXmlStreamReader *reader;

public:
    ProjectReader( const QString &fileName );
    ~ProjectReader();

    void Read( ProjectStructure &project );

private:
    void readProject( ProjectStructure &project );
    void readSymbol( ProjectSymbol &symbol );
};
