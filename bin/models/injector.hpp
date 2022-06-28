#include <map>
#include <string>

#ifndef BIN_MODELS_INJECTOR_HPP
#define BIN_MODELS_INJECTOR_HPP

namespace bin
{
namespace data_manipulation
{
template <class Element>
struct ElementInjectableTraits {
    static constexpr bool is_injectable() { return true; }
};

template <class Element>
struct ModelInjector
{
    template <class Model, class ...AdditionalArgs>
    static void inject(Model &m, const std::map<std::string, std::string> &data_storage, AdditionalArgs &&... args)
    {
        if constexpr (ElementInjectableTraits<Element>::is_injectable())
        {
            if (auto it = data_storage.find(std::string(Element::class_name()));
                it != data_storage.end())
            {
                if (m.template has_value<Element>())
                {
                    m.template value<Element>() = Element(typename Element::value_t (it->second),
                                                          std::forward<AdditionalArgs>(args)...);
                }
                else
                {
                    m.template emplace<Element>(typename Element::value_t (it->second),
                                                std::forward<AdditionalArgs>(args)...);
                }
            }
        }
    }
};

template <class Model, class ...Elements, class ...AdditionalArgs>
void inject_to_model(Model &m, const std::map<std::string, std::string> &data_storage, AdditionalArgs &&... args)
{
    (ModelInjector<Elements>::template inject(m, data_storage, std::forward<AdditionalArgs>(args)...), ...);
}
}
}
#endif // BIN_MODELS_INJECTOR_HPP
