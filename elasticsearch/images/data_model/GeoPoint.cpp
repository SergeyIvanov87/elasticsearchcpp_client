#include <exception>

#include "elasticsearch/images/data_model/GeoPoint.h"


namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
geopoint::geopoint(value_t lat, value_t lon, value_t alt) :
    latitude(lat), longitude(lon), altitude(alt)
{
}

geopoint::geopoint(const std::string& from_string)
{
    auto comma_pos = from_string.find(',');
    if (comma_pos == std::string::npos or comma_pos == from_string.size())
    {
        throw std::runtime_error("invalid geopoint format: comma must separate lat & lon");
    }
    latitude = std::stof(from_string.substr(0, comma_pos), nullptr);
    longitude = std::stof(from_string.substr(comma_pos + 1, from_string.size() - comma_pos - 1), nullptr);
}

geopoint::value_t geopoint::get_lat() const
{
    return latitude;
}

geopoint::value_t geopoint::get_lon() const
{
    return longitude;
}

geopoint::value_t geopoint::get_alt() const
{
    return altitude;
}

std::string geopoint::to_string() const
{
    return std::to_string(latitude) + "," + std::to_string(longitude);
}
}
}
}
}
