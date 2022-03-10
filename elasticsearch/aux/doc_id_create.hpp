#ifndef ELASTICSEARCH_AUX_DOC_INDEX_CREATE_HPP
#define ELASTICSEARCH_AUX_DOC_INDEX_CREATE_HPP

#include "elasticsearch/v7_10/request/put_data.hpp"
#include "elasticsearch/aux/doc_id/model.hpp"

namespace elasticsearch
{
namespace aux
{
namespace create_doc_id
{
class transaction
{
public:
    using impl_t = elasticsearch::v7::put_json_data::transaction;
    using receiver = impl_t::receiver;
    using response = elasticsearch::v7::put_json_data::response;

    static constexpr const char *name() { return "aux/create_doc_id"; }

    transaction(const std::string& host);
    ~transaction();

    template<class Tracer = txml::EmptyTracer>
    void execute_force_refresh(const std::string& index_name, bool curl_verbose = false, Tracer tracer = Tracer());

    template<class Tracer = txml::EmptyTracer>
    void execute(const std::string& index_name, bool curl_verbose = false, Tracer tracer = Tracer());

    const ::model::_Version &get_unique_index() const;

    const receiver& get_receiver() const;

    template<class Tracer = txml::EmptyTracer>
    std::shared_ptr<response> get_response(Tracer tracer = Tracer()) const;
private:
    std::unique_ptr<impl_t> generic_transaction_ptr;
};
} // namespace create_doc_id
} // namespace aux
} // namespace elasticsearch
#endif // ELASTICSEARCH_AUX_DOC_INDEX_CREATE_HPP
