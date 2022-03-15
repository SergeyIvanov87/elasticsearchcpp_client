#ifndef COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP
#define COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP

#include <iterator>
#include <sstream>

#include <nlohmann/json.hpp> //TODO

#include "elasticsearch/common_model/Tag.h"

namespace model
{
namespace must {

template<class SpecificModel>
class ElementToQuery<SpecificModel,
                     elasticsearch::common_model::Tags> :
     public txml::XMLNodeLeaf<ElementToQuery<SpecificModel,
                     elasticsearch::common_model::Tags>, elasticsearch::common_model::Tags::value_t>
{
public:
    using Model = SpecificModel;
    using Element = elasticsearch::common_model::Tags;
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

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        nlohmann::json cur_json_element = nlohmann::json::array();
        auto mediator = out.get_shared_mediator_object();

        for (auto &el : base_t::getValue())
        {
            cur_json_element.push_back(el);
        }
        mediator->push({{class_name(), std::move(cur_json_element)}});
    }

};
}
}
#endif // COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP
