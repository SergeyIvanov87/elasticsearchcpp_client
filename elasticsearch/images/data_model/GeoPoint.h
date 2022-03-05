#ifndef ES_IMAGES_MODEL_GEOPOINT_H
#define ES_IMAGES_MODEL_GEOPOINT_H

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace image
{
namespace model
{
namespace element
{
class geopoint {
public:
    using value_t = float;
    geopoint(value_t lat, value_t lon, value_t alt);
    geopoint(const std::string& from_string);
    value_t get_lat() const;
    value_t get_lon() const;
    value_t get_alt() const;

    std::string to_string() const;
private:
    value_t latitude;
    value_t longitude;
    value_t altitude;
};

class Location: public txml::XMLNodeLeaf<Location, geopoint>
{
public:
    using base_t = txml::XMLNodeLeaf<Location, geopoint>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "location";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Location(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Location(geopoint::value_t lat, geopoint::value_t lon, geopoint::value_t alt) :
        Location(value_t{lat, lon, alt})
    {
    }

    Location(const value_t& value) :
        base_t(value)
    {
    }
};
}
}
}
}
#endif // ES_IMAGES_MODEL_GEOPOINT_H
