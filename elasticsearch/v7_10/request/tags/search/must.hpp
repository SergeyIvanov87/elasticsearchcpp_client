#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP

#include <optional>

#include <txml/applications/json/json.hpp>
#include "elasticsearch/v7_10/answer_model/search/object/boolean/new_Bool.h"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;

template<class Model, class ...SpecificModelParams>
using must = ::model::search::MustNew<Model, SpecificModelParams...>;

namespace create
{
    template<class Model, class ...SpecificModelParams>
    must<Model, SpecificModelParams...> must_tag(SpecificModelParams&&...args)
    {
        return must<Model, SpecificModelParams...> (std::forward<SpecificModelParams>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
