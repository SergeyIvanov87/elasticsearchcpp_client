#ifndef ADAPTER_BASE_OBSERVER_MIXIN_HPP
#define ADAPTER_BASE_OBSERVER_MIXIN_HPP

#include <type_traits>

namespace adapter
{

template<class Impl, class Sender>
struct INotifier
{
    template<class ...SenderArgs>
    void notify_from(Sender &instance, SenderArgs &&...args)
    {
        on_notify_from(instance, std::forward<SenderArgs>(args)...);
    }

private:
    template<class ...SenderArgs>
    void on_notify_from(Sender &instance, SenderArgs &&...args)
    {
        static_cast<Impl*>(this)->template notify_observer(instance, std::forward<SenderArgs>(args)...);
    }
};

template <class Impl, class ...Observables>
struct IObserver : public INotifier<Impl, Observables>...
{
    template<class Sender, class ...NotifyArgs>
    void notify_observer(Sender &instance, NotifyArgs &&...args)
    {
        static_cast<Impl*>(this)->on_notify_from_impl(instance, std::forward<NotifyArgs>(args)...);
    }
};

template<class Sender, class ...Impls>
struct IMultiNotifier : private INotifier<Impls, Sender>...
{
    template<class Impl, class ...SenderArgs>
    void notify_from(Sender &instance, SenderArgs &&...args)
    {
        INotifier<Impl, Sender>::template on_notify_from(instance, std::forward<SenderArgs>(args)...);
    }

    template<class ...SenderArgs>
    void multi_notify_from(Sender &instance, SenderArgs &&...args)
    {
        bool expander[] = {
            (INotifier<Impls, Sender>::template on_notify_from(instance, std::forward<SenderArgs>(args)),true)...
        };
        (void)expander;
    }

};
}

#endif // ADAPTER_BASE_OBSERVER_MIXIN_HPP
