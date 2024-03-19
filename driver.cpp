#include "PGMimageProcessor.h"
#include "PGMimageProcessor.cpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

    int minSize = 3, maxSize = 100000000;
    int threshold, minValidSize = 1;
    std::string inputImageFile, outImageFile;
    bool bfilter = false, bprintComponents = false, bwriteComponents = false;

    inputImageFile = "chess.pgm";
    // make run ./driver.exe options="-s 3 10000 -t 128 -p -w sample.pgm " filename="chess.pgm"
    if (argc >= 3)
    {
        int k = 0;
        while (k < argc)
        {
            if (std::string(argv[k]) == "-s")
            {
                bfilter = true;
                minSize = std::stoi(argv[++k]);
                maxSize = std::stoi(argv[++k]);
            }
            else if (std::string(argv[k]) == "-t")
            {
                threshold = std::stoi(argv[++k]);
                threshold = std::min(std::max(threshold, 0), 255); // Bound threshold between 0 and 255- both inclusive
            }
            else if (std::string(argv[k]) == "-p")
            {
                bprintComponents = true;
            }
            else if (std::string(argv[k]) == "-w")
            {
                bwriteComponents = true;
                outImageFile = argv[++k];

                int period = outImageFile.find_last_of(".");
                if (period < 0)
                {
                    outImageFile += ".pgm"; // outImageFile.substr(0,period);
                }
            }
            else
            {
                inputImageFile = argv[argc - 1];
                int period = inputImageFile.find_last_of(".");
                if (period < 0)
                {
                    inputImageFile = inputImageFile + ".pgm";
                }
            }
            k++;
        }
    }
    else
    {
        threshold = 128;
        outImageFile = "outputFile.pgm";
        maxSize = 100000000;
    }

    std::cout << "Input file name: " << inputImageFile << std::endl;
    std::cout << "Minimum size: " << minSize << " Maximum size: " << maxSize << std::endl;
    std::cout << "Threshold: " << threshold << std::endl;
    std::cout << "Print components data?: " << bprintComponents << std::endl;

    NDLMDU011::PGMimageProcessor<unsigned char> *imageProcessor = new NDLMDU011::PGMimageProcessor<unsigned char>(inputImageFile);
    bool image_read = imageProcessor->readPGMImage();
    if (!image_read)
    {
        std::cerr << "File Not Found." << std::endl;
        return 1;
    }

    std::cout << "Image read successfully."
              << std::endl
              << std::endl;

    imageProcessor->extractComponents(threshold, minSize);
    std::cout << "Number of connected components (unfiltered): " << imageProcessor->getComponentCount() << std::endl;

    int ans = imageProcessor->getLargestSize();
    std::cout << "The largest Connected Component has " << ans << " pixels." << std::endl;

    ans = imageProcessor->getSmallestSize();
    std::cout << "Smallest Connected Component has " << ans << " pixels." << std::endl;

    if (bfilter)
    {
        std::cout << "\nFiltering Components by minimum size: " << minSize << " and maximum size: " << maxSize << std::endl;
        int size = imageProcessor->filterComponentsBySize(minSize, maxSize);

        std::cout << "Number of connected components (filtered): " << size << std::endl;
        std::cout << "The largest Connected Component has " << imageProcessor->getLargestSize() << " pixels." << std::endl;
        std::cout << "Smallest Connected Component has " << imageProcessor->getSmallestSize() << " pixels." << std::endl;
    }

    if (bwriteComponents)
    {
        std::cout << "\nWriting.. Connected components to PGM Image file " << std::endl;
        bool done = imageProcessor->writeComponents(outImageFile);
    }

    if (bprintComponents)
    {
        std::cout << "\nPrinting the Conected components to console..." << std::endl;
        imageProcessor->printAll();
    }
    delete imageProcessor;

    return 0;
}