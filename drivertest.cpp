#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <vector>
#include <memory>

using namespace NDLMDU011;
std::string filename = "chess-thresh-separate.pgm";
std::string blocks_name = "blocks.pgm";
int image_height = 252;
int image_width = 609;
int width = 8, height = 5;
int num_pixels = 252 * 609;

unsigned char blocks_pixel[5][8] =
    {
        {000, 150, 000, 100, 100, 000, 120, 120},
        {200, 200, 200, 003, 220, 220, 220, 001},
        {000, 200, 003, 220, 220, 050, 220, 220},
        {200, 200, 200, 003, 220, 220, 220, 001},
        {000, 150, 000, 100, 100, 000, 120, 120}};

bool writeBlocks(void)
{
    std::ofstream outfile(blocks_name, std::ofstream::binary);

    int width = 8;
    int height = 5;
    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << 255 << std::endl;

    // add components data to array

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            outfile.write((char *)&blocks_pixel[i][j], 1);
            // std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }
    return true;
}

/*unsigned char** copyToMemory(){
    unsigned char ** arr= new unsigned char *[5];
    for (int i = 0; i < )
}*/

TEST_CASE("TEST WRITING TEST IMAGE")
{
    REQUIRE(writeBlocks() == true);
}

TEST_CASE("TESTING READ IMAGE METHOD")
{
    PGMimageProcessor<unsigned char> imageProcessor(filename);
    bool is_read = imageProcessor.readPGMImage();

    REQUIRE(is_read == true);
    REQUIRE(imageProcessor.counter == num_pixels);
}

TEST_CASE("EXTRACTING COMPONENTS")
{
    /*
     Block 1: 220 pixel value = 10 pixels
     Block 2: 200 pixel value = 7 pixels
     Block 3: 100 pixel value = 2 pixels
     Block 4: 100 pixel value = 2 pixels
     Block 5: 120 pixel value = 2 pixels
     Block 6: 120 pixel value = 2 pixels
     Block 7: 150 pixel value = 1 pixel
     Block 8: 150 pixel value = 1 pixel
 */

    SECTION("EXTRACT_METHOD_0")
    {
        PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
        REQUIRE(imageProcessor.readPGMImage() == true);
        unsigned char threshold = 100; // smallest threshold
        int validSize = 1;

        int num_comps = imageProcessor.extractComponents(threshold, validSize);

        SECTION("TEST_LARGEST")
        {
            REQUIRE(imageProcessor.getLargestSize() == 10);
        }
        SECTION("TEST_SMALLEST")
        {
            REQUIRE(imageProcessor.getSmallestSize() == 1);
        }
        SECTION("TEST_COMPONENT_COUNT")
        {
            REQUIRE(imageProcessor.getComponentCount() == 8);
        }
        imageProcessor.writeComponents("outImage.pgm");
    }

    /*
      Pawn = red = 4349		        pixel = 130
      Castle = green = 6553		    pixel = 220
      Knight = blue = 8028		    pixel = 70
      Bishop = magenta = 5793		pixel = 145
      Queen = yellow = 7137		    pixel = 248
      King = cyan = 7640		    pixel = 228
  */
    int const size = 6;
    int arr_counts[size] = {4349, 6553, 8028, 5793, 7137, 7640};
    int arr_pixels[size] = {130, 220, 70, 145, 248, 228};

    SECTION("EXTRACT_METHOD_2")
    {
        PGMimageProcessor<unsigned char> imageProcessor(filename);
        imageProcessor.readPGMImage();
        unsigned char threshold = 70; // smallest threshold
        int validSize = 1;

        int num_comps = imageProcessor.extractComponents(threshold, validSize);

        SECTION("TEST_LARGEST")
        {
            REQUIRE(imageProcessor.getLargestSize() == arr_counts[2]);
        }
        SECTION("TEST_SMALLEST")
        {
            REQUIRE(imageProcessor.getSmallestSize() == arr_counts[0]);
        }
        SECTION("TEST_COMPONENT_COUNT")
        {
            REQUIRE(imageProcessor.getComponentCount() == size);
        }
    }

    SECTION("FLOODFILL_METHOD")
    {
        PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
        REQUIRE(imageProcessor.readPGMImage() == true);

        unsigned char **test_arr = imageProcessor.getPixelsArr();

        unsigned char threshold = 100; // smallest threshold
        int validSize = 1;

        // int num_comps = imageProcessor.extractComponents(threshold, validSize);
        // REQUIRE(num_comps == 8);

        int source = 220;
        int x = 3, y = 2;
        unsigned char seen = 0;

        std::shared_ptr<ConnectedComponent> cc1(new ConnectedComponent(0, 0));
        imageProcessor.floodFillLooping(test_arr, cc1, width, height, x, y, source, seen);
        REQUIRE(cc1->getNumPixels() == 10);
        REQUIRE(cc1.use_count() == 1);
        REQUIRE((*cc1).getID() == 0);

        source = 200;
        x = 0;
        y = 1;
        std::shared_ptr<ConnectedComponent> cc2(new ConnectedComponent(0, 1));
        imageProcessor.floodFillLooping(test_arr, cc2,width,height, x, y, source, seen);
        REQUIRE(cc2->getNumPixels() == 7);
        REQUIRE(cc2.use_count() == 1);
        REQUIRE(cc2->getID() == 1); 
        REQUIRE(cc2->getX(0) == x);
        REQUIRE(cc2->getY(0) == y);
        REQUIRE_THROWS(cc2->getX(7));
        REQUIRE_THROWS(cc2->getY(7));

        // PGMimageProcessor imageProcessor2(blocks_name);

        for (int i = 0; i < height; ++i)
        {
            delete[] test_arr[i];
        }
        delete[] test_arr;
    }
}