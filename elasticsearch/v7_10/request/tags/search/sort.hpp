#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SORT_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SORT_HPP

#include <txml/applications/json/json.hpp>

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;


template<class Model, class ...SortParams>
struct sort
{
    sort(std::initializer_list<::model::Order> orders)
    {
        using sorted_array_type = ::model::SortArray<Model, SortParams...>;
        using sorted_array_value_type = ::model::SortArrayElement<Model, SortParams...>;
        auto elem = std::make_shared<sorted_array_value_type>();

        // helper
        auto append = [order_it = orders.begin()](auto sorted_param) mutable{
            sorted_param->template emplace<::model::Order>(*order_it);
        };

        (append(elem->template emplace<::model::SortRecord<Model, SortParams>>()), ...);
        sorted_array_type array({elem});
        instance.template emplace<sorted_array_type>(std::move(array));
    }
    sort_subrequest<Model, SortParams...> instance;
};

namespace create
{
    template<class Model, class ...SortParams>
    static sort<Model, SortParams...> sort_tag(std::initializer_list<::model::Order> orders)
    {
        return sort<Model, SortParams...> (orders);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_SORT_HPP
