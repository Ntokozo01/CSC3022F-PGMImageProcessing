#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector>

namespace NDLMDU011
{
    class ConnectedComponent
    {
    private:
        int numPixels;
        int compID;
        std::vector<std::pair<int, int>> pixelCoords;

    public:
        ConnectedComponent();
        ConnectedComponent(int num_pixels, int id);
        ~ConnectedComponent();

        ConnectedComponent(const ConnectedComponent &cc);
        ConnectedComponent(ConnectedComponent &&cc);

        ConnectedComponent &operator=(const ConnectedComponent &rhs);
        ConnectedComponent &operator=(ConnectedComponent &&rhs);

        void addCoords(int x, int y);

        int getX(int index) const;
        int getY(int index) const;

        int getNumPixels() const {   return numPixels; }

        int getID() const {  return compID; }
    };
}

#endif