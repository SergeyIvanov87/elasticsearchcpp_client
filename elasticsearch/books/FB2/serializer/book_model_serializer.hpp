#ifndef ELASTICSEARCH_BOOKS_FB2_BOOK_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_BOOKS_FB2_BOOK_MODEL_SERIALIZER_HPP

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/FB2/model/ShortFictionBook.hpp"

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace fb2
{
struct to_model_data : public txml::FormatSerializerBase<to_model_data, txml::StaticCheckUnscopedElement,
                                                 ::fb2::ShortFictionBook,
                                                 ::fb2::Description,
                                                 ::fb2::TitleInfo, ::fb2::DocumentInfo, ::fb2::PublishInfo,
                                                 ::fb2::BookTitle, ::fb2::Genre, ::fb2::Language,
                                                 ::fb2::Author, ::fb2::Publisher, ::fb2::ISBN,
                                                 ::fb2::FirstName, ::fb2::MiddleName, ::fb2::LastName, ::fb2::NickName,
                                                 ::fb2::FB2TextElement>
{
    std::optional<elasticsearch::book::model::data> data_model;

    template<class SerializedItem, class Tracer>
    void serialize_impl(const SerializedItem &val, Tracer tracer)
    {
        val.make_format_serialize(*this, tracer);
    }
    template<class Tracer>
    void serialize_impl(const ::fb2::FB2TextElement &val, Tracer tracer)
    {
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::ShortFictionBook& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::ShortFictionBook::class_name());
        data_model = std::make_optional<elasticsearch::book::model::data>();
        val.make_format_serialize(*this, tracer);
        data_model->emplace<elasticsearch::common_model::Format>("fb2");
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::BookTitle& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::BookTitle::class_name());
        data_model->emplace<elasticsearch::book::model::element::Title>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::ISBN& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::ISBN::class_name());
        data_model->emplace<elasticsearch::book::model::element::Identifier>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::Publisher& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::Publisher::class_name());
        data_model->emplace<elasticsearch::book::model::element::Contributor>(val.value());
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::Author& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::Author::class_name());
        std::string author;
        const auto &f_name = val.node<::fb2::FirstName>();
        if (f_name)
        {
            author += f_name->value();
        }
        const auto &m_name = val.node<::fb2::MiddleName>();
        if (m_name)
        {
            author += " " + m_name->value();
        }
        const auto &l_name = val.node<::fb2::LastName>();
        if (l_name)
        {
            author += " " + l_name->value();
        }
        data_model->emplace<elasticsearch::book::model::element::Creator>(author);
    }

    template<class Tracer>
    void serialize_impl(const ::fb2::Language& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::fb2::Language::class_name());
        data_model->emplace<elasticsearch::book::model::element::Language>(val.value());
    }

};
} // namespace fb2
} // namespace model
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_FB2_BOOK_MODEL_SERIALIZER_HPP
