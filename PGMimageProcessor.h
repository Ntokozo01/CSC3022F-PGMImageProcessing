#ifndef PGM_IMAGEPROCESSOR_H
#define PGM_IMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <string>
#include <memory>
#include <fstream>

namespace NDLMDU011
{
    struct PPMpixel
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;

        PPMpixel();

        PPMpixel(unsigned char Red, unsigned char Green, unsigned char Blue);

        PPMpixel(int value);

        operator unsigned char();
    };

    std::ifstream &operator>>(std::ifstream &file_read, PPMpixel &ppm);

    std::ofstream &operator<<(std::ofstream &file_out, PPMpixel &ppm);

    template <typename T>
    class PGMimageProcessor
    {
    private:
        std::string filename;
        int imageHeight;
        int imageWidth;

        // 2D array for input/output image pixels data
        T **pixels = nullptr;
        PPMpixel **ppmArray = nullptr;
        // Container for connected components pairwise with its pixel value
        std::vector<std::shared_ptr<ConnectedComponent>> CCcontainer;

    public:
        int counter = 0;
        T seen = 0; // Minimum valid pixel value for components
        T white = 255;
        T black = 0;

    public:
        PGMimageProcessor(std::string fname);

        ~PGMimageProcessor();

        bool readPGMImage(void);

        bool convertToPPMArray(void);

        bool isValid(T **&pixels_Arr, int width, int height, int x, int y,
                     T prevC, T newC);

        void floodFillLooping(T **&screen, std::shared_ptr<ConnectedComponent> &cc, int width, int height, int x, int y,
                              T prevC, T newC);

        int extractComponents(T threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool setComponentsToArray();

        bool writeComponents(const std::string &outFileName);

        bool writePPMComponents(const std::string &outFileName);

        bool addBoundingBoxes(void);

        int getComponentCount(void) const;

        int getLargestSize(void) const;

        int getSmallestSize(void) const;

        void printAll() const;

        void printComponentData(const ConnectedComponent &theComponent) const;

        T **getPixelsArr() const;

        void clearArray(T **arr, int height);

        void initialiseArray(T **&arr, int height, int width);
    };

    //template <>
    //bool PGMimageProcessor<PPMpixel>::writePPMComponents(const std::string &outFileName);

    //template <>
    //bool PGMimageProcessor<PPMpixel>::addBoundingBoxes(void);
}

#endif