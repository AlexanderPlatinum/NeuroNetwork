#include "ProjectReader.h"

ProjectReader::ProjectReader( const QString &fileName )
{
    file = new QFile( fileName );
    file->open( QIODevice::ReadOnly );

    reader = new QXmlStreamReader( file );
}

ProjectReader::~ProjectReader()
{
    if ( file->isOpen() )
    {
        file->close();
    }

    delete reader;
    delete file;
}

void ProjectReader::Read( ProjectStructure &project )
{
    reader->readNext();
    this->readProject( project );
    file->close();
}

void ProjectReader::readProject(ProjectStructure &project)
{
    while ( !reader->atEnd() )
    {
        if ( reader->isStartElement() )
        {
            // Name
            if ( reader->name() == "project-name" )
            {
                project.Name = reader->readElementText();
            }

            // Description
            if ( reader->name() == "project-description" )
            {
                project.Description = reader->readElementText();
            }

            // CreatedAt
            if ( reader->name() == "project-created-at" )
            {
                QString createdAt = reader->readElementText();
                project.CreatedAt = createdAt.toInt();
            }

            // UpdatedAt
            if ( reader->name() == "project-updated-at" )
            {
                QString updatedAt = reader->readElementText();
                project.UpdatedAt = updatedAt.toInt();
            }

            // Symbols
            if ( reader->name() == "symbols" )
            {
                reader->readNext(); // go to symbol tag
                while( true )
                {
                    if ( reader->name() == QString() )
                    {
                        reader->readNext();
                        continue;
                    }

                    if ( ( reader->name() == "symbols" ) &&
                          reader->isEndElement() )
                    {
                        break;
                    }

                    ProjectSymbol symbol;
                    this->readSymbol( symbol );
                    project.Symbols.push_back( symbol );

                    reader->readNext();
                }
            }
        }

        reader->readNext();
    }
}

void ProjectReader::readSymbol(ProjectSymbol &symbol)
{
    // skip symbol tag
    reader->readNext();

    while ( true )
    {
        if ( ( reader->name() == "symbol" ) && ( reader->isEndElement() ) )
        {
            return;
        }

        if ( reader->isStartElement() )
        {
            if ( reader->name() == "symbol-type" )
            {
                QString type = reader->readElementText();
                symbol.Type = type.toInt();
            }

            if ( reader->name() == "symbol-name" )
            {
                symbol.Name = reader->readElementText();
            }

            if ( reader->name() == "symbol-description" )
            {
                symbol.Description = reader->readElementText();
            }

            if ( reader->name() == "symbol-data-item" )
            {
                QXmlStreamAttributes atrib = reader->attributes();

                int posX = atrib.value("pos-x").toInt();
                int posY = atrib.value("pos-y").toInt();

                QString value = reader->readElementText();
                bool itemData = ( value == "true" ) ? true : false;

                symbol.Data[posX][posY] = itemData;
            }
        }

        reader->readNext();
    }
}
