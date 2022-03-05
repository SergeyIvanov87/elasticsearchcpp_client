#ifndef ADAPTER_EASY_SSTREAM_RECEIVER_HPP
#define ADAPTER_EASY_SSTREAM_RECEIVER_HPP
#include <sstream>

#include "adapter/easy/adapter_callback_interface.hpp"

namespace adapter
{
namespace easy
{
class StringStreamReceiver: public IEasyCallbackAdapter<StringStreamReceiver, false, char *, size_t, size_t>
{
public:
    using base_t = IEasyCallbackAdapter<StringStreamReceiver, false, char *, size_t, size_t>;

    using base_t::base_t;

    friend class IAdapter;
    size_t callback_impl(char *ptr, size_t size, size_t nmemb);
    std::string get() const;
    void clear();

    StringStreamReceiver();
private:
    std::stringstream data;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_SSTREAM_RECEIVER_HPP
