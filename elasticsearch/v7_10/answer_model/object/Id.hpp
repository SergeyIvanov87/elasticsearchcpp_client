#ifndef ANSWER_MODEL_OBJECT_ID_HPP
#define ANSWER_MODEL_OBJECT_ID_HPP

#include <txml/txml.hpp>

#include "elasticsearch/v7_10/answer_model/object/Id.h"

namespace model
{
inline Id::Id(std::string&& str) :
    base_t(std::move(str))
{
}
}
#endif // ANSWER_MODEL_OBJECT_ID_HPP
