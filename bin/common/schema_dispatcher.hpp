#ifndef BIN_COMMON_SCHEMA_DISPATCHER_HPP
#define BIN_COMMON_SCHEMA_DISPATCHER_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

namespace bin
{
namespace common
{
struct dispatcher_settings
{
    std::string host;
    bool curl_verbose = false;
};

template<class ...Requests>
class schema_dispatcher
{
public:
    schema_dispatcher(dispatcher_settings &&s) :
        settings(std::move(s))
    {
    }
    virtual ~schema_dispatcher() = default;

    using name_t = std::string;

    template<class SpecificRequest>
    using request_ptr_t = std::shared_ptr<SpecificRequest>;

    template<class SpecificRequest>
    using schema_request_map = std::map<name_t, request_ptr_t<SpecificRequest>>;

    using container_t = std::tuple<schema_request_map<Requests>...>;

    template<class SpecificRequest, class ...SpecificRequestCtorArgs>
    request_ptr_t<SpecificRequest> create_request(const name_t &schema, SpecificRequestCtorArgs &&...args)
    {
        static_assert(std::disjunction_v<std::is_base_of<Requests, SpecificRequest>...>,
                      "SpecificRequest is not a descendant of Requests");
        static_assert(std::is_constructible_v<SpecificRequest, SpecificRequestCtorArgs...>,
                      "SpecificRequest is not constructible from provided SpecificRequestCtorArgs");
        return create_request_impl<SpecificRequest>(schema, std::forward<SpecificRequestCtorArgs>(args)...);
    }

    template<class SpecificRequest, class ...SpecificRequestArgs>
    request_ptr_t<SpecificRequest> execute_request(const name_t &schema, SpecificRequestArgs &&...args) const
    {
        static_assert(std::disjunction_v<std::is_base_of<Requests, SpecificRequest>...>,
                      "SpecificRequest is not a descendant of Requests");
        // TODO static_assert on execute
        return execute_request_impl<SpecificRequest>(schema, std::forward<SpecificRequestArgs>(args)...);
    }
    const dispatcher_settings& get_settings() const
    {
        return settings;
    }
protected:
    dispatcher_settings settings;
private:
    container_t dispatchable_schema;

    template<class SpecificRequest, class ...SpecificRequestCtorArgs>
    request_ptr_t<SpecificRequest> create_request_impl(const name_t &schema, SpecificRequestCtorArgs &&...args)
    {
        auto req = std::make_shared<SpecificRequest>(std::forward<SpecificRequestCtorArgs>(args)...);
        std::get<schema_request_map<SpecificRequest>>(dispatchable_schema)[schema] = req;
        return req;
    }

    template<class SpecificRequest, class ...SpecificRequestArgs>
    request_ptr_t<SpecificRequest> execute_request_impl(const name_t &schema, SpecificRequestArgs &&...args) const
    {
        auto &map = std::get<schema_request_map<SpecificRequest>>(dispatchable_schema);
        auto it = map.find(schema);
        if (it == map.end())
        {
            throw std::runtime_error("request for schema: " + schema + " - is not found");
        }
        it->second->execute(std::forward<SpecificRequestArgs>(args)...);
        return it->second;
    }
};
} // namespace common
} // namespace bin
#endif // BIN_COMMON_SCHEMA_DISPATCHER_HPP
