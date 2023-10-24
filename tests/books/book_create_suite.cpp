#include <iostream>
#include <txml/applications/fb2/fb2.hpp>

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

        transaction::response ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr.node<model::Ack>());
        ASSERT_EQ(ans_ptr.node<model::Ack>()->value(), true);
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
    auto &meta = book_package.emplace<EPUB::Metadata>().first.get();
    meta->emplace<EPUB::DcTitle>().first.get()->emplace<EPUB::TextElement>("title");
    meta->emplace<EPUB::DcIdentifier>().first.get()->emplace<EPUB::TextElement>("identifier");
    meta->emplace<EPUB::DcContributor>().first.get()->emplace<EPUB::TextElement>("contributor");
    meta->emplace<EPUB::DcCreator>().first.get()->emplace<EPUB::TextElement>("creator");
    meta->emplace<EPUB::DcLanguage>().first.get()->emplace<EPUB::TextElement>("language");

    epub::to_model_data b2m;
    txml::StdoutTracer std_tracer;
    book_package.format_serialize(b2m, std_tracer);

    // postproc
    b2m.data_model->emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19});
    b2m.data_model->emplace<elasticsearch::common_model::OriginalPath>("test_path");

    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", *b2m.data_model,
                                curl_verbose(), std_tracer));

    create::transaction::response ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr.node<::model::Result>());
    ASSERT_EQ(ans_ptr.node<::model::Result>()->value(), "created");
    ASSERT_TRUE(ans_ptr.node<::model::_Version>());
    ASSERT_EQ(ans_ptr.node<::model::_Version>()->value(), 1);
    ASSERT_TRUE(ans_ptr.node<::model::_Id>());
    ASSERT_EQ(ans_ptr.node<::model::_Id>()->value(), "1");
    ASSERT_TRUE(ans_ptr.node<::model::_Index>());
    ASSERT_EQ(ans_ptr.node<::model::_Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr.node<::model::_Type>());
    ASSERT_EQ(ans_ptr.node<::model::_Type>()->value(), "_doc");



    get::transaction get_book(get_host());
    ASSERT_NO_THROW(get_book.execute(get_index() + "/_doc/1", curl_verbose()));
    std::optional<get::transaction::response> return_model;
    ASSERT_NO_THROW(return_model = get_book.get_response(std_tracer));


    ASSERT_EQ(return_model->node<elasticsearch::common_model::OriginalPath>()->value(),
              b2m.data_model->node<elasticsearch::common_model::OriginalPath>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::common_model::BinaryBlob>()->value(),
              b2m.data_model->node<elasticsearch::common_model::BinaryBlob>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::common_model::Format>()->value(),
              b2m.data_model->node<elasticsearch::common_model::Format>()->value());

    ASSERT_EQ(return_model->node<elasticsearch::book::model::element::Contributor>()->value(),
              meta->node<EPUB::DcContributor>()->node<EPUB::TextElement>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::book::model::element::Creator>()->value(),
             meta->node<EPUB::DcCreator>()->node<EPUB::TextElement>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::book::model::element::Identifier>()->value(),
             meta->node<EPUB::DcIdentifier>()->node<EPUB::TextElement>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::book::model::element::Language>()->value(),
             meta->node<EPUB::DcLanguage>()->node<EPUB::TextElement>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::book::model::element::Title>()->value(),
             meta->node<EPUB::DcTitle>()->node<EPUB::TextElement>()->value());
}



TEST_F(BookCreateFixture, request_fb2)
{
    using namespace elasticsearch::book;
    //using namespace elasticsearch::book::model;

    fb2::ShortFictionBook book_package;
    auto &description = book_package.emplace<fb2::Description>().first.get();
    auto &title_info = description->emplace<fb2::TitleInfo>().first.get();
    title_info->emplace<fb2::BookTitle>().first.get()->emplace<fb2::FB2TextElement>("title");
    title_info->emplace<fb2::Language>().first.get()->emplace<fb2::FB2TextElement>("language");

    auto &doc_info = description->emplace<fb2::DocumentInfo>().first.get();
    auto &author = doc_info->emplace<fb2::Author>().first.get();
    author->emplace<fb2::FirstName>().first.get()->emplace<fb2::FB2TextElement>("first");
    author->emplace<fb2::MiddleName>().first.get()->emplace<fb2::FB2TextElement>("middle");
    author->emplace<fb2::LastName>().first.get()->emplace<fb2::FB2TextElement>("last");
    std::string author_str = "first middle last";
    auto &publish_info = description->emplace<fb2::PublishInfo>().first.get();
    publish_info->emplace<fb2::Publisher>().first.get()->emplace<fb2::FB2TextElement>("publisher");
    publish_info->emplace<fb2::ISBN>().first.get()->emplace<fb2::FB2TextElement>("isbn");


    elasticsearch::book::model::fb2::to_model_data b2m;
    txml::StdoutTracer std_tracer;
    book_package.format_serialize(b2m, std_tracer);

    // postproc
    b2m.data_model->emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19});
    b2m.data_model->emplace<elasticsearch::common_model::OriginalPath>("test_path");

    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", *b2m.data_model,
                                curl_verbose(), std_tracer));

    create::transaction::response ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr.node<::model::Result>());
    ASSERT_EQ(ans_ptr.node<::model::Result>()->value(), "created");
    ASSERT_TRUE(ans_ptr.node<::model::_Version>());
    ASSERT_EQ(ans_ptr.node<::model::_Version>()->value(), 1);
    ASSERT_TRUE(ans_ptr.node<::model::_Id>());
    ASSERT_EQ(ans_ptr.node<::model::_Id>()->value(), "1");
    ASSERT_TRUE(ans_ptr.node<::model::_Index>());
    ASSERT_EQ(ans_ptr.node<::model::_Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr.node<::model::_Type>());
    ASSERT_EQ(ans_ptr.node<::model::_Type>()->value(), "_doc");



    get::transaction get_book(get_host());
    ASSERT_NO_THROW(get_book.execute(get_index() + "/_doc/1", curl_verbose()));
    get::transaction::response return_model;
    ASSERT_NO_THROW(return_model = get_book.get_response(std_tracer));


    ASSERT_EQ(return_model.node<elasticsearch::common_model::OriginalPath>()->value(),
              b2m.data_model->node<elasticsearch::common_model::OriginalPath>()->value());
    ASSERT_EQ(return_model.node<elasticsearch::common_model::BinaryBlob>()->value(),
              b2m.data_model->node<elasticsearch::common_model::BinaryBlob>()->value());
    ASSERT_EQ(return_model.node<elasticsearch::common_model::Format>()->value(),
              b2m.data_model->node<elasticsearch::common_model::Format>()->value());

    ASSERT_EQ(return_model.node<elasticsearch::book::model::element::Contributor>()->value(),
              publish_info->node<fb2::Publisher>()->value());
    ASSERT_EQ(return_model.node<elasticsearch::book::model::element::Creator>()->value(),
             author_str);
    ASSERT_EQ(return_model.node<elasticsearch::book::model::element::Identifier>()->value(),
             publish_info->node<fb2::ISBN>()->value());
    ASSERT_EQ(return_model.node<elasticsearch::book::model::element::Language>()->value(),
             title_info->node<fb2::Language>()->value());
    ASSERT_EQ(return_model.node<elasticsearch::book::model::element::Title>()->value(),
             title_info->node<fb2::BookTitle>()->value());
}
}
