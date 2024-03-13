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

// Flood fill algorithm to determine the coordinates of pixels for the passed Connected Component
void PGMIP::floodFill(std::shared_ptr<ConnectedComponent> cc, int y, int x, int source, int minValidSize, int threshold)
{
    // Condition for checking out of bounds on pixels array
    if (y < 0 || y >= imageWidth || x < 0 || x >= imageWidth)
        return;

    int pix = pixels[y][x];
    // std::cout << " Value: " << pix << std::endl;

    // If this pixel has already been visited or is not part of this connected component return back
    if (pix <= minValidSize || pix != source)
    {
        // std::cout << "Color/ minSizeValue " << color << std::endl;
        return;
    }

    // Change the value of this pixel to the minimum valid pixel value identifying it as "seen"
    pixels[y][x] = minValidSize;
    // From above this pixel is of a connected component, so add its coordinates with the mates
    cc->addCoords(x, y);

    floodFill(cc, y - 1, x, source, minValidSize, threshold); // Visit the North pixel

    floodFill(cc, y + 1, x, source, minValidSize, threshold); // Visit the South pixel

    floodFill(cc, y, x + 1, source, minValidSize, threshold); // Visit the East pixel

    floodFill(cc, y, x - 1, source, minValidSize, threshold); // Visit the West pixel
}

/* process the input image to extract all the connected components,
based on the supplied threshold (0...255) and excluding any components
of less than the minValidSize. The final number of components that
you store in your container (after discarding undersized one)
must be returned.
*/

int PGMIP::extractComponents(unsigned char threshold, int minValidSize)
{
    std::cout << "Extracting Components from image file ..." << std::endl;

    int k = 0;
    int l = 0;

    std::vector<std::pair<int, int>> queue;

    // Loop through the pixels array
    while (k < imageHeight && l < imageWidth)
    {
        // std::cout << "k: " << k << " l: " << l << " pixel: " << (0 + pixels[k][l]) <<  std::endl;
        // Add to queue all coordinates of pixels that are above threshold value for detection
        if ((pixels[k][l]) >= threshold)
        {
            std::pair<int, int> first(l, k);
            first.first = l;
            first.second = k;
            queue.push_back(first);
        }

        l++;
        if (l == imageWidth)
        {
            l = 0;
            k++;
        }
    }

    // std::cout << "k: " << k << " l: " << l << std::endl;

    // create the processing queue and add the first pixel to process to the queue

    // While the queue is not empty, visit the pixels and process them to find connected
    // components from the pixels and their neighbours
    while (queue.size() > 0)
    {
        // Dequeue the last pixel coordinate addeed to the queue and visit the pixel if not visited already
        std::pair<int, int> coordinates = queue.back();
        // std::cout << "first: " << component.first << " second: " << component.second << " pixel: " << (0 + pixels[component.second][component.first]) << std::endl;
        queue.pop_back();
        std::shared_ptr<ConnectedComponent> cc(new ConnectedComponent(0, CCcontainer.size()));

        // integer pixel value for this popped pixel in the queue
        int source = 0 + pixels[coordinates.second][coordinates.first];
        // std::cout << "Source : " << source << std::endl;

        // If the pixel value is above mininmum valid pixel value (or not been processed) use flood fill algorithm to get
        // its pixel neighbours thus identifying a component and adding it to the Container
        if (source > minValidSize)
        {
            floodFill(cc, coordinates.second, coordinates.first, source, minValidSize, threshold);

            std::pair<std::shared_ptr<ConnectedComponent>, int> element(cc, source);
            CCcontainer.push_back(element);
        }
    }
    // return the number of connected components identified
    return CCcontainer.size();
}

/* iterate - with an iterator - though your container of connected
components and filter out (remove) all the components which do not
obey the size criteria pass as arguments. The number remaining
after this operation should be returned.
*/
int PGMIP::filterComponentsBySize(int minSize, int maxSize)
{
    std::cout << "Filtering Components by minimum size: " << minSize << " and maximum size: " << maxSize << std::endl;
    using namespace std;
    vector<pair<shared_ptr<ConnectedComponent>,
                unsigned char>>::iterator it = CCcontainer.begin();
    while (it < CCcontainer.end())
    {
        int numPixels = it->first->getNumPixels();
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
        int num = CCcontainer[k].first->getNumPixels();

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
        int num = CCcontainer[k].first->getNumPixels();

        if (num < smallest)
        {
            smallest = num;
        }
    }
    return smallest;
}

// Prints all the connected components in the container and their data or connected pixel coordinates
void PGMIP::printAll() const
{
    for (auto i : CCcontainer)
    {
        std::cout << "Pixel value: " << (0 + i.second) << std::endl;
        printComponentData(*i.first);
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
