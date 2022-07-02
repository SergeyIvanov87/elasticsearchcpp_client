#ifndef INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP
#define INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP

#include "adapter/easy/sstream_receiver.hpp"

#include <txml/applications/fb2/fb2_fwd.h>

template<class Response, class Deserializer>
struct ModelBasedSStreamReceiver : public adapter::easy::StringStreamReceiver
{

template<class Tracer = txml::EmptyTracer>
Response response(Deserializer &in, Tracer tracer = Tracer{}) const
{
    std::string string_response = adapter::easy::StringStreamReceiver::get();
    std::optional<Response> resp_ptr = Response::format_deserialize(in, tracer);
    if (!resp_ptr)
    {
        std::stringstream ss;
        ss << "Cannot deserialize response: " << Response::class_name()
           << ". Parse trace:\n";
        tracer.dump(ss);
        throw std::runtime_error(ss.str());
    }

    return resp_ptr.value();
}
};

#endif // INTERFACES_IMPL_RECEIVER_MODEL_BASED_SSTREAM_RECEIVER_HPP
