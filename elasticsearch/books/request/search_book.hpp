#ifndef ELASTICSEARCH_BOOKS_REQUEST_SEARCH_HPP
#define ELASTICSEARCH_BOOKS_REQUEST_SEARCH_HPP

#include "elasticsearch/v7_10/request/search.hpp"
#include "elasticsearch/books/request/search_book_specializations.hpp"
#include "elasticsearch/v7_10/answer_model/object/KeepAlive.h"

namespace elasticsearch
{
namespace book
{
namespace search
{
class transaction : public elasticsearch::v7::search::transaction
{
public:
    using base_t = elasticsearch::v7::search::transaction;
    using model_t = elasticsearch::book::model::data;
    using response = elasticsearch::v7::search::response<model_t>;

    using base_t::base_t;

    static constexpr const char *name() { return "book/search"; }
/*
    template<class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    transaction(const std::string& host,
                const tag::query<SpecificQueryParams...> &t = {},
                Tracer tracer = Tracer()) :
        base_t(host, std::move(t), tracer)
    {
    }

    template<class Tracer = txml::EmptyTracer>
    transaction(const std::string& host,
                Tracer tracer = Tracer()) :
        base_t(host, tag::query_all {}, tracer)
    {
    }
*/
    /************* execute request to find records matching with specific query params ************/
    template <class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, q, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, q, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount,
                 const ::model::PIT &pit,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, pit, q, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class ...SpecificQueryParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, ::model::KeepAlive {keep_alive}, q, curl_verbose, tracer);
    }

    /*********************** execute request to find all available records ************************/
    template <class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, curl_verbose, tracer);
    }

    template<class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, curl_verbose, tracer);
    }

    template<class Tracer = txml::EmptyTracer>
    void execute(size_t amount,
                 const ::model::PIT &pit,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, pit, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name,
                 size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, curl_verbose, tracer);
    }

    /* execute request to find records matching with specific query params and sorted by specific fields */
    template <class ...SpecificQueryParams, class ...SortParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, q, s, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class ...SortParams,
             class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, q, s, curl_verbose, tracer);
    }


    template<class ...SpecificQueryParams, class ...SortParams,
             class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, pit, q, s, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class ...SpecificQueryParams, class ...SortParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, ::model::KeepAlive {keep_alive}, q, s, curl_verbose, tracer);
    }


    /********* execute_impl request to find all available records and sorted by specific fields ********/
    template <class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, s, curl_verbose, tracer);
    }

    template<class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, keep_alive, s, curl_verbose, tracer);
    }


    template<class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, pit, s, curl_verbose, tracer);
    }

    template <class Rep, class Period,
              class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, s, curl_verbose, tracer);
    }
/*
    template <class Rep, class Period = std::ratio<1>,
              class ...SpecificQueryParams,
              class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, q, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(index_name, amount, keep_alive, q, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 const tag::query<SpecificQueryParams...> &q,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template execute(amount, pit, q, curl_verbose, tracer);
    }

    template <class Rep, class Period = std::ratio<1>,
              class ...SpecificQueryParams,
              class ...SortParams,
              class Tracer = txml::EmptyTracer>
    void sort_execute(const std::string& index_name, size_t amount,
                 std::chrono::duration<Rep, Period> keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template sort_execute(index_name, amount, keep_alive, q, s, curl_verbose, tracer);
    }

    template<class ...SpecificQueryParams, class ...SortParams, class Tracer = txml::EmptyTracer>
    void sort_execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 const tag::query<SpecificQueryParams...> &q,
                 const tag::sort<SortParams...> &s,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template sort_execute(index_name, amount, keep_alive, q, s, curl_verbose, tracer);
    }


    template<class Model, class ...SpecificQueryParams, class ...SortParams, class Tracer = txml::EmptyTracer>
    void sort_execute(size_t amount, const ::model::PIT &pit,
                      const tag::query<Model, SpecificQueryParams...> &q,
                      const tag::sort<Model, SortParams...> &s,
                      bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::template sort_execute(amount, pit, q, s, curl_verbose, tracer);
    }
*/

/*
    using base_t::execute;

    template<class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, size_t amount,
                 const ::model::KeepAlive& keep_alive,
                 tag::sort<SortParams...> &&t,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::execute<SortParams..., Tracer>(index_name, amount, keep_alive, std::move(t),
                                               curl_verbose, tracer);
    }

    template<class ...SortParams, class Tracer = txml::EmptyTracer>
    void execute(size_t amount, const ::model::PIT &pit,
                 tag::sort<SortParams...> &&t,
                 bool curl_verbose = false, Tracer tracer = Tracer())
    {
        base_t::execute<SortParams..., Tracer>(amount, pit, std::move(t),
                                               curl_verbose, tracer);
    }
*/
    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::optional<response> get_response(Tracer tracer = Tracer()) const;
};
} // namespace search
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_REQUEST_SEARCH_HPP
