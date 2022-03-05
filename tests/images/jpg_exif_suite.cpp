#include <time.h>

#include <random>
#include <iterator>
#include <algorithm>
#include <functional>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>


#include "elasticsearch/images/JPG/reader.hpp"
#include "elasticsearch/utils/file_to_base64_packer.hpp"

#include <gtest/gtest.h>

namespace tests
{
TEST(jpg_exif, read)
{

    JPG::reader r("/home/user/work/pet_projects/ES/BookIndexer/img2.JPG");
    JPG::reader s("/home/user/work/pet_projects/ES/BookIndexer/img.JPG");
    JPG::reader q("/home/user/work/pet_projects/ES/BookIndexer/mus.mp3");
}
}
