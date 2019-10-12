#pragma once

#include <QString>

const int IMAGE_WIDTH  = 8;
const int IMAGE_HEIGHT = 16;

struct ProjectSymbol
{
    int Type;

    QString Name;
    QString Description;

    bool Data[IMAGE_HEIGHT][IMAGE_WIDTH];

    ProjectSymbol()
    {
        this->Type = 0;

        this->Name = "";
        this->Description = "";

        for ( int i = 0; i < IMAGE_HEIGHT; i++ )
        {
            for ( int j = 0; j < IMAGE_WIDTH; j++ )
            {
                this->Data[i][j] = false;
            }
        }
    }

    ProjectSymbol( const ProjectSymbol &symbol  )
    {
        this->Type = symbol.Type;

        this->Name = symbol.Name;
        this->Description = symbol.Description;

        for ( int i = 0; i < IMAGE_HEIGHT; i++ )
        {
            for ( int j = 0; j < IMAGE_WIDTH; j++ )
            {
                this->Data[i][j] = symbol.Data[i][j];
            }
        }
    }

    ProjectSymbol& operator= ( const ProjectSymbol &symbol  )
    {
        this->Type = symbol.Type;

        this->Name = symbol.Name;
        this->Description = symbol.Description;

        for ( int i = 0; i < IMAGE_HEIGHT; i++ )
        {
            for ( int j = 0; j < IMAGE_WIDTH; j++ )
            {
                this->Data[i][j] = symbol.Data[i][j];
            }
        }

        return (*this);
    }

    static QString GetNameFromType( int type )
    {
        if ( type == 0 ) return QString( "A" );
        if ( type == 1 ) return QString( "B" );
        if ( type == 2 ) return QString( "C" );
        if ( type == 3 ) return QString( "D" );

        return QString( "unknow" );
    }
};
