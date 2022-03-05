#ifndef ELASTICSEARCH_AUX_DOC_INDEX_MODEL_HPP
#define ELASTICSEARCH_AUX_DOC_INDEX_MODEL_HPP

#include <txml/txml_fwd.h>

namespace elasticsearch
{
namespace aux
{
namespace model
{
namespace element
{
class Unused: public txml::XMLNodeLeaf<Unused, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<Unused, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr std::string_view class_name()
    {
        return "unused";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }

    Unused(value_t&& value) :
        base_t(std::move(value))
    {
    }

    Unused(const value_t& value) :
        base_t(value)
    {
    }
};
}
#define AUX_DATA_MODEL_ELEMENTS    elasticsearch::aux::model::data,                                \
                                   elasticsearch::aux::model::element::Unused

struct data : public txml::XMLNode<AUX_DATA_MODEL_ELEMENTS>
{
    using base = txml::XMLNode<AUX_DATA_MODEL_ELEMENTS>;

    static constexpr std::string_view class_name()
    {
        return "doc_id_gen";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
    data() {
        this->template emplace<element::Unused> ("");
    }
};
}
}
}
#endif // ELASTICSEARCH_AUX_DOC_INDEX_MODEL_HPP
