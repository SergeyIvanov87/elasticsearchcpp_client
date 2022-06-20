#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/aux/doc_id_pm.hpp"
#include "elasticsearch/aux/doc_id_create.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include <gtest/gtest.h>

namespace tests
{
class DocIndexFixture : public ::testing::Test,
                  public Settings
{
protected:

    void SetUp() override {
        using namespace elasticsearch::aux::index_mapping;
        transaction req(get_host());
        ASSERT_NO_THROW(req.execute(get_index(), false));

        std::optional<transaction::response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->node<model::Ack>());
        ASSERT_EQ(ans_ptr->value<model::Ack>().value(), true);
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), curl_verbose());
    }
};

TEST_F(DocIndexFixture, request_udated_version)
{
    using namespace elasticsearch::aux::create_doc_id;
    transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/0", curl_verbose()));

    std::optional<transaction::response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response(tracer));
//TODO remove extra method value() because optional has implicit type cast to underlyed type
    unsigned int requested_version = 1;
    ASSERT_TRUE(ans_ptr->node<::model::Result>());
    ASSERT_EQ(ans_ptr->value<::model::Result>().value(), "created");
    ASSERT_TRUE(ans_ptr->node<::model::_Version>());
    ASSERT_EQ(ans_ptr->value<::model::_Version>().value(), requested_version++);
    ASSERT_TRUE(ans_ptr->node<::model::_Id>());
    ASSERT_EQ(ans_ptr->value<::model::_Id>().value(), "0");
    ASSERT_TRUE(ans_ptr->node<::model::_Index>());
    ASSERT_EQ(ans_ptr->value<::model::_Index>().value(), get_index());
    ASSERT_TRUE(ans_ptr->node<::model::_Type>());
    ASSERT_EQ(ans_ptr->value<::model::_Type>().value(), "_doc");


    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_NO_THROW(req.execute(get_index() + "/_doc/0", curl_verbose()));
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->node<::model::Result>());
        ASSERT_EQ(ans_ptr->value<::model::Result>().value(), "updated");
        ASSERT_EQ(ans_ptr->value<::model::_Version>().value(), requested_version++);
    }
}
}
