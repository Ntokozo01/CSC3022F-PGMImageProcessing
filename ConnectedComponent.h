
/*
     MDUDUZI NDLOVU - NDLMDU011
    Connected Component header file
 */

#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector>

namespace NDLMDU011
{
    /// Helper class for the PGMimageProcessor which contains the coordinates of the connected pixels on the image.
    /// Connected pixels are those that pixels adjacent to each other which have the same pixel value
    class ConnectedComponent
    {
    private:
        int numPixels;
        int compID;
        std::vector<std::pair<int, int>> pixelCoords;

    public:
        unsigned char pixel_value; // The pixel value of this connected component

        /// ConnectedComponent no-args constructor
        ConnectedComponent();
        /// ConnectedComponent Custom constructor
        ConnectedComponent(int num_pixels, int id);
        /// ConnectedComponent destructor
        ~ConnectedComponent();

        /// ConnectedComponent Copy Constructor
        ConnectedComponent(const ConnectedComponent &cc);

        /// ConnectedComponent Move Constructor
        ConnectedComponent(ConnectedComponent &&cc);

        /// ConnectedComponent Copy Assignment operator
        ConnectedComponent &operator=(const ConnectedComponent &rhs);
        /// ConnectedComponent Move Assignment operator
        ConnectedComponent &operator=(ConnectedComponent &&rhs);

        /// @brief add the coordinate of the pixel
        /// in the image file which is contained in this component
        /// @param x horizontal coord of the pixel in input image
        /// @param y vertical coord of the pixel in input image
        void addCoords(int x, int y);

        void addCoords(std::pair<int, int> pair);

        /// @brief returns the x value of the coordinate at specified index
        /// @param index of the pair in the vector container
        /// @return x value coordinate
        int getX(int index) const;

        /// @brief returns the y value of the coordinate at specified index
        /// @param index of the pair in the vector container
        /// @return y value coordinate
        int getY(int index) const;

        // returns the number of pixels of this ConnectedComponent
        int getNumPixels() const { return numPixels; }
        // returns the integer ID of this ConnectedComponent
        int getID() const { return compID; }

        // Returns the coordinates for minimum X and Y value as coordinate for starting position of
        // the bounding box
        std::pair<int, int> getMinXY(void);

        // Returns the coordinates for maximum X and Y value as coordinate for ending position of
        // the bounding box
        std::pair<int, int> getMaxXY(void);

        unsigned char getPixelValue(void) const { return pixel_value; }
    };
}

#endif