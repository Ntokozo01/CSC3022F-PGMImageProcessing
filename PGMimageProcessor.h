#ifndef PGM_IMAGEPROCESSOR_H
#define PGM_IMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <string>
#define u_char unsigned char


namespace NDLMDU011
{
    class PGMimageProcessor
    {
    private:
        std::string filename;
        int imageHeight;
        int imageWidth;
        unsigned char **pixels;

    public:
        PGMimageProcessor(std::string fname);

        ~PGMimageProcessor();

        bool readPGMImage(void);

        int extractComponents(u_char threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool writeComponents(const std::string &outFileName);

        int getComponentCount(void) const;

        int getLargestSize(void) const;

        int getSmallestSize(void) const;

        void printComponentData(const ConnectedComponent &theComponent) const;

    };  

}

#endif