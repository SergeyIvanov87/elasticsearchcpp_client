#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/books/data_model/serializers/deserializer.hpp"
#include "elasticsearch/common_model/serializers/deserializer.hpp"

namespace elasticsearch
{
namespace book
{
namespace get
{
transaction::transaction(const std::string& host):
    impl_t(host)
{
}

transaction::~transaction() = default;

template<class Tracer>
transaction::response transaction::get_response(Tracer tracer) const
{
    auto &&res = impl_t::template get_response<response,
                                                        elasticsearch::book::model::from_data,
                                                        elasticsearch::common_model::from_data>(tracer);

    if (!res.template has_value<::model::_Source<response>>())
    {
        std::stringstream ss;
        ss << "Unexpected response for: " << name()
           << ". Reason: missing field \"" << ::model::_Source<response>::class_name()
           << ". Parsing trace:\n";
        tracer.dump(ss);
        throw std::runtime_error(ss.str());
    }

    auto &&source_ptr = res.template value<::model::_Source<response>>();
    return source_ptr.template value<response>();
}

template transaction::response transaction::get_response(txml::StdoutTracer) const;
template transaction::response transaction::get_response(txml::EmptyTracer) const;
} // namespace get
} // namespace book
} // namespace elasticsearch
