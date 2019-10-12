#pragma once

#include <iostream>

#include "ProjectSymbol.h"

class Network
{
private:

    bool isMemoryAllocated;

    float **weight;
    bool  **data;

public:
    Network();
    ~Network();

    void SetData( bool **_data );

    void Learning();
    bool Recognze();

private:
    bool allocateMemory();
    void freeMemory();
};
