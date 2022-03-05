#include <time.h>

#include <random>
#include <iterator>
#include <algorithm>
#include <functional>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "elasticsearch/books/FB2/fb2_packer.hpp"
#include "elasticsearch/books/FB2/fb2_unpacker.hpp"
#include <gtest/gtest.h>

namespace tests
{
template<size_t length>
class PackUnpackFixture : public ::testing::Test
{
protected:
    std::unique_ptr<std::ofstream> tmp;
    //std::string file_data;
    std::vector<uint8_t> file_data;
    void SetUp() override {
        tmp.reset(new std::ofstream(file_name()));

        std::mt19937 rng;
        file_data.reserve(length);
        std::generate_n(std::back_inserter(file_data), length, std::ref(rng));
        tmp->write(reinterpret_cast<char*>(file_data.data()), file_data.size());
        tmp.reset();
    }

    const std::string& file_name() const
    {
        static const std::string name{"fb2_pack_unpack_suite.cpp"};
        return name;
    }
    const std::vector<uint8_t>& stencil_data() const
    {
        return file_data;
    }

    void TearDown() override {
        unlink(file_name().c_str());
    }
};

TEST(base_64, decoder)
{
    size_t length = 2048;
    std::vector<uint8_t> orig;
    orig.reserve(length);

    std::mt19937 rng;
    for (size_t i = 1; i < length; i++)
    {
        std::generate_n(std::back_inserter(orig), i, std::ref(rng));
        std::string encoded = elasticsearch::utils::base64_encode(orig);
        std::vector<uint8_t> decoded = elasticsearch::utils::base64_decode(encoded);
        ASSERT_EQ(decoded, orig);
        orig.clear();
    }
}

using PackUnpackFixture_25M = PackUnpackFixture<25*1024*1024>;
TEST_F(PackUnpackFixture_25M, success)
{
    fb2::packer p(file_name());
    std::shared_ptr<elasticsearch::common_model::BinaryBlob> b_p = p.getBlob();

    auto encoded = b_p->getEncodedValue();

    std::shared_ptr<elasticsearch::common_model::BinaryBlob> b_u =
                        std::make_shared<elasticsearch::common_model::BinaryBlob>(std::move(encoded));

    std::string u_file_name {"PackUnpackFixture_success"};
    fb2::unpacker u(*b_u, u_file_name);
    ASSERT_EQ(u.getFilePath(), u_file_name);

    std::fstream u_file(u_file_name);
    u_file.seekp(0, std::ios_base::end);
    std::vector<uint8_t> u_file_data(u_file.tellp(), 0);

    u_file.seekp(0, std::ios_base::beg);
    u_file.read(reinterpret_cast<char*>(u_file_data.data()), u_file_data.size());

    unlink(u_file_name.c_str());
    ASSERT_EQ(stencil_data().size(), u_file_data.size());
    ASSERT_EQ(memcmp(stencil_data().data(), u_file_data.data(), u_file_data.size()), 0);
}
}
