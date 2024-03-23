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
/*
    Block 1: 220 pixel value = 10 pixels
    Block 2: 200 pixel value = 7 pixels
    Block 3: 100 pixel value = 2 pixels
    Block 4: 100 pixel value = 2 pixels
    Block 5: 120 pixel value = 2 pixels
    Block 6: 120 pixel value = 2 pixels
    Block 7: 150 pixel value = 1 pixel
    Block 8: 150 pixel value = 1 pixel
    Block 9: 50 pixel value = 1 pixel
    Blocks: 003 pixel value = 1 pixel * 3
    Blocks: 001 pixel value = 1 pixel * 2
    Not component: 0 pixel value = 1 pixel * 7
    Total Components = 19
    Total pixels = 40
*/

bool writeBlocks(void)
{
    std::ofstream outfile(blocks_name, std::ofstream::binary);

    int width = 8;
    int height = 5;

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << 255 << std::endl;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            outfile.write((char *)&blocks_pixel[i][j], 1);
        }
    }
    return true;
}

TEST_CASE("TEST WRITING TEST IMAGE")
{
    REQUIRE(writeBlocks() == true);
}

TEST_CASE("TESTING INITIALISE METHOD")
{
    PGMimageProcessor<unsigned char> imageProcessor;

    unsigned char **test = nullptr;
    imageProcessor.initialiseArray(test, 200, 250);

    REQUIRE(test != nullptr);

    int count = 0;
    bool allseen = true;
    for (int i = 0; i < 200; ++i)
    {
        for (int j = 0; j < 250; ++j)
        {
            if (test[i][j] != imageProcessor.seen)
                allseen = false;
        }
    }
    REQUIRE(allseen);
    imageProcessor.clearArray(test, height);
    REQUIRE(test == nullptr);
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
}
TEST_CASE("ISVALID METHO")
{
    PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
    REQUIRE(imageProcessor.readPGMImage() == true);

    unsigned char **test_arr = imageProcessor.getPixelsArr();
    REQUIRE(test_arr[4][7] == 120);

    unsigned char seen = 0;

    imageProcessor.clearArray(test_arr, height);
}

TEST_CASE("FLOODFILL_METHOD")
{
    PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
    REQUIRE(imageProcessor.readPGMImage() == true);

    unsigned char **test_arr = imageProcessor.getPixelsArr();
    REQUIRE(test_arr[4][7] == 120);

    unsigned char seen = 0;

    SECTION("Connected pixels for 220 pixel value")
    {
        int source = 220;
        int x = 3, y = 2;

        std::shared_ptr<ConnectedComponent> cc1(new ConnectedComponent(0, 0));
        imageProcessor.floodFillLooping(test_arr, cc1, width, height, x, y, source, seen);
        REQUIRE(cc1->getNumPixels() == 10);
        REQUIRE(cc1.use_count() == 1);
        REQUIRE((*cc1).getID() == 0);
    }

    SECTION("Connected pixels for 200 pixel value")
    {
        unsigned char source = 200;
        int x = 0;
        int y = 1;
        std::shared_ptr<ConnectedComponent> cc2(new ConnectedComponent(0, 1));
        imageProcessor.floodFillLooping(test_arr, cc2, width, height, x, y, source, seen);

        REQUIRE(cc2->getNumPixels() == 7);
        REQUIRE(cc2.use_count() == 1);
        REQUIRE(cc2->getID() == 1);
        REQUIRE(cc2->getX(0) == x);
        REQUIRE(cc2->getY(0) == y);
        REQUIRE_THROWS(cc2->getX(7));
        REQUIRE_THROWS(cc2->getY(7));

        SECTION("Retest extracting Connected pixels for 200 pixel value")
        {
            unsigned char source = 200;
            int x = 0;
            int y = 1;
            std::shared_ptr<ConnectedComponent> cc2(new ConnectedComponent(0, 1));
            imageProcessor.floodFillLooping(test_arr, cc2, width, height, x, y, source, seen);

            // The Connected component should be empty as the component has already been extracted (pixel values overwritten to seen pixel value)
            REQUIRE(cc2->getNumPixels() == 0);
            REQUIRE(cc2.use_count() == 1);
            REQUIRE(cc2->getID() == 1);
            REQUIRE_THROWS(cc2->getX(0) == 0);
            REQUIRE_THROWS(cc2->getY(0) == 0);
            REQUIRE_THROWS(cc2->getX(0));
            REQUIRE_THROWS(cc2->getY(0));
        }
    }

    imageProcessor.clearArray(test_arr, height);
    REQUIRE(test_arr == nullptr);
}

TEST_CASE("FILTERING COMPONENTS BY SIZE")
{
    PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
    REQUIRE(imageProcessor.readPGMImage() == true);
    unsigned char threshold = 0; // smallest threshold
    int validSize = 1;

    int num_comps = imageProcessor.extractComponents(threshold, validSize);
    REQUIRE(num_comps == 14);
    REQUIRE(imageProcessor.getSmallestSize() == 1);
    REQUIRE(imageProcessor.getLargestSize() == 10);
    REQUIRE(imageProcessor.getComponentCount() == num_comps);

    SECTION("FILTER BY MIN_SIZE 2 AND MAX SIZE 10")
    {
        imageProcessor.filterComponentsBySize(2, 10);

        REQUIRE(imageProcessor.getSmallestSize() >= 2);
        REQUIRE(imageProcessor.getLargestSize() <= 10);
        REQUIRE(imageProcessor.getComponentCount() == 6);
    }

    SECTION("FILTER BY MIN_SIZE 2 AND MAX SIZE 9")
    {
        imageProcessor.filterComponentsBySize(2, 9);

        REQUIRE(imageProcessor.getSmallestSize() >= 2);
        REQUIRE(imageProcessor.getLargestSize() <= 9);
        REQUIRE(imageProcessor.getComponentCount() == 5);
    }

    SECTION("FILTER BY MIN_SIZE 7 AND MAX SIZE 7")
    {
        imageProcessor.filterComponentsBySize(7, 7);

        REQUIRE(imageProcessor.getSmallestSize() == 7);
        REQUIRE(imageProcessor.getLargestSize() == 7);
        REQUIRE(imageProcessor.getComponentCount() == 1);
    }
}

TEST_CASE("TESTING SETCOMPONENTS METHOD")
{
    PGMimageProcessor<unsigned char> imageProcessor(blocks_name);
    REQUIRE(imageProcessor.readPGMImage() == true);
    unsigned char threshold = 0; // smallest threshold
    int validSize = 1;

    imageProcessor.extractComponents(threshold, validSize);
    imageProcessor.setComponentsToArray();

    unsigned char **test = imageProcessor.getPixelsArr();
    bool blackOrwhite = true;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (test[i][j] != 255 && test[i][j] != 0)
                blackOrwhite = false;

    REQUIRE(blackOrwhite);
}