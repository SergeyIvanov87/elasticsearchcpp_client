#include <algorithm>
#include <iterator>
#include <ostream>

#ifndef BIN_MODELS_PRINTER_HPP
#define BIN_MODELS_PRINTER_HPP
namespace bin
{
namespace models
{
template <class Element>
struct ElementPrintableTraits {
    static constexpr bool is_printable() { return true; }
};

template <class Element>
struct ElementPrinterBase
{
    static std::ostream &print_element(std::ostream &out, const std::string &sep)
    {
        if constexpr (ElementPrintableTraits<Element>::is_printable())
        {
            out << Element::class_name() << sep ;
        }
        return out;
    }
};

template <class Element>
struct ElementPrinter : public ElementPrinterBase<Element>
{
    using ElementPrinterBase<Element>::print_element;

    template <class Model>
    static std::ostream &print_element_value(std::ostream &out, const Model &m, const std::string &sep)
    {
        if constexpr (ElementPrintableTraits<Element>::is_printable())
        {
            const auto &n = m.template node<Element>();
            if (n)
            {
                out << "\"" << n->value() << "\"" << sep ;
            }
            else
            {
                out << "null" << sep;
            }
        }
        return out;
    }
};

template<class Model, class ...Elements>
std::ostream &print_element_values(std::ostream &out, const Model &m, const std::string &sep = ",")
{
    (ElementPrinter<Elements>::template print_element_value(out, m, sep), ...);
    return out;
}

template<class ...Elements>
std::ostream &print_elements(std::ostream &out, const std::string &sep = ",")
{
    (ElementPrinter<Elements>::print_element(out, sep), ...);
    return out;
}

}
}
#endif // BIN_MODELS_PRINTER_HPP
