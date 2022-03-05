#ifndef ANSWER_MODEL_OBJECT_ERROR_H
#define ANSWER_MODEL_OBJECT_ERROR_H

#include <txml/txml_fwd.h>
#include "elasticsearch/v7_10/answer_model/object/RootCause.h"
#include "elasticsearch/v7_10/answer_model/object/Type.h"
#include "elasticsearch/v7_10/answer_model/object/Reason.h"

namespace model
{
template<class ...ErrorExt>
class Error: public txml::XMLNode<Error<ErrorExt...>, Type, Reason, ErrorExt...>
{
public:
    using base_t = txml::XMLNode<Error<ErrorExt...>, Type, Reason, ErrorExt...>;

    static constexpr std::string_view class_name()
    {
        return "error";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Element;
    }
};
}
#endif // ANSWER_MODEL_OBJECT_ERROR_H
