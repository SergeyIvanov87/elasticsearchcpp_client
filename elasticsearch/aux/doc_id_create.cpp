#include "elasticsearch/aux/doc_id_create.hpp"
#include "elasticsearch/aux/doc_id/serializer.hpp"

namespace elasticsearch
{
namespace aux
{
namespace create_doc_id
{
transaction::transaction(const std::string& host)
{
    generic_transaction_ptr.reset(new impl_t(host, false));
}

transaction::~transaction() = default;

template<class Tracer>
void transaction::execute_force_refresh(const std::string& index_name,
                                        bool curl_verbose/* = false*/,
                                        Tracer tracer/* = Tracer()*/)
{
    static const elasticsearch::aux::model::data empty;
    generic_transaction_ptr->execute_force_refresh(index_name,
                                                   impl_t::Tag<elasticsearch::aux::model::data,
                                                               elasticsearch::aux::model::to_data>{empty},
                                                   curl_verbose,
                                                   tracer);
}

template<class Tracer>
void transaction::execute(const std::string& index_name,
                          bool curl_verbose/* = false*/,
                          Tracer tracer/* = Tracer()*/)
{
    static const elasticsearch::aux::model::data empty;
    generic_transaction_ptr->execute(index_name,
                                     impl_t::Tag<elasticsearch::aux::model::data,
                                                 elasticsearch::aux::model::to_data>{empty},
                                     curl_verbose,
                                     tracer);
}

const ::model::_Version &transaction::get_unique_index() const
{
    auto ans_ptr = get_response();
    if (ans_ptr.has_value<::model::Result>())
    {
        const auto &result_value = ans_ptr.value<::model::Result>().value();
        if (result_value == "created" || result_value == "updated")
        {
            return ans_ptr.value<::model::_Version>();
        }
        throw std::runtime_error("Cannot generate unique index, result: " + result_value);
    }

    throw std::runtime_error("Cannot generate unique index");
}

const transaction::receiver& transaction::get_receiver() const
{
    return generic_transaction_ptr->get_receiver();
}

template<class Tracer>
transaction::response transaction::get_response(Tracer tracer) const
{
    return generic_transaction_ptr->get_response(tracer);
}

template void transaction::execute_force_refresh(const std::string& index_name,
                                                 bool curl_verbose,
                                                 txml::StdoutTracer);
template void transaction::execute_force_refresh(const std::string& index_name,
                                                 bool curl_verbose,
                                                 txml::EmptyTracer);

template void transaction::execute(const std::string& index_name,
                                   bool curl_verbose,
                                   txml::StdoutTracer);
template void transaction::execute(const std::string& index_name,
                                   bool curl_verbose,
                                   txml::EmptyTracer);

template transaction::response transaction::get_response(txml::StdoutTracer) const;
template transaction::response transaction::get_response(txml::EmptyTracer) const;
}// namespace create_doc_id
}// namespace aux
} // namespace elasticsearch
