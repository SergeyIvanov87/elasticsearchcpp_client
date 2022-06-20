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
struct ElementPrinter
{
    template <class Model>
    static std::ostream &print(std::ostream &out, const Model &m)
    {
        if constexpr (ElementPrintableTraits<Element>::is_printable())
        {
            if (m.template node<Element>())
            {
                out << "\"" << m.template node<Element>()->value() << "\"," ;
            }
            else
            {
                out << "null,";
            }
        }
        return out;
    }
};

template<class Model, class ...Elements>
std::ostream &print_elements(std::ostream &out, const Model &m)
{
    (ElementPrinter<Elements>::template print(out, m), ...);
    return out;
}


}
}
#endif // BIN_MODELS_PRINTER_HPP
