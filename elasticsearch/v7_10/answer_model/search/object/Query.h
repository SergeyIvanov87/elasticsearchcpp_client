#ifndef ANSWER_MODEL_SEARCH_OBJECT_QUERY_H
#define ANSWER_MODEL_SEARCH_OBJECT_QUERY_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/search/object/MatchAll.h"
#include "elasticsearch/v7_10/answer_model/search/object/boolean/Bool.h"

namespace model
{
class EmptyModel {};
struct EmptyParam {using value_type = EmptyParam; };

template<class Model, class ...Params>
class Query: public txml::XMLNode<Query<Model, Params...>,
                                  Params...>
{
public:
    using base_t = txml::XMLNode<Query<Model, Params...>,
                                 Params...>;

    static constexpr std::string_view class_name()
    {
        return "query";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};

template<class Model>
class Query<Model, EmptyParam>: public txml::XMLNode<Query<Model, EmptyParam>,
                                                          MatchAll>
{
public:
    using base_t = txml::XMLNode<Query<Model, EmptyParam>,
                                       MatchAll>;

    static constexpr std::string_view class_name()
    {
        return "query";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
    Query() {
        this->template emplace<MatchAll>();
    }
};
}
#endif // ANSWER_MODEL_SEARCH_OBJECT_QUERY_H
