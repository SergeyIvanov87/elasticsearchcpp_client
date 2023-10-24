#include <iostream>
#include <thread>

#include <txml/applications/fb2/fb2.hpp>

#include "tests/common/settings.hpp"
#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/request/create_book.hpp"
#include "elasticsearch/books/request/get_book.hpp"
#include "elasticsearch/books/request/search_book.hpp"

#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"
#include "elasticsearch/books/data_model/serializers/serializer.hpp"
#include "elasticsearch/books/data_model/serializers/deserializer.hpp"
#include "elasticsearch/common_model/serializers/deserializer.hpp"

#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"

#include "elasticsearch/v7_10/request/search.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/MatchAll.h"

#include <gtest/gtest.h>


using namespace std::chrono_literals;
namespace tests
{
using namespace json;
template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(MatchAllToJSON, Parent,
                                                  ToJSON,
                                                            ::model::MatchAll)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT
    using json = nlohmann::json;

    template<class Tracer>
    void serialize_impl(const ::model::MatchAll&)
    {
        auto mediator = this->get_shared_mediator_object();
        json element({{::model::MatchAll::class_name(), ""}});
        mediator->push(std::move(element));
    }
};
using namespace json;

class BookCreateSearchFixture : public ::testing::Test,
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

elasticsearch::book::model::data get_test_book_model_by_index(const std::string& index_str)
{
    using namespace elasticsearch::book::model;
    data data_model;
    data_model.emplace<element::Title>("title_search " + index_str + " word" + index_str);
    data_model.emplace<element::Identifier>("identifier_search" + index_str);
    data_model.emplace<element::Contributor>("contributor_search" + index_str);
    data_model.emplace<element::Creator>("creator_search" + index_str);
    data_model.emplace<element::Language>("language_search" + index_str);
    data_model.emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19,
                                        static_cast<uint8_t>(reinterpret_cast<size_t>(index_str.size()) % 256)});
    data_model.emplace<elasticsearch::common_model::OriginalPath>("test_path_search" + index_str);
    data_model.emplace<elasticsearch::common_model::Format>("test_format_search" + index_str);
    return data_model;
}
elasticsearch::book::model::data get_test_book_model_by_index(size_t index) {
    return get_test_book_model_by_index(std::to_string(index));
}

bool test_data_equal(const elasticsearch::book::model::data &lhs, const elasticsearch::book::model::data &rhs)
{
    bool ret = false;
    ret = (lhs.node<elasticsearch::common_model::OriginalPath>()->value() ==
           rhs.node<elasticsearch::common_model::OriginalPath>()->value());
    ret &= (lhs.node<elasticsearch::common_model::BinaryBlob>()->value() ==
            rhs.node<elasticsearch::common_model::BinaryBlob>()->value());
    ret &= (lhs.node<elasticsearch::common_model::Format>()->value() ==
            rhs.node<elasticsearch::common_model::Format>()->value());

    ret &= (lhs.node<elasticsearch::book::model::element::Contributor>()->value() ==
            rhs.node<elasticsearch::book::model::element::Contributor>()->value());
    ret &= (lhs.node<elasticsearch::book::model::element::Creator>()->value() ==
            rhs.node<elasticsearch::book::model::element::Creator>()->value());
    ret &= (lhs.node<elasticsearch::book::model::element::Identifier>()->value() ==
            rhs.node<elasticsearch::book::model::element::Identifier>()->value());
    ret &= (lhs.node<elasticsearch::book::model::element::Language>()->value() ==
            rhs.node<elasticsearch::book::model::element::Language>()->value());
    ret &= (lhs.node<elasticsearch::book::model::element::Title>()->value() ==
            rhs.node<elasticsearch::book::model::element::Title>()->value());
    return ret;
}

TEST_F(BookCreateSearchFixture, create_n_search)
{
    using namespace elasticsearch::book;
    using namespace elasticsearch::book::model;

    size_t index = 11111;
    elasticsearch::book::model::data data_model = get_test_book_model_by_index(index);
    std::string document_id = std::to_string(index);
    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute_force_refresh(get_index() + "/_doc/" + document_id,
                                              data_model, curl_verbose()));

    create::transaction::response ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response());
    ASSERT_TRUE(ans_ptr.node<::model::Result>());
    ASSERT_EQ(ans_ptr.node<::model::Result>()->value(), "created");
    ASSERT_TRUE(ans_ptr.node<::model::_Version>());
    ASSERT_EQ(ans_ptr.node<::model::_Version>()->value(), 1);
    ASSERT_TRUE(ans_ptr.node<::model::_Id>());
    ASSERT_EQ(ans_ptr.node<::model::_Id>()->value(), document_id);
    ASSERT_TRUE(ans_ptr.node<::model::_Index>());
    ASSERT_EQ(ans_ptr.node<::model::_Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr.node<::model::_Type>());
    ASSERT_EQ(ans_ptr.node<::model::_Type>()->value(), "_doc");


    txml::StdoutTracer tracer;
    search::transaction s(get_host());
    ASSERT_NO_THROW(s.execute(get_index(), 10, curl_verbose(), tracer));
    search::transaction::response search_ans_ptr;
    search_ans_ptr = s.get_response(tracer);
    ASSERT_TRUE(search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>());
    const auto &hits = search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>();

    ASSERT_TRUE(hits->node<::model::HitsArray<elasticsearch::book::model::data>>());
    const auto &hits_array = hits->node<::model::HitsArray<elasticsearch::book::model::data>>()->value();
    ASSERT_FALSE(hits_array.empty());

    bool found = false;
    for (const auto &hit : hits_array)
    {
        std::cout << hit->node<::model::_Index>()->value() << std::endl;
        std::cout << hit->node<::model::_Id>()->value() << std::endl;
        std::cout << hit->node<::model::_Score>()->value() << std::endl;
        std::cout << hit->node<::model::_Type>()->value() << std::endl;
        auto source_ptr = hit->node<::model::_Source<elasticsearch::book::model::data>>();
        if (source_ptr)
        {
            const auto &found_model_ptr = source_ptr->node<elasticsearch::book::model::data>();
            if (found_model_ptr && document_id ==  hit->node<::model::_Id>()->value())
            {
                found |= test_data_equal(*found_model_ptr, data_model);
            }
        }
    }
    ASSERT_TRUE(found);
}


template<size_t size>
class BookMultipleCreateSearchFixture : public ::testing::Test,
                                        public Settings
{
public:
    const std::set<std::string>& get_generated_ids() const
    {
        return generated_item_ids;
    }
protected:
    std::set<std::string> generated_item_ids;
    void SetUp() override
    {
        using namespace elasticsearch::book::index_mapping;
        using namespace elasticsearch::book;

        transaction req(get_host());
        ASSERT_NO_THROW(req.execute(get_index(), false));

        transaction::response ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr.node<::model::Ack>());
        ASSERT_EQ(ans_ptr.node<::model::Ack>()->value(), true);

        generate_elements();
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), curl_verbose());

        for (const auto& id : generated_item_ids)
        {
            // TODO delete
        }
    }

    void generate_elements()
    {
        using namespace elasticsearch::book;
        //generate elements
        for (size_t index = 0; index < size; index++)
        {
            elasticsearch::book::model::data data_model = get_test_book_model_by_index(index);
            std::string document_id = std::to_string(index);
            create::transaction req(get_host());
            ASSERT_NO_THROW(req.execute_force_refresh(get_index() + "/_doc/" + document_id,
                                                      data_model, curl_verbose()));

            create::transaction::response create_ans_ptr;
            ASSERT_NO_THROW(create_ans_ptr = req.get_response());
            ASSERT_TRUE(create_ans_ptr.node<::model::Result>());
            ASSERT_EQ(create_ans_ptr.node<::model::Result>()->value(), "created");
            ASSERT_EQ(create_ans_ptr.node<::model::_Id>()->value(), document_id);
            ASSERT_TRUE(create_ans_ptr.node<::model::_Index>());

            // remember to validate
            generated_item_ids.insert(std::to_string(index));
        }
    }
};

using BookMultipleCreateSearchFixture_10 = BookMultipleCreateSearchFixture<10>;
TEST_F(BookMultipleCreateSearchFixture_10, create_n_search_pit)
{
    using namespace elasticsearch::book;
    using namespace elasticsearch::book::model;

    std::set<std::string> inserted_items = this->get_generated_ids();

    txml::StdoutTracer tracer;

    search::transaction s(get_host());

    ASSERT_NO_THROW(s.execute(get_index(), 10, 10s,
                              search::tag::sort<element::Contributor>({::model::Order("desc")}),
                              curl_verbose(), tracer));
    search::transaction::response search_ans_ptr;
    search_ans_ptr = s.get_response(tracer);
    ASSERT_TRUE(search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>());
    const auto &hits = search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>();

    ASSERT_TRUE(hits->node<::model::HitsArray<elasticsearch::book::model::data>>());
    const auto &hits_array = hits->node<::model::HitsArray<elasticsearch::book::model::data>>()->value();
    ASSERT_FALSE(hits_array.empty());

    //std::this_thread::sleep_for(30s);
    bool found = false;
    for (const auto &hit : hits_array)
    {
        std::cout << hit->node<::model::_Index>()->value() << std::endl;
        std::cout << hit->node<::model::_Id>()->value() << std::endl;
        std::cout << hit->node<::model::_Type>()->value() << std::endl;
        const auto &source_ptr = hit->node<::model::_Source<elasticsearch::book::model::data>>();
        if (source_ptr)
        {
            const auto &found_model_ptr = source_ptr->node<elasticsearch::book::model::data>();
            if (found_model_ptr)
            {
                const std::string &id = hit->node<::model::_Id>()->value();
                if (auto inserted_it = inserted_items.find(id); inserted_it != inserted_items.end())
                {
                    auto expected_model = get_test_book_model_by_index(id);
                    ASSERT_TRUE(test_data_equal(*found_model_ptr, expected_model));

                    inserted_items.erase(inserted_it);
                }
            }
        }
    }
    ASSERT_TRUE(inserted_items.empty());
/*
    // insert new elements
    generate_elements();

    // TODO use OLD PIT
    ASSERT_NO_THROW(s.execute("", 10, curl_verbose()));
    search_ans_ptr = s.get_response(tracer);
    ASSERT_TRUE(search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>());
    hits = search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>();

    ASSERT_TRUE(hits->node<::model::HitsArray<elasticsearch::book::model::data>>());
    hits_array = hits->node<::model::HitsArray<elasticsearch::book::model::data>>()->value();
    ASSERT_TRUE(hits_array.empty());
    */
}

using BookMultipleCreateSearchFixture_11 = BookMultipleCreateSearchFixture<11>;
TEST_F(BookMultipleCreateSearchFixture_11, create_n_search_boolean_2_terms)
{
    using namespace elasticsearch::book;
    using namespace elasticsearch::book::model;

    std::set<std::string> inserted_items = this->get_generated_ids();

    txml::StdoutTracer tracer;

    auto mu = search::tag::create::must_tag(element::Creator("creator_search10"),
                                            element::Language("language_search10"));
    auto boo = search::tag::create::boolean_tag(mu);
    search::transaction s(get_host());

    ASSERT_NO_THROW(s.execute(get_index(), 10, 10s,
                              search::tag::create::query_tag(boo).value(),
                              search::tag::sort<element::Contributor> ({::model::Order("desc")}),
                              curl_verbose(),
                              tracer));
    search::transaction::response search_ans_ptr;
    search_ans_ptr = s.get_response(tracer);
    ASSERT_TRUE(search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>());
    const auto &hits = search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>();

    ASSERT_TRUE(hits->node<::model::HitsArray<elasticsearch::book::model::data>>());
    const auto &hits_array = hits->node<::model::HitsArray<elasticsearch::book::model::data>>()->value();
    ASSERT_FALSE(hits_array.empty());

    bool found = false;
    for (const auto &hit : hits_array)
    {
        std::cout << hit->node<::model::_Index>()->value() << std::endl;
        std::cout << hit->node<::model::_Id>()->value() << std::endl;
        std::cout << hit->node<::model::_Type>()->value() << std::endl;
        const auto &source_ptr = hit->node<::model::_Source<elasticsearch::book::model::data>>();
        if (source_ptr)
        {
            const auto &found_model_ptr = source_ptr->node<elasticsearch::book::model::data>();
            if (found_model_ptr)
            {
                ASSERT_EQ(hit->node<::model::_Id>()->value(), "10");
                found = true;
            }
        }
    }
    ASSERT_TRUE(found);
}


TEST_F(BookMultipleCreateSearchFixture_11, create_n_search_query_simple_string)
{
    using namespace elasticsearch::book;
    using namespace elasticsearch::book::model;

    std::set<std::string> inserted_items = this->get_generated_ids();

    txml::StdoutTracer tracer;
    auto qst = search::tag::create::simple_query_string_tag<element::Title>("word1|word10");
    search::transaction s(get_host());

    ASSERT_NO_THROW(s.execute(get_index(), 10, 10s,
                              search::tag::create::query_tag(qst).value(),
                              search::tag::sort<element::Contributor> ({::model::Order("desc")}),
                              curl_verbose(),
                              tracer));
    search::transaction::response search_ans_ptr;
    search_ans_ptr = s.get_response(tracer);
    ASSERT_TRUE(search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>());
    const auto &hits = search_ans_ptr.node<::model::HitsNode<elasticsearch::book::model::data>>();

    ASSERT_TRUE(hits->node<::model::HitsArray<elasticsearch::book::model::data>>());
    const auto &hits_array = hits->node<::model::HitsArray<elasticsearch::book::model::data>>()->value();
    ASSERT_FALSE(hits_array.empty());

    size_t found = 0;
    for (const auto &hit : hits_array)
    {
        std::cout << hit->node<::model::_Index>()->value() << std::endl;
        std::cout << hit->node<::model::_Id>()->value() << std::endl;
        std::cout << hit->node<::model::_Type>()->value() << std::endl;
        const auto &source_ptr = hit->node<::model::_Source<elasticsearch::book::model::data>>();
        if (source_ptr)
        {
            const auto &found_model_ptr = source_ptr->node<elasticsearch::book::model::data>();
            if (found_model_ptr)
            {
                if(hit->node<::model::_Id>()->value() =="1" ||
                  hit->node<::model::_Id>()->value() =="10")
                {
                    found++;
                }
            }
        }
    }
    ASSERT_EQ(found, 2);
}

}
