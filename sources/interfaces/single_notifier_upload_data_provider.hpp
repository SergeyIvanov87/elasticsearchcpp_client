#ifndef INTERFACES_SINGLE_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP
#define INTERFACES_SINGLE_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP

#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "interfaces/upload_data_provider.hpp"
#include "adapter/base_observer_mixin.hpp"


namespace interface
{
template<class NotifierImpl, class Observer>
struct ISingleNotifierUploadDataProvider: public virtual IUploadDataProvider
{
    using NotifierType = NotifierImpl;
    using ObserverFullType = Observer;
    using ObserverType = adapter::INotifier<Observer, NotifierImpl>;

    ObserverType* set_notify(ObserverType *new_observer)
    {
        ObserverType *old_observer = observer;
        observer = new_observer;
        return old_observer;
    }

    ObserverType* get_observer()
    {
        return observer;
    }

    template <class ...Args>
    void make_notify(Args&&...args)
    {
        if (!observer) {
            throw std::runtime_error("No observer to notify");
        }
        observer->notify_from(static_cast<NotifierImpl&>(*this), std::forward<Args>(args)...);
    }
protected:
    template<class UnknownObserver>
    adapter::INotifier<UnknownObserver, NotifierImpl>*
        set_notify_silent(adapter::INotifier<UnknownObserver, NotifierImpl> *new_observer)
    {
        if constexpr(std::is_same_v<UnknownObserver,Observer>)
        {
            return set_notify();
        }
        else
        {
            return nullptr;
        }
    }
private:
    ObserverType* observer = nullptr;
};
}
#endif // INTERFACES_SINGLE_NOTIFIER_UPLOAD_DATA_PROVIDER_HPP
