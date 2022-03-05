#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP

#include "elasticsearch/v7_10/answer_model/search/object/boolean/serializer.hpp"

namespace elasticsearch
{
namespace v7
{
namespace search
{
namespace tag
{
using namespace elasticsearch::v7::search;
// the same as must
template<class Model, class ...SpecificModelParams>
struct filter {
    using value_type = ::model::Filter<Model, SpecificModelParams...>;
    using filter_array_type_value_type = typename value_type::element_t;
    using serializer_type = QueryFilterContextToJSON<Model, SpecificModelParams...>;

    template<class Parent>
    using serializer_parted_type = QueryFilterContextToJSONParted<Parent, Model, SpecificModelParams...>;

    filter(typename SpecificModelParams::value_t &&...args)
    {
        auto elem = std::make_shared<filter_array_type_value_type>();
        (elem->template emplace<::model::filter::Term<Model, SpecificModelParams>>()->template emplace<::model::filter::ElementToQuery<Model, SpecificModelParams>>(std::forward<typename SpecificModelParams::value_t>(args)), ...);

        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<filter_array_type_value_type>>{elem});
    }

    template <class Tracer = txml::EmptyTracer>
    void serialize(nlohmann::json& to, Tracer tracer = Tracer()) const
    {
        serializer_type s;
        instance_ptr->format_serialize(s, tracer);
        s.finalize(to, tracer);
    }


    template <class Tracer = txml::EmptyTracer>
    nlohmann::json serialize(Tracer tracer = Tracer()) const
    {
        nlohmann::json js = nlohmann::json::object();
        this->serialize(js, std::move(tracer));
        return js;
    }
    std::shared_ptr<value_type> instance_ptr;
};

namespace create
{
    template<class Model, class ...SpecificModelParams>
    filter<Model, SpecificModelParams...> filter_tag(typename SpecificModelParams::value_t &&...args)
    {
        return filter<Model, SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_FILTER_HPP
