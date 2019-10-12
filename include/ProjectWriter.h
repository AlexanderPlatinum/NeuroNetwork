#pragma once

#include <QList>
#include <QFile>
#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

#include "ProjectSymbol.h"
#include "ProjectStructure.h"

class ProjectWriter
{
private:
    QFile *file;
    QXmlStreamWriter *writer;

public:
    ProjectWriter( const QString &fileName );
    ~ProjectWriter();

    void Save( const ProjectStructure &project );

private:
    void writeProject( const ProjectStructure &project );

    void writeSymbol( const ProjectSymbol &symbol );
    void writeSymbols( const ProjectStructure &project );
};
