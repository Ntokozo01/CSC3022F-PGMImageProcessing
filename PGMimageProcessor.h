#ifndef PGM_IMAGEPROCESSOR_H
#define PGM_IMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <string>
#include <memory>
#define u_char unsigned char

namespace NDLMDU011
{
    class PGMimageProcessor
    {
    private:
        std::string filename;
        int imageHeight;
        int imageWidth;

        // 2D array for input/output image pixels data
        unsigned char **pixels;
        // Container for connected components pairwise with its pixel value
        std::vector<std::shared_ptr<ConnectedComponent>> CCcontainer;

    public:
        int counter = 0;

    public:
        PGMimageProcessor(std::string fname);

        ~PGMimageProcessor();

        bool readPGMImage(void);

        void floodFill(unsigned char **&pixels_arr, std::shared_ptr<ConnectedComponent> &cc,
                       int y, int x, u_char source, u_char &seen, u_char &threshold);

        int extractComponents(u_char threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool setComponentsToArray();

        bool writeComponents(const std::string &outFileName);

        int getComponentCount(void) const;

        int getLargestSize(void) const;

        int getSmallestSize(void) const;

        void printAll() const;

        void printComponentData(const ConnectedComponent &theComponent) const;

        unsigned char **getPixelsArr() const;
    };

    void clearArray(unsigned char **arr, int height);

}

#endif