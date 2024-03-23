#include "PGMimageProcessor.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    int minSize = 3, maxSize = 100000000;
    int threshold, minValidSize = 1;
    std::string inputImageFile, outImageFile, outputPPM;
    bool bfilter = false, bprintComponents = false, bwriteComponents = false, bWritePPM = false;
    std::string imageType;

    inputImageFile = "chess.pgm";
    // make run ./driver.exe options="-s 3 10000 -t 128 -p -w sample.pgm " filename="chess.pgm"
    if (argc >= 3)
    {
        int k = 0;
        while (k < argc)
        {
            if (std::string(argv[k]) == "-s") // Flag to indicate to filter the components
            {
                bfilter = true;
                minSize = std::stoi(argv[++k]);
                maxSize = std::stoi(argv[++k]);
                minValidSize = minSize;
            }
            else if (std::string(argv[k]) == "-t") // Flag for the threshold value
            {
                threshold = std::stoi(argv[++k]);
                threshold = std::min(std::max(threshold, 0), 255); // Bound threshold between 0 and 255- both inclusive
            }
            else if (std::string(argv[k]) == "-p") // Flag indicating to print components data
            {
                bprintComponents = true;
            }
            else if (std::string(argv[k]) == "-w") // Flag to indicate to write an output image
            {
                bwriteComponents = true;
                outImageFile = argv[++k];

                int period = outImageFile.find_last_of(".");
                if (period < 0) // Check whether an extension is included, if not included default to pgm
                {
                    outImageFile += ".pgm"; 
                }
            }
            else if (std::string(argv[k]) == "-b") // Flag to indicate to write a bounded boxed PPM for detected components
            {
                bWritePPM = true;
                outputPPM = argv[++k];

                int period = outputPPM.find_last_of(".");
                // The output image should be PPM, enforcing that
                if (period < 0)
                {
                    outputPPM += ".ppm"; 
                } else {
                    outputPPM = outputPPM.substr(0,period) +".ppm";
                }
            }
            else // The argument as the input inpu file name
            {
                inputImageFile = argv[argc - 1];
                int period = inputImageFile.find_last_of(".");
                if (period <= 0)
                {
                    inputImageFile = inputImageFile + ".pgm";
                    imageType = "pgm";
                }
                else
                {
                    imageType = inputImageFile.substr(period + 1);
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

    if (imageType == "pgm") // If the input image is a PGM image, use a unsigned char PGMimageProcessor object
    {

        NDLMDU011::PGMimageProcessor<unsigned char> *imageProcessor = new NDLMDU011::PGMimageProcessor<unsigned char>(inputImageFile);

        std::cout << "Reading Image..." << std::endl;
        bool image_read = imageProcessor->readPGMImage();

        if (!image_read)
        {
            std::cerr << "File Not Found." << std::endl;
            return 1;
        }
        std::cout << "Image read successfully." << std::endl;

        std::cout << "\nExtracting Components ..." << std::endl;
        int num_comp = imageProcessor->extractComponents(threshold, minValidSize);
        std::cout << "Number of connected components (unfiltered): " << num_comp << std::endl;

        if (bwriteComponents)
        {
            std::cout << "\nWriting.. Connected components to PGM Image file " << std::endl;
            bool done = imageProcessor->writeComponents(outImageFile);
        }

        if (bfilter)
        {
            std::cout << "\nFiltering Components by minimum size: " << minSize << " and maximum size: " << maxSize << std::endl;
            int size = imageProcessor->filterComponentsBySize(minSize, maxSize);
            std::cout << "Number of connected components (filtered): " << size << std::endl;
        }

        if (bprintComponents)
        {
            std::cout << "\nPrinting out Component Data..." << std::endl;
            imageProcessor->printAll();

            std::cout << "Number of connected components: " << imageProcessor->getComponentCount() << std::endl;

            int ans = imageProcessor->getLargestSize();
            std::cout << "The largest Connected Component has " << ans << " pixels." << std::endl;

            ans = imageProcessor->getSmallestSize();
            std::cout << "Smallest Connected Component has " << ans << " pixels." << std::endl;
        }

        if (bWritePPM)
        {
            std::cout << "\nWriting out a PPM image with bounded boxes.." << std::endl;
            imageProcessor->writeBoundedPPM(outputPPM);
            std::cout << "PPM Image written out" << std::endl;
        }

        delete imageProcessor;
    }
    else if (imageType == "ppm")// If the input image is a PPM image, use a PPMpixel PGMimageProcessor object
    {
        NDLMDU011::PGMimageProcessor<NDLMDU011::PPMpixel> *imageProcessor = new NDLMDU011::PGMimageProcessor<NDLMDU011::PPMpixel>(inputImageFile);

        std::cout << "Reading Image..." << std::endl;
        bool image_read = imageProcessor->readPGMImage();

        if (!image_read)
        {
            std::cerr << "File Not Found." << std::endl;
            return 1;
        }
        std::cout << "Image read successfully." << std::endl;

        std::cout << "\nExtracting Components ..." << std::endl;
        int num_comp = imageProcessor->extractComponents(threshold, minValidSize);
        std::cout << "Number of connected components (unfiltered): " << num_comp << std::endl;

        if (bwriteComponents)
        {
            std::cout << "\nWriting.. Connected components to PGM Image file " << std::endl;
            bool done = imageProcessor->writeComponents(outImageFile);
        }

        if (bfilter)
        {
            std::cout << "\nFiltering Components by minimum size: " << minSize << " and maximum size: " << maxSize << std::endl;
            int size = imageProcessor->filterComponentsBySize(minSize, maxSize);
            std::cout << "Number of connected components (filtered): " << size << std::endl;
        }

        if (bprintComponents)
        {
            std::cout << "\nPrinting out Component Data..." << std::endl;
            imageProcessor->printAll();

            std::cout << "Number of connected components: " << imageProcessor->getComponentCount() << std::endl;

            int ans = imageProcessor->getLargestSize();
            std::cout << "The largest Connected Component has " << ans << " pixels." << std::endl;

            ans = imageProcessor->getSmallestSize();
            std::cout << "Smallest Connected Component has " << ans << " pixels." << std::endl;
        }

        if (bWritePPM)
        {
            std::cout << "\nWriting out a PPM image with bounded boxes.." << std::endl;
            imageProcessor->writeBoundedPPM(outputPPM);
            std::cout << "PPM Image written out" << std::endl;
        }

        delete imageProcessor;
    }

    return 0;
}