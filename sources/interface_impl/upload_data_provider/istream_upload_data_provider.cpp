#include <algorithm>

#include "interface_impl/upload_data_provider/istream_upload_data_provider.hpp"
#include "adapter/easy/upload_adapter.hpp"
#include "adapter/easy/POST/POSTUpload.hpp"

IStreamUploadDataProvider::IStreamUploadDataProvider(std::istream *data, bool rewind_after_finish) :
    stream_ptr(data),
    rewind_stream(rewind_after_finish)
{
}

IStreamUploadDataProvider::~IStreamUploadDataProvider() = default;

size_t IStreamUploadDataProvider::fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size)
{
    if(empty())
    {
        if (!rewind_stream)
        {
            return 0;
        }
        stream_ptr->clear();
        stream_ptr->seekg(0, std::ios_base::beg);
    }
    stream_ptr->read(reinterpret_cast<char*>(out_buffer), in_available_buffer_bytes_size);
    return stream_ptr->gcount();
}

size_t IStreamUploadDataProvider::size() const
{
    if (!stream_ptr)
    {
        return 0;
    }
    auto cur_pos = stream_ptr->tellg();

    stream_ptr->seekg(0, std::ios_base::end);
    auto size = stream_ptr->tellg();
    stream_ptr->seekg(cur_pos, std::ios_base::beg);
    return size - cur_pos;
}

size_t IStreamUploadDataProvider::empty() const
{
    return !stream_ptr || stream_ptr->eof() || stream_ptr->bad();
}

void IStreamUploadDataProvider::reset(std::istream *new_data, bool rewind_after_finish)
{
    std::swap(new_data, stream_ptr);
    rewind_stream = rewind_after_finish;

    this->make_all_notify(size());
}
