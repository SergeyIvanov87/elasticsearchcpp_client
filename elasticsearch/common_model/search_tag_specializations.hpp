#ifndef COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP
#define COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP

#include <iterator>
#include <sstream>

#include <nlohmann/json.hpp> //TODO

#include "elasticsearch/common_model/Tag.h"

namespace model
{
namespace must {

class TagsArrayElement : public txml::XMLNodeLeaf<TagsArrayElement,
                                                  std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<TagsArrayElement,
                                     std::string>;

    using base_t::base_t;

    static constexpr std::string_view class_name()
    {
        return "";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
   //<---- Doesn't serialized!!!!! in serializer class list!!!!

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

#if 0
template<class SpecificModel>
class ElementToQuery<SpecificModel, elasticsearch::common_model::Tags> :
     public txml::XMLArray<ElementToQuery<SpecificModel, elasticsearch::common_model::Tags>,
                           TagsArrayElement>
{
public:
    using Model = SpecificModel;
    using Element = TagsArrayElement;
    using base_t = txml::XMLArray<ElementToQuery<Model, elasticsearch::common_model::Tags>, TagsArrayElement>;

    static constexpr std::string_view class_name()
    {
        return Model::template get_path<elasticsearch::common_model::Tags>();
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    template<class Formatter, class Tracer>
    void format_serialize_impl(Formatter& out, Tracer tracer) const
    {
        nlohmann::json cur_json_element = nlohmann::json::array();
        auto mediator = out.get_shared_mediator_object();

        for (auto &el : base_t::getValue())
        {
            cur_json_element.push_back(el->getValue());
        }
        mediator->push({{class_name(), std::move(cur_json_element)}});
    }

    ElementToQuery(const elasticsearch::common_model::Tags::value_t& value)
    {
        for (auto && el : value)
        {
            base_t::getValue().push_back(std::make_shared<TagsArrayElement>(el));
        }
    }
};
#endif
}
}
#endif // COMMON_MODEL_SEARCH_TAG_SPECIALIZATIONS_HPP
