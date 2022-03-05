#include "interface_impl/upload_data_provider/binary_array_upload_data_provider.hpp"

BinaryArrayUploadDataProvider::BinaryArrayUploadDataProvider(std::vector<unsigned char> &&data,
                                                             bool rewind_after_finish) :
    impl(std::move(data), rewind_after_finish)
{
}

BinaryArrayUploadDataProvider::~BinaryArrayUploadDataProvider() = default;

size_t BinaryArrayUploadDataProvider::fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size)
{
    return impl.fetch_data(out_buffer, in_available_buffer_bytes_size);
}

size_t BinaryArrayUploadDataProvider::size() const
{
    return impl.size();
}

size_t BinaryArrayUploadDataProvider::empty() const
{
    return impl.empty();
}

void BinaryArrayUploadDataProvider::swap(std::vector<unsigned char>& new_data, bool rewind_after_finish)
{
    impl.set_observers(this->get_observers());
    impl.swap(new_data, rewind_after_finish);
}
