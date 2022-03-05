#include <stdexcept>

#include "adapter/curlu/url_part_adapter.hpp"

namespace adapter
{
namespace curlu
{
URLPartAdapter::URLPartAdapter(std::string_view url_part) :
    weak_handle(),
    url(url_part)
{
}

URLPartAdapter::~URLPartAdapter() = default;

void URLPartAdapter::init(CURLU *curlu_handle)
{
    CURLUcode ret = curl_url_set(curlu_handle, CURLUPART_URL, url.c_str(), 0);
    if (ret != CURLUE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed with CURLU error: " +
                                 std::to_string(ret) + ". URL: " + url);
    }

    weak_handle = curlu_handle;
}

void URLPartAdapter::deinit(CURLU *curlu_handle)
{
    CURLUcode ret = curl_url_set(curlu_handle, CURLUPART_URL, nullptr, 0);
    if (ret != CURLUE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed with CURLU error: " +
                                 std::to_string(ret));
    }

    weak_handle = nullptr;
}

void URLPartAdapter::set_path(const std::string& path)
{
    CURLUcode ret = curl_url_set(weak_handle, CURLUPART_PATH, path.c_str(), 0);
    if (ret != CURLUE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed with CURLU error: " +
                                 std::to_string(ret) + ". Path: " + path);
    }
}

void URLPartAdapter::set_query_param(const std::string& query, unsigned int flags)
{
    CURLUcode ret = curl_url_set(weak_handle, CURLUPART_QUERY, query.c_str(), flags);
    if (ret != CURLUE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed with CURLU error: " +
                                 std::to_string(ret) + ". Query: " + query);
    }

    query_param_is_set = !query.empty();
}

void URLPartAdapter::add_query_param(const std::string& query,  unsigned int flags)
{
    if (query_param_is_set)
    {
         flags |= CURLU_APPENDQUERY;
    }

    CURLUcode ret = curl_url_set(weak_handle, CURLUPART_QUERY, query.c_str(), flags);
    if (ret != CURLUE_OK)
    {
        throw std::runtime_error(std::string(__FUNCTION__) + " - failed with CURLU error: " +
                                 std::to_string(ret) + ". Query: " + query);
    }
    query_param_is_set |= !query.empty(); /*`non-empty` then `empty` should not reset query_param_is_set*/
}

const std::string& URLPartAdapter::get_url_part() const
{
    return url;
}

std::string& URLPartAdapter::get_url_part()
{
    return const_cast<std::string&>(static_cast<const URLPartAdapter*>(this)->get_url_part());
}
} // namespace curl
} // namespace adapter
