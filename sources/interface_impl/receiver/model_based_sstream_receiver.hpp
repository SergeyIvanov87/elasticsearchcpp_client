#ifndef INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP
#define INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP

#include "adapter/easy/sstream_receiver.hpp"

#include <txml/applications/fb2/fb2_fwd.h>

template<class Response, class Deserializer>
struct ModelBasedSStreamReceiver : public adapter::easy::StringStreamReceiver
{

template<class Tracer = txml::EmptyTracer>
std::shared_ptr<Response> response(Deserializer &in, Tracer tracer = Tracer{}) const
{
    std::string string_response = adapter::easy::StringStreamReceiver::get();
    std::shared_ptr<Response> resp_ptr = Response::format_deserialize(in, tracer);
    if (!resp_ptr)
    {
        throw std::runtime_error(std::string("Cannot deserialize response: ") + Response::class_name().data());
    }

    return resp_ptr;
}
};

#endif // INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP
