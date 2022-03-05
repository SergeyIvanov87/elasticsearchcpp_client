#ifndef INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_JSON_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_JSON_UPLOAD_DATA_PROVIDER_HPP

#ifdef HAVE_JSON
#include <nlohmann/json.hpp>

#include "interface_impl/upload_data_provider/detail/plain_container_provider_impl.hpp"
#include "adapter/easy/upload_adapter.hpp"
#include "adapter/easy/POST/POSTUpload.hpp"

struct JSONUploadDataProvider :
     public interface::INotifierDataProviderDispatcher<interface::IUploadDataProvider,
                                                       adapter::easy::UploadAdapter,
                                                       adapter::easy::POSTUploadAdapter>
{
    using json_core_t = nlohmann::json;

    struct accessor {
        friend class JSONUploadDataProvider;
        accessor(accessor &&src) :
            parent_object(src.parent_object),
            parent(src.parent),
            commit(src.commit)
        {
            src.commit = false;
        }

        accessor &operator= (accessor &&src)
        {
            if (this == &src)
            {
                return *this;
            }
            this->parent_object = src.parent_object;
            this->parent = src.parent;
            commit = src.commit;
            src.commit = false;
            return *this;
        }

        ~accessor()
        {
            if (commit)
            {
                parent.commit(parent.has_repeat_after());
            }
        }
        const json_core_t &object() const { return parent_object; }
        json_core_t &object() { commit = true; return parent_object; }
    private:
        accessor &operator= (const accessor &src) = delete;
        accessor(const accessor &src) = delete;
        accessor(JSONUploadDataProvider &p, json_core_t& obj) : parent(p),
                                                                parent_object(obj),
                                                                commit(false) {}

        json_core_t &parent_object;
        JSONUploadDataProvider &parent;
        bool commit;
    };

    JSONUploadDataProvider(std::string &&data, bool rewind_after_finish = false);
    JSONUploadDataProvider(json_core_t &&json, bool rewind_after_finish = false);
    virtual ~JSONUploadDataProvider();

    virtual size_t fetch_data(void *out_buffer, size_t in_available_buffer_bytes_size) override;
    virtual size_t size() const override;
    virtual size_t empty() const override;

    void swap(json_core_t& new_data, bool rewind_after_finish = false);
    accessor access();
private:
    json_core_t object;
    PlainContainerUploadDataProvider<std::string,
                                     adapter::easy::UploadAdapter,
                                     adapter::easy::POSTUploadAdapter> impl;
    bool has_repeat_after() const;
    json_core_t& get_object();
    void commit(bool rewind_after_finish);
};
#endif // HAVE_JSON
#endif // INTERFACES_IMPL_UPLOAD_DATA_PROVIDER_JSON_UPLOAD_DATA_PROVIDER_HPP
