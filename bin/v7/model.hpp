#ifndef BIN_COMMON_V7_MODEL_HPP
#define BIN_COMMON_V7_MODEL_HPP

#include "elasticsearch/books/all_requests.hpp"
#include "elasticsearch/images/all_requests.hpp"
#include "bin/models/extractor.hpp"
#include "bin/models/printer.hpp"
#include "bin/models/injector.hpp"

namespace bin
{
namespace models
{
template<>
struct ElementPrintableTraits<elasticsearch::common_model::BinaryBlob> {static constexpr bool is_printable() {return false;}};

template<>
struct ElementPrintableTraits<elasticsearch::common_model::Preview> {static constexpr bool is_printable() {return false;}};

template <>
struct ElementPrinter <elasticsearch::common_model::Tags>
{
    template <class Model>
    static std::ostream &print(std::ostream &out, const Model &m)
    {
        using Element = elasticsearch::common_model::Tags;
        if (const auto &n = m.template node<Element>(); n)
        {
            const auto &list = n->value();
            out << "\"";
            std::copy(list.begin(), list.end(), std::ostream_iterator<std::string>(out, ","));
            out << "\",";
        }
        else
        {
            out << "null,";
        }
        return out;
    }
};

template <>
struct ElementPrinter <elasticsearch::image::model::element::Location>
{
    template <class Model>
    static std::ostream &print(std::ostream &out, const Model &m)
    {
        using Element = elasticsearch::image::model::element::Location;
        if (const auto &n = m.template node<Element>(); n)
        {
            out << "\"" << n->value().to_string() << "\",";
        }
        else
        {
            out << "null,";
        }
        return out;
    }
};

template <>
struct ElementPrinter <elasticsearch::image::model::element::Resolution>
{
    template <class Model>
    static std::ostream &print(std::ostream &out, const Model &m)
    {
        using Element = elasticsearch::image::model::element::Resolution;
        if (const auto &n = m.template node<Element>(); n)
        {
            out << "\"" << n->value().to_string() << "\",";
        }
        else
        {
            out << "null,";
        }
        return out;
    }
};
}

namespace data_manipulation
{
template <>
struct ElementInjectableTraits<elasticsearch::common_model::BinaryBlob> {static constexpr bool is_injectable() {return false;}};

template <>
struct ElementInjectableTraits<elasticsearch::common_model::Preview> {static constexpr bool is_injectable() {return false;}};

template <>
struct ModelInjector <elasticsearch::common_model::Tags>
{
    template <class Model>
    static void inject(Model &m, const std::map<std::string, std::string> &data_storage, const std::string &sep = {","})
    {
        using Element = elasticsearch::common_model::Tags;
        if (auto it = data_storage.find(std::string(Element::class_name()));
            it != data_storage.end())
        {
            if (m.template has_value<Element>())
            {
                m.template value<Element>() = Element(it->second, sep);
            }
            else
            {
                m.template emplace<Element>(it->second, sep);
            }
        }
    }
};

template <>
struct ModelExtractorTraits<elasticsearch::common_model::BinaryBlob> {static constexpr bool is_extractable() {return false;}};

template <>
struct ModelExtractorTraits<elasticsearch::common_model::Preview> {static constexpr bool is_extractable() {return false;}};

template <>
struct ModelExtractor <elasticsearch::common_model::Tags>
{
    template <class Model>
    static void extract(const Model &m, std::map<std::string, std::string> &data_storage)
    {
        using Element = elasticsearch::common_model::Tags;
        auto conv = [] (const Element::value_t &list)
        {
            std::stringstream ss;
            std::copy(list.begin(), list.end(), std::ostream_iterator<std::string>(ss, ","));
            return ss.str();
        };
        const auto &val = m.template node<Element>();
        if (val)
        {
            data_storage.emplace(std::string(Element::class_name()), conv(val->value()));
        }
    }
};


template <>
struct ModelExtractor <elasticsearch::image::model::element::Location>
{
    template <class Model>
    static void extract(const Model &m, std::map<std::string, std::string> &data_storage)
    {
        using Element = elasticsearch::image::model::element::Location;
        const auto &val = m.template node<Element>();
        if (val)
        {
            data_storage.emplace(std::string(Element::class_name()), val->value().to_string());
        }
    }
};

template <>
struct ModelExtractor <elasticsearch::image::model::element::Resolution>
{
    template <class Model>
    static void extract(const Model &m, std::map<std::string, std::string> &data_storage)
    {
        using Element = elasticsearch::image::model::element::Resolution;
        const auto &val = m.template node<Element>();
        if (val)
        {
            data_storage.emplace(std::string(Element::class_name()), val->value().to_string());
        }
    }
};
}
}
#endif // BIN_COMMON_V7_MODEL_HPP
