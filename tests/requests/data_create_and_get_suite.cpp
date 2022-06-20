#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/v7_10/request/put_data.hpp"
#include "elasticsearch/v7_10/request/get_data.hpp"
#include "tests/common/Serializers.hpp"
#include "tests/common/Deserializers.hpp"

#include <gtest/gtest.h>

namespace tests
{
class DataCreateGetFixture : public ::testing::Test,
                             public Settings
{
protected:

    void SetUp() override {
        // create index must be done before others
        using namespace elasticsearch::v7::index_mapping;
        transaction req(get_host(), transaction::Tag<StubLeafNode, StubLeafNodeSerializer> {});
        ASSERT_NO_THROW(req.execute(get_index(), false));

        std::optional<response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->node<model::Ack>());
        ASSERT_TRUE(ans_ptr->value<model::Ack>().value());
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), false);
    }
};

TEST_F(DataCreateGetFixture, request_create)
{
    using namespace elasticsearch::v7;

    StubLeafNode data_to_send("data_to_send");
    txml::StdoutTracer tracer;

    put_json_data::transaction req(get_host(), true);
    put_json_data::transaction::Tag<StubLeafNode, StubLeafNodeSerializerMixin> t(data_to_send);
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1/_create", t, curl_verbose(), tracer));

    std::optional<put_json_data::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response());
    ASSERT_TRUE(ans_ptr->node<model::Result>());
    ASSERT_EQ(ans_ptr->value<model::Result>().value(), "created");
    ASSERT_TRUE(ans_ptr->node<model::_Version>());
    ASSERT_EQ(ans_ptr->value<model::_Version>().value(), 1);
    ASSERT_TRUE(ans_ptr->node<model::_Id>());
    ASSERT_EQ(ans_ptr->value<model::_Id>().value(), "1");
    ASSERT_TRUE(ans_ptr->node<model::_Index>());
    ASSERT_EQ(ans_ptr->value<model::_Index>().value(), get_index());
    ASSERT_TRUE(ans_ptr->node<model::_Type>());
    ASSERT_EQ(ans_ptr->value<model::_Type>().value(), "_doc");

    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", t, curl_verbose(), tracer));
    ASSERT_NO_THROW(ans_ptr = req.get_response());
    ASSERT_TRUE(ans_ptr->node<model::Result>());
    ASSERT_EQ(ans_ptr->value<model::Result>().value(), "updated");
    ASSERT_TRUE(ans_ptr->node<model::_Version>());
    ASSERT_EQ(ans_ptr->value<model::_Version>().value(), 2);
}

TEST_F(DataCreateGetFixture, request_create_n_get_dynamic_model)
{
    using namespace elasticsearch::v7;

    StubLeafNode data_to_send("data_to_send");
    txml::StdoutTracer tracer;
    put_json_data::transaction::Tag<StubLeafNode, StubLeafNodeSerializerMixin> t(data_to_send);
    put_json_data::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", t, curl_verbose(), tracer));

    std::optional<put_json_data::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response());
    ASSERT_TRUE(ans_ptr->node<model::Result>());
    ASSERT_EQ(ans_ptr->value<model::Result>().value(), "created");

    get_json_data::transaction get(get_host());
    ASSERT_NO_THROW(get.execute(get_index() + "/_doc/1", curl_verbose()));

    std::optional<get_json_data::response<StubLeafNode>> resp_ptr;
    resp_ptr = get.get_response<StubLeafNode, StubLeafNodeDeserializer>(tracer);
    ASSERT_TRUE(resp_ptr->node<model::_Id>());
    ASSERT_TRUE(resp_ptr->node<model::_Index>());
    ASSERT_TRUE(resp_ptr->node<model::_Type>());
    ASSERT_TRUE(resp_ptr->node<model::_SeqNo>());
    ASSERT_TRUE(resp_ptr->node<model::_PrimaryTerm>());
    ASSERT_TRUE(resp_ptr->node<model::Found>());
    ASSERT_EQ(resp_ptr->value<model::Found>().value(), true);
    ASSERT_TRUE(resp_ptr->node<model::_Version>());
    ASSERT_EQ(resp_ptr->value<model::_Version>().value(), 1);
    ASSERT_TRUE(resp_ptr->node<model::_Source<StubLeafNode>>());
    ASSERT_TRUE(resp_ptr->node<model::_Source<StubLeafNode>>()->node<StubLeafNode>());
    ASSERT_EQ(resp_ptr->node<model::_Source<StubLeafNode>>()->node<StubLeafNode>()->value(), "data_to_send");
}
}
