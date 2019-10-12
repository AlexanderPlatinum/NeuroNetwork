#pragma once

#include <QList>
#include <QString>

#include "ProjectSymbol.h"

struct TreaningData
{
    float PublicWeight[IMAGE_HEIGHT][IMAGE_WIDTH];
};

struct ProjectStructure
{
    QString Name;
    QString Description;

    int CreatedAt;
    int UpdatedAt;

    TreaningData WeightData;

    QList<ProjectSymbol> Symbols;
};
