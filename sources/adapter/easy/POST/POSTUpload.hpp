#ifndef ADAPTER_EASY_POST_UPLOAD_ADAPTER_HPP
#define ADAPTER_EASY_POST_UPLOAD_ADAPTER_HPP
#include <memory>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "adapter/easy/adapter_callback_interface.hpp"
#include "adapter/base_observer_mixin.hpp"

namespace interface
{
    struct IUploadDataProvider;
    template<class NotifierImpl, class ...WhoObserve>
    struct INotifierUploadDataProvider;

    template<class NotifierImpl, class WhoObserve>
    struct ISingleNotifierUploadDataProvider;
}
namespace adapter
{
namespace easy
{
class POSTUploadAdapter: public IEasyCallbackAdapter<POSTUploadAdapter, true, char *, size_t, size_t>,
                         public IObserver<POSTUploadAdapter, interface::IUploadDataProvider>
{
public:
    using base_t = IEasyCallbackAdapter<POSTUploadAdapter, true, char *, size_t, size_t>;
    using UploaderNotifierInterface = interface::ISingleNotifierUploadDataProvider<interface::IUploadDataProvider, POSTUploadAdapter>;

    using base_t::base_t;

    friend class IAdapter;
    friend base_t;
    size_t callback_impl(char *ptr, size_t size, size_t nmemb);
    void on_notify_from_impl(interface::IUploadDataProvider&, size_t new_data_size);
protected:
    //POSTUploadAdapter(std::shared_ptr<interface::IUploadDataProvider> source);
    POSTUploadAdapter(std::shared_ptr<UploaderNotifierInterface> source);
    POSTUploadAdapter(std::vector<unsigned char>&& source, bool rewind_after_finish = false);
    POSTUploadAdapter(std::string&& source, bool rewind_after_finish = false);
    POSTUploadAdapter(std::istringstream&& source, bool rewind_after_finish = false);
    POSTUploadAdapter(std::stringstream&& source, bool rewind_after_finish = false);
    POSTUploadAdapter(nlohmann::json&& source, bool rewind_after_finish = false);

    void init_next(CURL *curl_handle);
    void deinit_next(CURL *curl_handle);
    void reinit_next(CURL *curl_handle);
    bool has_changed_impl() const;
/*
    std::shared_ptr<interface::IUploadDataProvider> reset(std::shared_ptr<interface::IUploadDataProvider> new_source_ptr);
private:
    std::shared_ptr<interface::IUploadDataProvider> data_source_ptr;
*/
    std::shared_ptr<UploaderNotifierInterface> reset(std::shared_ptr<UploaderNotifierInterface> new_source_ptr);
private:
    std::shared_ptr<UploaderNotifierInterface> data_source_ptr;
    bool data_source_changed;
};
} // namespace easy
} // namespase adapter
#endif // ADAPTER_EASY_POST_UPLOAD_ADAPTER_HPP
