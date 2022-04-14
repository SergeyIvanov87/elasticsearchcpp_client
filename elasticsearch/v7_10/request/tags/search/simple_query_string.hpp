#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP

#include "elasticsearch/v7_10/answer_model/search/object/full_text/QuerySimpleString.hpp"

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
using simple_query_string = ::model::search::full_text::SimpleQueryString<Model, SpecificModelParams...>;

namespace create
{
    template<class Model, class ...SpecificModelParams>
    simple_query_string<Model, SpecificModelParams...> simple_query_string_tag(const std::string &query_string)
    {
        return simple_query_string<Model, SpecificModelParams...> {query_string};
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SIMPLE_QUERY_STRING_HPP
