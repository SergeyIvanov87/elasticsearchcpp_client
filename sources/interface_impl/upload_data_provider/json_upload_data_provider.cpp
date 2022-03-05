#ifdef HAVE_JSON

#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"

JSONUploadDataProvider::JSONUploadDataProvider(std::string &&data,
                                               bool rewind_after_finish/* = false*/) :
    JSONUploadDataProvider(json_core_t::parse(std::move(data)), rewind_after_finish)
{
}

JSONUploadDataProvider::JSONUploadDataProvider(json_core_t &&js, bool rewind_after_finish/* = false*/) :
    interface::IUploadDataProvider(),
    object(std::move(js)),
    /*
     impl(object.type() == nlohmann::json::value_t::null ?
         object.dump() : "{}\n", rewind_after_finish)
    */
    impl(object.dump(), rewind_after_finish)
{
}

JSONUploadDataProvider::~JSONUploadDataProvider() = default;

size_t JSONUploadDataProvider::fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size)
{
    return impl.fetch_data(out_buffer, in_available_buffer_bytes_size);
}

size_t JSONUploadDataProvider::size() const
{
    return impl.size();
}

size_t JSONUploadDataProvider::empty() const
{
    return impl.empty();
}

void JSONUploadDataProvider::swap(json_core_t& new_data, bool rewind_after_finish)
{
    std::swap(object, new_data);
    commit(rewind_after_finish);
}

bool JSONUploadDataProvider::has_repeat_after() const
{
    return impl.has_repeat_after();
}

JSONUploadDataProvider::accessor JSONUploadDataProvider::access()
{
    return accessor{*this, get_object()};
}

JSONUploadDataProvider::json_core_t& JSONUploadDataProvider::get_object()
{
    return object;
}

void JSONUploadDataProvider::commit(bool rewind_after_finish)
{
    impl.set_observers(this->get_observers());
    std::string new_data = object.dump();
    impl.swap(new_data, rewind_after_finish);
}
#endif // HAVE_JSON
