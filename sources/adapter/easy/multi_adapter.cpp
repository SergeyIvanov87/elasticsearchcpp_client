#include <stdexcept>

#include "sources/adapter/easy/multi_adapter.hpp"

namespace adapter
{
namespace easy
{
MultiAdapter::MultiAdapter() :
    IEasyAdapter(),
    m_multi_handle(curl_multi_init(),  curl_multi_cleanup),
    easy_handles_count()
{
}

MultiAdapter::~MultiAdapter()
{
    //sanity (!easy_handles_count)
}

void MultiAdapter::init(CURL *curl_handle)
{
    curl_multi_add_handle(m_multi_handle.get(), curl_handle);
    easy_handles_count++;
}

void MultiAdapter::deinit(CURL *curl_handle)
{
    curl_multi_remove_handle(m_multi_handle.get(), curl_handle);
    easy_handles_count--;
}

void MultiAdapter::reinit(CURL *)
{
}

bool MultiAdapter::has_changed() const
{
    return false; // no need to change
}

CURLM* MultiAdapter::get_handle()
{
    return m_multi_handle.get();
}
} // namespace easy
} // namespase adapter
