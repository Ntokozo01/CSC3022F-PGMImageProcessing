#include "PGMimageProcessor.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

    int minSize = 3;
    int maxSize, image_width, image_height, threshold;
    std::string inputImageFile, outImageFile;
    bool bfilter = false, bprintComponents = false, bwriteComponents = false;

    inputImageFile = "chess.pgm";
    // make run ./driver.exe options="-s 3 10000 -t 128 -p -w sample.pgm " filename="chess.pgm"
    if (argc >= 10)
    {
        int k = 0;
        while (k < argc)
        {
            std::cout << argv[k] << std::endl;
            if (std::string(argv[k]) == "-s")
            {
                bfilter = true;
                minSize = std::stoi(argv[++k]);
                maxSize = std::stoi(argv[++k]);
            }
            else if (std::string(argv[k]) == "-t")
            {
                threshold = std::stoi(argv[++k]);
            }
            else if (std::string(argv[k]) == "-p")
            {
                bprintComponents = true;
            }
            else if (std::string(argv[k]) == "-w")
            {
                bwriteComponents = true;
                outImageFile = argv[++k];
            }
            else
            {
                inputImageFile = argv[9];
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
    std::cout << "threshold: " << threshold << std::endl;
    std::cout << "Print components data?: " << bprintComponents << std::endl;

    return 1;
}