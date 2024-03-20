#ifndef PGM_IMAGEPROCESSOR_H
#define PGM_IMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <string>
#include <memory>
#define u_char unsigned char

namespace NDLMDU011
{
    template <typename T>
    class PGMimageProcessor
    {
    private:
        std::string filename;
        int imageHeight;
        int imageWidth;

        // 2D array for input/output image pixels data
        T **pixels;
        // Container for connected components pairwise with its pixel value
        std::vector<std::shared_ptr<ConnectedComponent>> CCcontainer;

    public:
        int counter = 0;
        T seen; // Minimum valid pixel value for components
        T white;
        T black;

    public:
        PGMimageProcessor(std::string fname);

        ~PGMimageProcessor();

        bool readPGMImage(void);

        bool isValid(T **&pixels_Arr, int width, int height, int x, int y,
                     T prevC, T newC);

        void floodFillLooping(T **&screen, std::shared_ptr<ConnectedComponent> &cc, int width, int height, int x, int y,
                              T prevC, T newC);

        void floodFill(T **&pixels_arr, std::shared_ptr<ConnectedComponent> &cc,
                       int y, int x, T source, const T seen, const T threshold);

        int extractComponents(T threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool setComponentsToArray();

        bool writeComponents(const std::string &outFileName);

        int getComponentCount(void) const;

        int getLargestSize(void) const;

        int getSmallestSize(void) const;

        void printAll() const;

        void printComponentData(const ConnectedComponent &theComponent) const;

        T **getPixelsArr() const;
    };

    void clearArray(unsigned char **arr, int height);

}

#endif