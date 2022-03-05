#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_PLAIN_CONTAINER_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_PLAIN_CONTAINER_DATA_PROVIDER_HPP

#include <algorithm>

#include "interfaces/notifier_data_provider_dispatcher.hpp"

template<class Container, class ...Observers>
struct PlainContainerUploadDataProvider final :
                public interface::INotifierDataProviderDispatcher<interface::IUploadDataProvider,
                                                                  Observers...>
{
    PlainContainerUploadDataProvider(Container &&data, bool rewind_after_finish) :
        impl(std::move(data)),
        current_data_offset_bytes(0),
        repeat_after_finish(rewind_after_finish)
    {
    }
    ~PlainContainerUploadDataProvider() = default;

    size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) override
    {
        auto it = impl.begin();
        if (repeat_after_finish && current_data_offset_bytes == impl.size())
        {
            current_data_offset_bytes = 0;
        }

        std::advance(it, current_data_offset_bytes);
        auto fit = std::copy_n(it, std::min(in_available_buffer_bytes_size, impl.size() - current_data_offset_bytes), static_cast<unsigned char*>(out_buffer));
        auto count = std::distance(static_cast<unsigned char*>(out_buffer), fit);
        current_data_offset_bytes += count;
        return count;
    }

    size_t size() const override
    {
          return (impl.size() - current_data_offset_bytes);
    }

    size_t empty() const override
    {
        return !(size() - current_data_offset_bytes);
    }

    void swap(Container& new_data, bool rewind_after_finish = false)
    {
        std::swap(impl, new_data);
        repeat_after_finish = rewind_after_finish;
        current_data_offset_bytes = 0;

        this->make_all_notify(impl.size());
    }

    bool has_repeat_after() const
    {
        return repeat_after_finish;
    }
private:
    Container impl;
    size_t current_data_offset_bytes;
    bool repeat_after_finish; /* rewind to begin */
};

#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_PLAIN_CONTAINER_DATA_PROVIDER_HPP
