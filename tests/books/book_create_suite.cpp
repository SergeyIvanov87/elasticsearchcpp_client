#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/request/create_book.hpp"
#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"
#include "elasticsearch/books/data_model/serializers/serializer.hpp"

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/books/EPUB/model/Package.h"
#include "elasticsearch/books/EPUB/serializer/book_model_serializer.hpp"

#include "elasticsearch/books/FB2/model/ShortFictionBook.hpp"
#include "elasticsearch/books/FB2/serializer/book_model_serializer.hpp"


#include <gtest/gtest.h>

namespace tests
{
class BookCreateFixture : public ::testing::Test,
                          public Settings
{
protected:

    void SetUp() override {
        using namespace elasticsearch::book::index_mapping;
        transaction req(get_host());
        ASSERT_NO_THROW(req.execute(get_index(), false));

        std::shared_ptr<transaction::response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->getValue<model::Ack>());
        ASSERT_EQ(ans_ptr->getValue<model::Ack>()->getValue(), true);
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), curl_verbose());
    }
};


TEST_F(BookCreateFixture, request_epub)
{
    using namespace elasticsearch::book;
    using namespace elasticsearch::book::model;

    EPUB::Package book_package;
    auto meta = book_package.emplace<EPUB::Metadata>();
    meta->emplace<EPUB::DcTitle>()->emplace<EPUB::TextElement>("title");
    meta->emplace<EPUB::DcIdentifier>()->emplace<EPUB::TextElement>("identifier");
    meta->emplace<EPUB::DcContributor>()->emplace<EPUB::TextElement>("contributor");
    meta->emplace<EPUB::DcCreator>()->emplace<EPUB::TextElement>("creator");
    meta->emplace<EPUB::DcLanguage>()->emplace<EPUB::TextElement>("language");

    epub::to_model_data b2m;
    txml::StdoutTracer std_tracer;
    book_package.format_serialize(b2m, std_tracer);

    // postproc
    b2m.data_model->emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19});
    b2m.data_model->emplace<elasticsearch::common_model::OriginalPath>("test_path");

    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", *b2m.data_model,
                                curl_verbose(), std_tracer));

    std::shared_ptr<create::transaction::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr->getValue<::model::Result>());
    ASSERT_EQ(ans_ptr->getValue<::model::Result>()->getValue(), "created");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Version>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Version>()->getValue(), 1);
    ASSERT_TRUE(ans_ptr->getValue<::model::_Id>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Id>()->getValue(), "1");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Index>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<::model::_Type>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Type>()->getValue(), "_doc");



    get::transaction get_book(get_host());
    ASSERT_NO_THROW(get_book.execute(get_index() + "/_doc/1", curl_verbose()));
    std::shared_ptr<get::transaction::response> return_model;
    ASSERT_NO_THROW(return_model = get_book.get_response(std_tracer));


    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::Format>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::Format>()->getValue());

    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Contributor>()->getValue(),
              meta->getValue<EPUB::DcContributor>()->getValue<EPUB::TextElement>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Creator>()->getValue(),
             meta->getValue<EPUB::DcCreator>()->getValue<EPUB::TextElement>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Identifier>()->getValue(),
             meta->getValue<EPUB::DcIdentifier>()->getValue<EPUB::TextElement>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Language>()->getValue(),
             meta->getValue<EPUB::DcLanguage>()->getValue<EPUB::TextElement>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Title>()->getValue(),
             meta->getValue<EPUB::DcTitle>()->getValue<EPUB::TextElement>()->getValue());
}



TEST_F(BookCreateFixture, request_fb2)
{
    using namespace elasticsearch::book;
    //using namespace elasticsearch::book::model;

    fb2::ShortFictionBook book_package;
    auto description = book_package.emplace<fb2::Description>();
    auto title_info = description->emplace<fb2::TitleInfo>();
    title_info->emplace<fb2::BookTitle>()->emplace<fb2::FB2TextElement>("title");
    title_info->emplace<fb2::Language>()->emplace<fb2::FB2TextElement>("language");

    auto doc_info = description->emplace<fb2::DocumentInfo>();
    auto author = doc_info->emplace<fb2::Author>();
    author->emplace<fb2::FirstName>()->emplace<fb2::FB2TextElement>("first");
    author->emplace<fb2::MiddleName>()->emplace<fb2::FB2TextElement>("middle");
    author->emplace<fb2::LastName>()->emplace<fb2::FB2TextElement>("last");
    std::string author_str = "first middle last";
    auto publish_info = description->emplace<fb2::PublishInfo>();
    publish_info->emplace<fb2::Publisher>()->emplace<fb2::FB2TextElement>("publisher");
    publish_info->emplace<fb2::ISBN>()->emplace<fb2::FB2TextElement>("isbn");


    elasticsearch::book::model::fb2::to_model_data b2m;
    txml::StdoutTracer std_tracer;
    book_package.format_serialize(b2m, std_tracer);

    // postproc
    b2m.data_model->emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19});
    b2m.data_model->emplace<elasticsearch::common_model::OriginalPath>("test_path");

    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", *b2m.data_model,
                                curl_verbose(), std_tracer));

    std::shared_ptr<create::transaction::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr->getValue<::model::Result>());
    ASSERT_EQ(ans_ptr->getValue<::model::Result>()->getValue(), "created");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Version>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Version>()->getValue(), 1);
    ASSERT_TRUE(ans_ptr->getValue<::model::_Id>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Id>()->getValue(), "1");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Index>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<::model::_Type>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Type>()->getValue(), "_doc");



    get::transaction get_book(get_host());
    ASSERT_NO_THROW(get_book.execute(get_index() + "/_doc/1", curl_verbose()));
    std::shared_ptr<get::transaction::response> return_model;
    ASSERT_NO_THROW(return_model = get_book.get_response(std_tracer));


    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::Format>()->getValue(),
              b2m.data_model->getValue<elasticsearch::common_model::Format>()->getValue());

    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Contributor>()->getValue(),
              publish_info->getValue<fb2::Publisher>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Creator>()->getValue(),
             author_str);
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Identifier>()->getValue(),
             publish_info->getValue<fb2::ISBN>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Language>()->getValue(),
             title_info->getValue<fb2::Language>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::book::model::element::Title>()->getValue(),
             title_info->getValue<fb2::BookTitle>()->getValue());
}
}
