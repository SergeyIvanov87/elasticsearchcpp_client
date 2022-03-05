#ifndef INTERFACES_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP

#include <stdexcept>
#include <tuple>

#include "interfaces/upload_data_provider.hpp"
#include "adapter/base_observer_mixin.hpp"


namespace interface
{
template<class NotifierImpl, class ...WhoObserve>
struct INotifierUploadDataProvider: public IUploadDataProvider
{
    template<class WhoObserveSpecific>
    using ObserverType = adapter::INotifier<WhoObserveSpecific, NotifierImpl>;

    using ObserversTupleType = std::tuple<ObserverType<WhoObserve>*...>;

    template<class WhoObserveSpecific>
    void set_notify(ObserverType<WhoObserveSpecific> *new_observer)
    {
        std::get<ObserverType<WhoObserveSpecific>*>(observers) = new_observer;
    }

    void set_observers(const ObserversTupleType& subcribers)
    {
        observers = subcribers;
    }

    template<class WhoObserveSpecific>
    ObserverType<WhoObserveSpecific>* get_observer()
    {
        return std::get<ObserverType<WhoObserveSpecific>*>(observers);
    }

    const ObserversTupleType& get_observers() const
    {
        return observers;
    }

    template<class WhomObserverNotify, class ...Args>
    void make_notify(Args&&...args)
    {
        auto observer_ptr = get_observer<WhomObserverNotify>();
        if (!observer_ptr) {
            throw std::runtime_error("No observer of requested type");
        }
        observer_ptr->notify_from(static_cast<NotifierImpl&>(*this), std::forward<Args>(args)...);
    }

    template<class ...Args>
    void make_all_notify(const Args &...args)
    {
        std::apply([this, &args...] (ObserverType<WhoObserve> *...observer_ptr)
        {
            bool dispatchingResult[]
            {
                (observer_ptr ?  observer_ptr->notify_from(static_cast<NotifierImpl&>(*this), args...), true : false)...
            };
            (void)dispatchingResult;
        }, observers);
    }
private:
    ObserversTupleType observers;
};
}
#endif // INTERFACES_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP
