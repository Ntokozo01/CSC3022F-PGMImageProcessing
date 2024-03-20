/**
 * PGM Image Processor helper class that stores the coordinates of
 * connected/touching pixels of the same value, compId is used an identifier
 * @author Mduduzi Ndlovu
 * @date 13/03/2024
 */
#include "ConnectedComponent.h"

#define CC NDLMDU011::ConnectedComponent

CC::ConnectedComponent() : numPixels(0), compID(-1) {}

CC::ConnectedComponent(int num_pixels, int id) : numPixels(num_pixels), compID(id) {}

CC::~ConnectedComponent()
{
    pixelCoords.clear();
}

void CC::addCoords(int x, int y)
{
    std::pair<int, int> coordinate(x, y);
    pixelCoords.push_back(coordinate);
    numPixels = pixelCoords.size();
}

void CC::addCoords(std::pair<int, int> coords_pair){
    pixelCoords.push_back(coords_pair);
    numPixels = pixelCoords.size();
}

int CC::getX(int index) const
{
    return pixelCoords.at(index).first;
}

int CC::getY(int index) const
{
    return pixelCoords.at(index).second;
}

// Copy constructor
CC::ConnectedComponent(const CC &cc) : compID(cc.compID), numPixels(cc.numPixels)
{
    int length = cc.numPixels;

    for (int i = 0; i < length; ++i)
    {
        pixelCoords.push_back(cc.pixelCoords[i]);
    }
}

// Move constructor
CC::ConnectedComponent(CC &&cc) : compID(cc.compID), numPixels(cc.numPixels)
{
    cc.pixelCoords.clear();
}

/// ConnectedComponent Copy Assignment operator
CC &CC::operator=(const CC &rhs) {
    if (this != &rhs){
        compID = rhs.compID;
        numPixels = rhs.numPixels;

        if (!pixelCoords.empty()) { pixelCoords.clear(); }

        pixelCoords = rhs.pixelCoords;
    }
    return *this;
}

/// ConnectedComponent Move Assignment operator
CC &CC::operator=(CC &&rhs) {
    if (this != &rhs){
        compID = rhs.compID;
        numPixels = rhs.numPixels;

        if (!pixelCoords.empty()) { pixelCoords.clear(); }
        
        pixelCoords = rhs.pixelCoords;
    
        rhs.compID = -1;
        rhs.numPixels = 0;
        rhs.pixelCoords.clear();
    }
    return *this;
}