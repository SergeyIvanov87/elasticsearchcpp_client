#ifndef ELASTICSEARCH_BOOKS_EPUB_BOOK_MODEL_SERIALIZER_HPP
#define ELASTICSEARCH_BOOKS_EPUB_BOOK_MODEL_SERIALIZER_HPP

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/EPUB/model/Package.h"

namespace elasticsearch
{
namespace book
{
namespace model
{
namespace epub
{
struct to_model_data : public txml::FormatSerializerBase<to_model_data, txml::StaticCheckUnscopedElement,
                                                 ::EPUB::Package, ::EPUB::Metadata,
                                                 ::EPUB::DcIdentifier, ::EPUB::DcTitle,
                                                 ::EPUB::DcContributor, ::EPUB::DcCreator,
                                                 ::EPUB::DcLanguage>
{
    std::shared_ptr<elasticsearch::book::model::data> data_model;

    template<class Tracer>
    void serialize_impl(const ::EPUB::Package& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::Package::class_name());
        data_model.reset(new elasticsearch::book::model::data);
        val.format_serialize_elements(*this, tracer);
        data_model->emplace<elasticsearch::common_model::Format>("epub");
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::Metadata& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::Metadata::class_name());
        //auto& metadata = data_model.getValue<::EPUB::Metadata>();
        val.format_serialize_elements(*this, tracer);
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::DcTitle& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::DcTitle::class_name());
        data_model->set(std::make_shared<elasticsearch::book::model::element::Title>(val.getValue<::EPUB::TextElement>()->getValue()));
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::DcIdentifier& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::DcIdentifier::class_name());
        data_model->set(std::make_shared<elasticsearch::book::model::element::Identifier>(val.getValue<::EPUB::TextElement>()->getValue()));
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::DcContributor& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::DcContributor::class_name());
        data_model->set(std::make_shared<elasticsearch::book::model::element::Contributor>(val.getValue<::EPUB::TextElement>()->getValue()));
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::DcCreator& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::DcCreator::class_name());
        data_model->set(std::make_shared<elasticsearch::book::model::element::Creator>(val.getValue<::EPUB::TextElement>()->getValue()));
    }

    template<class Tracer>
    void serialize_impl(const ::EPUB::DcLanguage& val, Tracer tracer)
    {
        tracer.trace(__FUNCTION__, " - ", ::EPUB::DcLanguage::class_name());
        data_model->set(std::make_shared<elasticsearch::book::model::element::Language>(val.getValue<::EPUB::TextElement>()->getValue()));
    }

};
} // namespace epub
} // namespace model
} // namespace book
} // namespace elasticsearch
#endif // ELASTICSEARCH_BOOKS_EPUB_BOOK_MODEL_SERIALIZER_HPP
