#ifndef INTERFACES_NOTIFIER_DATA_PROVIDER_DISPATCHER_HPP
#define INTERFACES_NOTIFIER_DATA_PROVIDER_DISPATCHER_HPP

#include <stdexcept>
#include <tuple>

#include "interfaces/upload_data_provider.hpp"
#include "adapter/base_observer_mixin.hpp"
#include "interfaces/single_notifier_upload_data_provider.hpp"


namespace interface
{
template<class NotifierImpl, class ...WhoObserve>
struct INotifierDataProviderDispatcher : public virtual ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>...
{
    using single_notifiers_types = std::tuple<ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>...>;
    using observer_full_types = std::tuple<WhoObserve...>;
    using observer_types = std::tuple<typename ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>::ObserverType *...>;

    template<class WhoObserveSpecific>
    adapter::INotifier<WhoObserveSpecific, NotifierImpl> *
        set_notify(adapter::INotifier<WhoObserveSpecific, NotifierImpl> *new_observer)
    {
        using parent_candidate_type = ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserveSpecific>;
        return parent_candidate_type::set_notify(new_observer);
    }

    template<class WhoObserveSpecific>
    typename ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserveSpecific>::ObserverType* get_observer()
    {
        using parent_candidate_type = ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserveSpecific>;
        return parent_candidate_type::get_observer();
    }

    observer_types get_observers()
    {
        observer_types ret;
        std::apply([this] (adapter::INotifier<WhoObserve, NotifierImpl> *&...observer_ptr)
        {
            bool dispatchingResult[]
            {
                ((observer_ptr = static_cast<ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>*>(this)->get_observer()), true)...
            };
        }, ret);
        return ret;
    }

    template<class ...NewObservers>
    void set_observers(const std::tuple<adapter::INotifier<NewObservers, NotifierImpl>*...>& observers_holder)//INotifierDataProviderDispatcher<NotifierImpl, NewObservers...> &observers_holder)
    {
        bool dispatchingResult[]
        {
            (set_notify_silent<NewObservers>(
                std::get<adapter::INotifier<NewObservers, NotifierImpl> *>(observers_holder)), true)...
        };
    }

    template<class WhomObserverNotify, class ...Args>
    void make_notify(Args&&...args)
    {
        using parent_candidate_type = ISingleNotifierUploadDataProvider<NotifierImpl, WhomObserverNotify>;
        parent_candidate_type::make_notify(std::forward<Args>(args)...);
    }

    template<class ...Args>
    void make_all_notify(const Args &...args)
    {
        bool dispatchingResult[]
        {
            (static_cast<ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>*>(this)->get_observer() ?
             static_cast<ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserve>*>(this)->get_observer()->notify_from(static_cast<NotifierImpl&>(*this), args...), true
             : false)...
        };
        (void)dispatchingResult;
    }
private:
    template<class WhoObserveSpecific>
    void set_notify_silent(adapter::INotifier<WhoObserveSpecific, NotifierImpl> *new_observer)
    {
        bool dispatchingResult[]
        {
            (std::is_same_v<WhoObserveSpecific, WhoObserve> ?
             static_cast<ISingleNotifierUploadDataProvider<NotifierImpl, WhoObserveSpecific>*>(this)->set_notify(new_observer), true
             : false
            )...
        };
    }
};

/*
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
};*/
}
#endif // INTERFACES_NOTIFIER_DATA_PROVIDER_DISPATCHER_HPP
