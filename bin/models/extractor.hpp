#include <map>
#include <string>

#ifndef BIN_MODELS_EXTRACTOR_HPP
#define BIN_MODELS_EXTRACTOR_HPP

namespace bin
{
namespace data_manipulation
{
template <class Element>
struct ModelExtractorTraits {
    static constexpr bool is_extractable() { return true; }
};

template <class Element>
struct ModelExtractor
{
    template <class Model>
    static void extract(const Model &m, std::map<std::string, std::string> &data_storage)
    {
        if constexpr (ModelExtractorTraits<Element>::is_extractable())
        {
            const auto &val = m.template node<Element>();
            if (val)
            {
                data_storage.emplace(std::string(Element::class_name()), val->value());
            }
        }
    }
};

template <class Model, class ...Elements>
void extract_from_model(const Model &m, std::map<std::string, std::string> &data_storage)
{
    (ModelExtractor<Elements>::template extract(m, data_storage), ...);
}
}
}
#endif // BIN_MODELS_EXTRACTOR_HPP
