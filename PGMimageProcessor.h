/*
     MDUDUZI NDLOVU - NDLMDU011
    PGMimageProcessor class header file
 */

#ifndef PGM_IMAGEPROCESSOR_H
#define PGM_IMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <string>
#include <memory>
#include <fstream>

namespace NDLMDU011
{
    // A datatype/object for holding the PPM pixel value (red, green and red)
    struct PPMpixel
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;

        // No arg constructor which intialises values to zero
        PPMpixel();

        // Custom constructor to assign the red, green and blue values of the pixel
        PPMpixel(unsigned char Red, unsigned char Green, unsigned char Blue);

        // Custom constructor for converting an int value PPMpixel which is grayscale pixel of intensity value
        PPMpixel(int value);

        // Cast operator to convert PPMpixel to a 1-byte unsigned char pixel value
        operator unsigned char();
    };

    // friend method to overload the input file stream to read a PGM pixel
    std::ifstream &operator>>(std::ifstream &file_read, PPMpixel &ppm);

    // friend method to overload the output file stream to read a PGM pixel
    std::ofstream &operator<<(std::ofstream &file_out, PPMpixel &ppm);

    // Image Processor class for PGM and PPM images to extract Connected Components detected on some threshold value
    template <typename T>
    class PGMimageProcessor
    {
    private:
        std::string filename;
        int imageHeight;
        int imageWidth;

        // 2D array for input/output image pixels data
        T **pixels = nullptr;

        // 2D array which is contains image pixels data as PPMpixel, it is for use with Writing PPM bounded image
        PPMpixel **ppmArray = nullptr;
        // Container for connected components pairwise with its pixel value
        std::vector<std::shared_ptr<ConnectedComponent>> CCcontainer;

    public:
        int counter = 0;
        T seen = 0;    // Minimum valid pixel value for components
        T white = 255; // White pixel value for indicating connected components on thresholded image
        T black = 0;   // Black pixel value for indicating non-components pixels on the thresholded image

    public:
        // Default Constructor
        PGMimageProcessor();

        // Custom Constructor with input image filename
        PGMimageProcessor(std::string fname);

        // Destructor
        ~PGMimageProcessor();

        // Dynamically allocates memory for 2D image of the size width x height and set each value as seen/black/zero 
        void initialiseArray(T **&arr, int height, int width);

        // Reads the PGM or PPM image of the supplied filename and writes its pixel values to pixels
        bool readPGMImage(void);

        // Copies pixels value to ppmArray as PPMpixel values
        bool convertToPPMArray(void);

        /* Checks that the pixel on pixels_Arr (2D array) at coords x and y is within image bounds and is part of the connected
         Component of pixel value source  */
        bool isValid(T **&pixels_Arr, int width, int height, int x, int y,
                     T source, T seen);

        // Finds the pixels in pixelsArr that are part of the connected component (cc) and adds their coordinates to the component
        // Checks N, S, W and E pixels of each pixel and process them using a queue to go in order
        void floodFillLooping(T **&pixes_Arr, std::shared_ptr<ConnectedComponent> &cc, int width, int height, int x, int y,
                              T source, T seen);

        // Finds the connected components of pixel values >= threshold value and their number of pixels >= minValidSize
        // Calls on the floodFill algorith to get each connected component's pixels
        int extractComponents(T threshold, int minValidSize);


        int filterComponentsBySize(int minSize, int maxSize);

        // Set all the pixels which are part of the connected component White(255) and the rest black (0) 
        bool setComponentsToArray();

        // Calls on setComponentsToArray to put components into pixes as black and white then writes out the output image
        bool writeComponents(const std::string &outFileName);

        // Writes a PPM image of the original input image with detected components in bounding boxes using the
        // method addBoundingBoxes to set up the pixel as PPMpixels on ppmArray
        bool writeBoundedPPM(const std::string &outFileName);

        // Adds the Bounding boxes (red color) to the original input image as PPMpixels for the detected components
        // using the min and max X-Y values of the connected Components coordinates
        bool addBoundingBoxes(void);

        // Returns the number of Connected components in the container
        int getComponentCount(void) const;

        // Returns the size of Connected component with largest number of pixels
        int getLargestSize(void) const;

        // Returns the size of Connected component with smalllest number of pixels
        int getSmallestSize(void) const;

        // Print out all the Connected  Component data in the container to the console
        void printAll() const;

        // Print out the Connected Component data to the console
        void printComponentData(const ConnectedComponent &theComponent) const;

        // Returns a copy of the 2D array of pixel values by Value
        T **getPixelsArr() const;

        // Deletes the dynamically allocated memory of the 2D array
        void clearArray(T **&arr, int height);

    };

}

#endif