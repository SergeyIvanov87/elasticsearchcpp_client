#include "interface_impl/upload_data_provider/string_upload_data_provider.hpp"

StringUploadDataProvider::StringUploadDataProvider(std::string &&data,
                                                   bool rewind_after_finish/* = false*/) :
    interface::IUploadDataProvider(),
    impl(std::move(data), rewind_after_finish)
{
}

StringUploadDataProvider::~StringUploadDataProvider() = default;

size_t StringUploadDataProvider::fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size)
{
    return impl.fetch_data(out_buffer, in_available_buffer_bytes_size);
}

size_t StringUploadDataProvider::size() const
{
    return impl.size();
}

size_t StringUploadDataProvider::empty() const
{
    return impl.empty();
}

void StringUploadDataProvider::swap(std::string& new_data, bool rewind_after_finish)
{
    impl.set_observers(this->get_observers());
    impl.swap(new_data, rewind_after_finish);
}
