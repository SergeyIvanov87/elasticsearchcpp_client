#ifndef ELASTICSEARCH_UTILS_BASE64_H
#define ELASTICSEARCH_UTILS_BASE64_H

#include <string>
#include <vector>

namespace  elasticsearch
{
namespace utils
{
std::string base64_encode(const std::vector<uint8_t> &in_data);
std::vector<uint8_t> base64_decode(const std::string &data);
}
}

#endif // ELASTICSEARCH_UTILS_BASE64_H
