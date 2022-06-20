#ifndef ELASTICSEARCH_IMAGES_SEARCH_IMAGE_SPECIALIZATIONS_HPP
#define ELASTICSEARCH_IMAGES_SEARCH_IMAGE_SPECIALIZATIONS_HPP

#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/images/request/search_image_tags.hpp"
#include "elasticsearch/common_model/search_tag_specializations.hpp"

namespace model
{
namespace search {
namespace must {
template<>
class ElementToQuery<elasticsearch::image::model::data,
                     elasticsearch::image::model::element::Location> :
     public txml::XMLNodeLeaf<ElementToQuery<elasticsearch::image::model::data,
                     elasticsearch::image::model::element::Location>, elasticsearch::image::model::element::Location::value_t>
{
public:
    using Model = elasticsearch::image::model::data;
    using Element = elasticsearch::image::model::element::Location;
    using base_t = txml::XMLNodeLeaf<ElementToQuery<Model, Element>, typename Element::value_t>;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<Element>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    ElementToQuery(const typename Element::value_t& value) :
        base_t(value)
    {
    }

    std::string value() const
    {
        return base_t::value().to_string();
    }
};
}
}
}
#endif // ELASTICSEARCH_IMAGES_SEARCH_IMAGE_SPECIALIZATIONS_HPP
