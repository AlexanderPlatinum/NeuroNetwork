#include "Network.h"

Network::Network()
{
    isMemoryAllocated = allocateMemory();
}

Network::~Network()
{
    freeMemory();
}

void Network::SetData(bool **_data)
{
    data = _data;
}

void Network::Learning()
{
    if ( isMemoryAllocated == false ) return;
}

bool Network::Recognze()
{
    if ( isMemoryAllocated == false ) return false;
    return true;
}

bool Network::allocateMemory()
{
    try
    {
        weight = new float*[IMAGE_HEIGHT];

        for ( int i = 0; i < IMAGE_HEIGHT; i++ )
        {
            weight[i] = new float[IMAGE_WIDTH];
        }

        return true;
    }
    catch ( std::bad_alloc &e )
    {
        std::cerr << __PRETTY_FUNCTION__ << " " << e.what() << std::endl;
        return false;
    }
}

void Network::freeMemory()
{
    for ( int i = 0; i < IMAGE_HEIGHT; i++ )
    {
        delete [] weight[i];
    }

    delete [] weight;

    isMemoryAllocated = false;
}
