#ifndef EPUB_TEXT_ELEMENT_H
#define EPUB_TEXT_ELEMENT_H

#include <ostream>
#include <string>

#include <txml/include/fwd/XMLNodeLeaf.h>

namespace EPUB
{
class TextElement : public txml::XMLNodeLeaf<TextElement, std::string>
{
public:
    using base_t = txml::XMLNodeLeaf<TextElement, std::string>;
    using value_t = typename base_t::value_t;

    static constexpr const char* class_name()
    {
        return "#text";
    }

    static constexpr txml::TextReaderWrapper::NodeType class_node_type()
    {
        return txml::TextReaderWrapper::NodeType::Text;
    };

    TextElement(std::string&& str) : base_t(std::move(str))
    {
    }
    ~TextElement() = default;

    template<class Tracer>
    static  std::optional<TextElement> create_impl(/*std::string &name, */txml::TextReaderWrapper &reader, Tracer tracer)
    {
        std::optional<TextElement> ret;
        tracer.trace("start to create '", class_name(), "'");
        if (reader.has_value())
        {
            const std::string& tmp_value = reader.get_value();
            auto it = tmp_value.begin();
            if (*it == '\r' or *it == '\n')
            {
                ++it;
            }
            ret = std::make_optional<TextElement>(std::string(it, tmp_value.end()));
            tracer.trace("Value: '", ret->value(), "'");
        }

        return ret;
    }
};
} // namespace epub
#endif //EPUB_TEXT_ELEMENT_H
