#include "PGMimageProcessor.h"

#include "PGMimageProcessor.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <fstream>

#define PGMIP NDLMDU011::PGMimageProcessor

// Custom constructor
PGMIP::PGMimageProcessor(std::string fname) : filename(fname)
{
    imageHeight = 0; // number of vertical pixels
    imageWidth = 0;  // number of horizontal pixels in an image
}

PGMIP::~PGMimageProcessor()
{
    std::cout << "Destructor" << std::endl;

    for (int i = 0; i < imageHeight; ++i){
        delete[] pixels[i];
    }
    delete[] pixels;
}


bool PGMIP::readPGMImage()
{
    // Read the PGM input image and save the data in the appropriate variables
    std::ifstream file_reader(filename, std::fstream::binary);
    std::cout << "Reading file: " << filename << std::endl;
    std::string line;
    std::getline(file_reader, line); // get the Magic number of input image which is "P5"

    // Read the next line and check if it is a comment line, repeats until the line read is not a comment
    std::getline(file_reader, line);
    while (line[0] == '#') // comment lines start with the character '#'
    {
        std::getline(file_reader, line);
    }

    // Next line after comments is the image dimensions line which is width and height, extracts it from the line
    std::istringstream iss(line);
    iss >> imageWidth >> imageHeight;

    std::getline(file_reader, line); // get the maximum value of the pixels in this image data usually 255

    pixels = new unsigned char *[imageHeight];
    // read each pixel value as 1 byte char and cast it into the unsigned char value to store it in the pixels array
    char p;
    int counter = 0;
    for (int i = 0; i < imageHeight; ++i)
    {
        pixels[i] = new unsigned char[imageWidth];
        for (int j = 0; j < imageWidth; ++j)
        {
            counter++;
            file_reader.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
            //  std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }
    // std::cout << "Size: " << sizeof(pixels) << " " << sizeof(pixels[0]) << std::endl;
    bool image_read = (counter > 0 && counter == (imageWidth * imageHeight));
    return image_read;
}
