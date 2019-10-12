#include "projectwriter.h"

ProjectWriter::ProjectWriter( const QString &fileName )
{
    file = new QFile(fileName);
    file->open(QIODevice::WriteOnly);

    writer = new QXmlStreamWriter( file );
    writer->setAutoFormatting( true );
}

ProjectWriter::~ProjectWriter()
{
    if ( file->isOpen() )
    {
        file->close();
    }

    delete file;
    delete writer;
}

void ProjectWriter::Save( const ProjectStructure &project )
{
    writer->writeStartDocument();
    this->writeProject( project );
    writer->writeEndDocument();

    file->close();
}

void ProjectWriter::writeProject( const ProjectStructure &project )
{
    writer->writeStartElement( "project" );

    // Name
    writer->writeStartElement( "project-name" );
    writer->writeCharacters( project.Name );
    writer->writeEndElement();

    // Description
    writer->writeStartElement( "project-description" );
    writer->writeCharacters( project.Description );
    writer->writeEndElement();

    // CreatedAt
    writer->writeStartElement( "project-created-at" );
    writer->writeCharacters( QString::number( project.CreatedAt ) );
    writer->writeEndElement();

    // UpdatedAt
    writer->writeStartElement( "project-updated-at" );
    writer->writeCharacters( QString::number( project.UpdatedAt ) );
    writer->writeEndElement();

    // Symbols
    this->writeSymbols( project );

    writer->writeEndElement();
}

void ProjectWriter::writeSymbols(const ProjectStructure &project)
{
    writer->writeStartElement( "symbols" );

    QList<ProjectSymbol>::const_iterator it;
    for ( it = project.Symbols.begin(); it != project.Symbols.end(); ++it )
    {
        this->writeSymbol( (*it) );
    }

    writer->writeEndElement();
}

void ProjectWriter::writeSymbol(const ProjectSymbol &symbol)
{
    writer->writeStartElement( "symbol" );

    // Type
    writer->writeStartElement( "symbol-type" );
    writer->writeCharacters( QString::number( symbol.Type ) );
    writer->writeEndElement();

    // Name
    writer->writeStartElement( "symbol-name" );
    writer->writeCharacters( symbol.Name );
    writer->writeEndElement();

    // Description
    writer->writeStartElement( "symbol-description" );
    writer->writeCharacters( symbol.Description );
    writer->writeEndElement();

    writer->writeStartElement( "symbol-data" );

    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        for ( int j = 0; j < IMAGE_WIDTH; j++ )
        {
            QXmlStreamAttributes atr;

            atr.append( "pos-x", QString::number( i ) );
            atr.append( "pos-y", QString::number( j ) );

            writer->writeStartElement( "symbol-data-item" );

            writer->writeAttributes( atr );
            writer->writeCharacters( ( symbol.Data[i][j] )
                                        ? QString( "true" ) :
                                          QString( "false" ) );

            writer->writeEndElement();
        }
    }

    writer->writeEndElement();

    writer->writeEndElement();
}
