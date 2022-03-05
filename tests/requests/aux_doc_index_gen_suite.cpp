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

TEST_F(DocIndexFixture, request_udated_version)
{
    using namespace elasticsearch::aux::create_doc_id;
    transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/0", curl_verbose()));

    std::shared_ptr<transaction::response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    unsigned int requested_version = 1;
    ASSERT_TRUE(ans_ptr->getValue<::model::Result>());
    ASSERT_EQ(ans_ptr->getValue<::model::Result>()->getValue(), "created");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Version>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Version>()->getValue(), requested_version++);
    ASSERT_TRUE(ans_ptr->getValue<::model::_Id>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Id>()->getValue(), "0");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Index>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<::model::_Type>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Type>()->getValue(), "_doc");


    for (size_t i = 0; i < 100; i++)
    {
        ASSERT_NO_THROW(req.execute(get_index() + "/_doc/0", curl_verbose()));
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->getValue<::model::Result>());
        ASSERT_EQ(ans_ptr->getValue<::model::Result>()->getValue(), "updated");
        ASSERT_EQ(ans_ptr->getValue<::model::_Version>()->getValue(), requested_version++);
    }
}
}
