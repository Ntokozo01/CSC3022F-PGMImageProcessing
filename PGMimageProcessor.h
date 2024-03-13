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
        std::vector< std::pair<std::shared_ptr<ConnectedComponent>, unsigned char> > CCcontainer; 

    public:
        PGMimageProcessor(std::string fname);

        ~PGMimageProcessor();

        bool readPGMImage(void);

        void floodFill(std::shared_ptr<ConnectedComponent> cc,
         int y, int x, int source, int minValidSize, int threshold);

        int extractComponents(u_char threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool writeComponents(const std::string &outFileName);

        int getComponentCount(void) const;

        int getLargestSize(void) const;

        int getSmallestSize(void) const;

        void printAll() const;

        void printComponentData(const ConnectedComponent &theComponent) const;

    };  

}

#endif