#ifndef ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
#define ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP

#include <optional>

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

// Simple Must Tag, which used to construct `must` object.
// must object declare match conditions for different `SpecificModelParams` elements,
// which parts of `Model`
// So, must incapsulate builder functionality to match `SpecificModelParams` in `Model` set
template<class Model, class ...SpecificModelParams>
struct must
{
    using value_type = ::model::Must<Model, SpecificModelParams...>;
    using must_array_type_value_type = typename value_type::element_t;

    // standalone itself serializer
    using serializer_type = QueryMustContextToJSON<Model, SpecificModelParams...>;

    // serializer hint: how to serialize this underlying tag for consequent upperlevel tag serializer
    template<class Parent>
    using serializer_parted_type = QueryMustContextToJSONParted<Parent, Model, SpecificModelParams...>;

    must(typename SpecificModelParams::value_t &&...args)
    {
        auto elem = std::make_shared<must_array_type_value_type>();
        (elem->template emplace<::model::must::Term<Model, SpecificModelParams>>()->template emplace<::model::must::ElementToQuery<Model, SpecificModelParams>>(std::forward<typename SpecificModelParams::value_t>(args)), ...);

        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<must_array_type_value_type>>{elem});
    }

    must(std::optional<typename SpecificModelParams::value_t> &&...args)
    {
        auto elem = std::make_shared<must_array_type_value_type>();
        ((args.has_value() ? elem->template emplace<::model::must::Term<Model, SpecificModelParams>>()->template emplace<::model::must::ElementToQuery<Model, SpecificModelParams>>(std::forward<typename SpecificModelParams::value_t>(args.value())),false : true), ...);

        instance_ptr = std::make_shared<value_type>(std::initializer_list<std::shared_ptr<must_array_type_value_type>>{elem});
    }
    std::shared_ptr<value_type> instance_ptr;

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
};
namespace create
{
    template<class Model, class ...SpecificModelParams>
    must<Model, SpecificModelParams...> must_tag(typename SpecificModelParams::value_t &&...args)
    {
        return must<Model, SpecificModelParams...> (std::forward<typename SpecificModelParams::value_t>(args)...);
    }

    template<class ...SpecificModelParams>
    must<SpecificModelParams...> must_tag(std::optional<typename SpecificModelParams::value_t> &&...args)
    {
        return must<SpecificModelParams...> (std::forward<std::optional<typename SpecificModelParams::value_t>>(args)...);
    }
} // namespace create
} // namespace tag
} // namespace search
} // namespace v7
} // namespace elasticsearch
#endif // ELASTICSEARCH_7_10_REQUEST_SEARCH_TAGS_MUST_HPP
