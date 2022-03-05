#include "adapter/easy/sstream_receiver.hpp"


namespace adapter
{
namespace easy
{
StringStreamReceiver::StringStreamReceiver() :
    base_t(CURLOPT_WRITEFUNCTION, CURLOPT_WRITEDATA)
{
}
size_t StringStreamReceiver::callback_impl(char *ptr, size_t size, size_t nmemb)
{
    data.write(ptr, size * nmemb);
    return size * nmemb;
}

std::string StringStreamReceiver::get() const
{
    return data.str();
}

void StringStreamReceiver::clear()
{
    std::stringstream().swap(data);
}
} // namespace easy
} // namespase adapter
