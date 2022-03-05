#include <stdexcept>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "elasticsearch/utils/base64.h"

namespace  elasticsearch
{
namespace utils
{
static const char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                      'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                      '4', '5', '6', '7', '8', '9', '+', '/'};
static const uint8_t decoding_table[256] = {
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
      52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
      64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
      64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};

static const size_t mod_table[] = {0, 2, 1};

std::string base64_encode(const std::vector<uint8_t> &in_data) {

    size_t ret_size = 4 * ((in_data.size() + 2) / 3);
    std::string ret(ret_size, '=');
    for (size_t i = 0, j = 0; i < in_data.size();) {

        uint32_t octet_a = i < in_data.size() ? in_data[i++] : 0;
        uint32_t octet_b = i < in_data.size() ? in_data[i++] : 0;
        uint32_t octet_c = i < in_data.size() ? in_data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) | (octet_b << 0x08) | octet_c;

        ret[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        ret[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        ret[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        ret[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[in_data.size() % 3]; i++)
    {
        ret[ret.size() - 1 - i] = '=';
    }
    return ret;
}


std::vector<uint8_t> base64_decode(const std::string &data)
{
    if (data.size() % 4 != 0)
    {
        throw std::runtime_error("invalid size of BASE64 data to decode: " + std::to_string(data.size()));
    }

    if (data.empty())
    {
        return {};
    }
    size_t ret_size = data.size() / 4 * 3;
    if (data[data.size() - 1] == '=')
    {
        ret_size--;
    }
    if (data[data.size()  - 2] == '=')
    {
        ret_size--;
    }

    std::vector<uint8_t> ret(ret_size, 0);
    for (size_t i = 0, j = 0; i < data.size();) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6) | (sextet_b << 2 * 6) | (sextet_c << 1 * 6) | (sextet_d << 0 * 6);

        if (j < ret_size) ret[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < ret_size) ret[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < ret_size) ret[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return ret;
}
}
}
