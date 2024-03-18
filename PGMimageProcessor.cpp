#include "PGMimageProcessor.h"

#include "PGMimageProcessor.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <queue>
#include <fstream>

#define PGMIP NDLMDU011::PGMimageProcessor

u_char seen = 0;        // Minimum valid pixel value for components
u_char white = 255;
u_char black = 0;

// Custom constructor
PGMIP::PGMimageProcessor(std::string fname) : filename(fname), imageHeight(0), imageWidth(0) {}

PGMIP::~PGMimageProcessor()
{
    // std::cout << "Destructor" << std::endl;

    for (int i = 0; i < imageHeight; ++i)
    {
        delete[] pixels[i];
    }
    delete[] pixels;
}

bool PGMIP::readPGMImage()
{
    // Read the PGM input image and save the data in the appropriate variables
    std::ifstream file_reader(filename, std::fstream::binary);

    if (!file_reader)
        return false;

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
    counter = 0;
    for (int i = 0; i < imageHeight; ++i)
    {
        pixels[i] = new unsigned char[imageWidth];
        for (int j = 0; j < imageWidth; ++j)
        {
            counter++;
            file_reader.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
            // std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }
    bool image_read = (counter > 0 && counter == (imageWidth * imageHeight));
    return image_read;
}

unsigned char **PGMIP::getPixelsArr() const
{
    unsigned char **arr = new unsigned char *[imageHeight];

    for (int i = 0; i < imageHeight; ++i)
    {
        arr[i] = new unsigned char[imageWidth];
        for (int j = 0; j < imageWidth; ++j)
        {
            arr[i][j] = pixels[i][j];
        }
    }

    return arr;
}

// Flood fill algorithm to determine the coordinates of pixels for the passed Connected Component
void PGMIP::floodFill(u_char **&pixels_arr, std::shared_ptr<ConnectedComponent> &cc, int y, int x, u_char source, u_char &seen, u_char &threshold)
{
    // Condition for checking out of bounds on pixels array then checks this next pixels to add is part of component
    if ((y < 0 || y >= imageHeight) || (x < 0) || (x >= imageWidth) || (pixels_arr[y][x] != source))
    {
        return;
    }

    //  Change the value of this pixel value to be identified as "seen" or processed
    pixels_arr[y][x] = seen;

    // From above this pixel is of a connected component, so add its coordinates with the mates
    cc->addCoords(x, y);
    // std::cout << " added coordinates to component" << std::endl;

    floodFill(pixels_arr, cc, y - 1, x, source, seen, threshold); // Visit the North pixel

    floodFill(pixels_arr, cc, y + 1, x, source, seen, threshold); // Visit the South pixel

    floodFill(pixels_arr, cc, y, x + 1, source, seen, threshold); // Visit the East pixel

    floodFill(pixels_arr, cc, y, x - 1, source, seen, threshold); // Visit the West pixel
}

/* process the input image to extract all the connected components, based on the supplied threshold (0...255)
 and excluding any components of less than the minValidSize. The final number of components that
you store in your container (after discarding undersized one) must be returned.
*/
int PGMIP::extractComponents(unsigned char threshold, int minValidSize)
{
    // std::cout << "Extracting Components from image file ..." << std::endl;

    int y = 0;
    int x = 0;

    std::queue<std::pair<int, int>> queue;

    // Loop through the pixels array
    while (y < imageHeight && x < imageWidth)
    {
        // Add to queue all coordinates of pixels that are above threshold value for detection
        if ((pixels[y][x]) >= threshold)
        {
            std::pair<int, int> first(x,y);
            queue.push(first);
        }

        x++;
        if (x == imageWidth)
        {
            x = 0;
            y++;
        }
    }

    // While the queue is not empty, visit the pixels and process them to find connected
    // components from the pixels and their neighbours
    while (queue.size() > 0)
    {
        // Dequeue the last pixel coordinate added to the queue and visit the pixel if not visited already
        std::pair<int, int> coordinates = queue.front();
        queue.pop();

        // pixel value for this popped pixel in the queue
        unsigned char source = pixels[coordinates.second][coordinates.first];

        // If the pixel value is above mininmum valid pixel value (or not been processed) use flood fill algorithm to get
        // its pixel neighbours thus identifying a component and adding it to the Container
        if (source > seen)
        {
            // std::cout << "(" << coordinates.second << ", " << coordinates.first << ")  pixel: " << (0 + pixels[coordinates.second][coordinates.first]);
            // std::cout << " queue size: " << queue.size() << std::endl;
            std::shared_ptr<ConnectedComponent> cc(new ConnectedComponent(0, CCcontainer.size()));
            cc->pixel_value = pixels[coordinates.second][coordinates.first];

            floodFill(pixels, cc, coordinates.second, coordinates.first, source, seen, threshold);

            if (cc->getNumPixels() >= minValidSize)
            {
                CCcontainer.push_back(cc);
            }
        }
    }
    // return the number of connected components identified
    return CCcontainer.size();
}

/** iterate - with an iterator - though your container of connected components and filter out (remove) all the components
which do not obey the size criteria pass as arguments. The number remaining after this operation should be returned.
*/
int PGMIP::filterComponentsBySize(int minSize, int maxSize)
{
    using namespace std;
    vector<shared_ptr<ConnectedComponent>>::iterator it = CCcontainer.begin();
    while (it < CCcontainer.end())
    {
        int numPixels = it->get()->getNumPixels();
        if (numPixels < minSize || numPixels > maxSize)
        {
            CCcontainer.erase(it);
            --it; // go back to the last possible index as this element is erased
        }
        ++it; // move to the next element in the container
    }
    return CCcontainer.size();
}

// Return number of connected components identified
int PGMIP::getComponentCount(void) const
{
    return CCcontainer.size();
}

// return number of pixels in largest component
int PGMIP::getLargestSize(void) const
{
    int largest = 0;
    for (int k = 0; k < CCcontainer.size(); ++k)
    {
        int num = CCcontainer[k]->getNumPixels();

        if (num > largest)
        {
            largest = num;
        }
    }
    return largest;
}

// return number of pixels in smallest component
int PGMIP::getSmallestSize(void) const
{
    int smallest = 10000;
    for (int k = 0; k < CCcontainer.size(); ++k)
    {
        int num = CCcontainer[k]->getNumPixels();

        if (num < smallest)
        {
            smallest = num;
        }
    }
    return smallest;
}

/// @brief iterates through the components container to put the maximum value 255 in the position
/// or coordinates of pixels in the component
/// @return true successful added connected components data to the array as value 255
bool PGMIP::setComponentsToArray()
{
    for (int k = 0; k < CCcontainer.size(); ++k)
    {
        int compSize = CCcontainer[k]->getNumPixels();

        for (int l = 0; l < compSize; ++l)
        {
            int x = CCcontainer[k]->getX(l);
            int y = CCcontainer[k]->getY(l);

            pixels[y][x] = white;
            // std::cout << "("  << y << "," << x << ")" << std::endl;
        }
    }
    return true;
}

/* create a new PGM file which contains all current components (255=component pixel, 0 otherwise)
and write this to outFileName as a valid PGM. the return value indicates success of operation
*/
bool PGMIP::writeComponents(const std::string &outFileName)
{
    std::cout << "Writing to file " << std::endl;
    std::ofstream outfile(outFileName, std::ofstream::binary);

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << imageWidth << " " << imageHeight << std::endl;
    outfile << 255 << std::endl;

    // add components data to array
    if (!setComponentsToArray())
        return false;

    for (int i = 0; i < imageHeight; ++i)
    {
        for (int j = 0; j < imageWidth; ++j)
        {
            if (pixels[i][j] != white)
            {
                pixels[i][j] = black;
            }

            outfile.write((char *)&pixels[i][j], 1);
            // std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }

    return true;
}

// Prints all the connected components in the container and their data or connected pixel coordinates
void PGMIP::printAll() const
{
    for (auto i : CCcontainer)
    {
        std::cout << "Pixel value: " << (0 + i->pixel_value) << std::endl;
        printComponentData(*i);
        std::cout << std::endl;
    }
}

/* print the data for a component to std::cout
print out to std::cout: component ID, number of pixels
*/
void PGMIP::printComponentData(const ConnectedComponent &theComponent) const
{
    int length = theComponent.getNumPixels();
    std::cout << "Component ID: " << theComponent.getID() << ", Number of Pixels: " << length << std::endl;

    /*for (int i = 0; i < length; ++i)
    {
        std::cout << "(y , x) = (" << theComponent.getY(i) << "," << theComponent.getX(i) << ")" << std::endl;
    }*/
}

void NDLMDU011::clearArray(unsigned char **arr, int height)
{
    for (int i = 0; i < height; ++i)
    {
        delete[] arr[i];
    }
    delete[] arr;
}