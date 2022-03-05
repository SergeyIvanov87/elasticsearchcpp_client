#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/v7_10/request/put_data.hpp"
#include "elasticsearch/v7_10/request/delete_data.hpp"
#include "tests/common/Serializers.hpp"
#include "tests/common/Deserializers.hpp"

#include <gtest/gtest.h>

namespace tests
{
class DataDeleteFixture : public ::testing::Test,
                          public Settings
{
protected:

    void SetUp() override {
        // create index must be done before others
        using namespace elasticsearch::v7::index_mapping;
        transaction req(get_host(), transaction::Tag<StubLeafNode, StubLeafNodeSerializer> {});
        ASSERT_NO_THROW(req.execute(get_index(), false));

        std::shared_ptr<response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->getValue<model::Ack>());
        ASSERT_TRUE(ans_ptr->getValue<model::Ack>()->getValue());
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), false);
    }
};

TEST_F(DataDeleteFixture, request_delete)
{
    using namespace elasticsearch::v7;

    StubLeafNode data_to_send("data_to_send");
    txml::StdoutTracer tracer;

    {
        put_json_data::transaction::Tag<StubLeafNode, StubLeafNodeSerializerMixin> t(data_to_send);
        put_json_data::transaction req(get_host());
        ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", t, curl_verbose(), tracer));

        std::shared_ptr<put_json_data::response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->getValue<model::Result>());
        ASSERT_EQ(ans_ptr->getValue<model::Result>()->getValue(), "created");
    }

    //delete it
    delete_data::transaction del(get_host());
    ASSERT_NO_THROW(del.execute(get_index() + "/_doc/1", curl_verbose()));
    std::shared_ptr<delete_data::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = del.get_response());
    ASSERT_TRUE(ans_ptr->getValue<model::Result>());
    ASSERT_EQ(ans_ptr->getValue<model::Result>()->getValue(), "deleted");
}

}
