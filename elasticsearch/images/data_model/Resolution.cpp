#include "elasticsearch/images/data_model/Resolution.h"
namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
resolution::resolution(value_t w, value_t h) :
    width(w), height(h)
{
}
resolution::resolution(const std::string& from_string)
{
    auto comma_pos = from_string.find('x');
    if (comma_pos == std::string::npos or comma_pos == from_string.size())
    {
        throw std::runtime_error("invalid resolution format: 'x' must separate W & H");
    }
    width = std::stof(from_string.substr(0, comma_pos), nullptr);
    height = std::stof(from_string.substr(comma_pos + 1, from_string.size() - comma_pos - 1), nullptr);
}

resolution::value_t resolution::get_width() const
{
    return width;
}

resolution::value_t resolution::get_height() const
{
    return height;
}

std::string resolution::to_string() const
{
    return std::to_string(width) + "x" + std::to_string(height);
}
}
}
}
}
