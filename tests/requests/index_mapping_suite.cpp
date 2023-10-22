#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/v7_10/request/index_mapping_update.hpp"
#include "tests/common/Serializers.hpp"
#include "tests/common/Deserializers.hpp"

#include <gtest/gtest.h>

namespace tests
{
class IMFixture : public ::testing::Test,
                  public Settings
{
protected:

    void SetUp() override {
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), curl_verbose());
    }
};


TEST_F(IMFixture, delete_failed)
{
    using namespace elasticsearch::v7;

    index_mapping_delete::transaction delete_index_mapping(get_host());
    ASSERT_NO_THROW(delete_index_mapping.execute(get_index(), curl_verbose()));

    std::optional<index_mapping_delete::response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = delete_index_mapping.get_response(tracer));

    ASSERT_FALSE(ans_ptr->node<model::Ack>());

    // Check Error
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::Index>());
    ASSERT_EQ(ans_ptr->node<index_mapping_delete::Error>()->node<model::Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::ResourceId>());
    ASSERT_EQ(ans_ptr->node<index_mapping_delete::Error>()->node<model::ResourceId>()->value(), get_index());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::ResourceType>());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::Type>());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::Reason>());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<model::IndexUUID>());
    ASSERT_TRUE(ans_ptr->node<index_mapping_delete::Error>()->node<index_mapping_delete::RootCause>());

    // Check RC
    std::optional<index_mapping_delete::RootCause> rc =
                        ans_ptr->node<index_mapping_delete::Error>()->node<index_mapping_delete::RootCause>();
    ASSERT_EQ(rc->value().size(), 1);
    const std::vector<std::optional<typename index_mapping_delete::RootCause::element_t>> &rc_array = rc->value();
    std::optional<typename index_mapping_delete::RootCause::element_t> rc_elem = *(rc_array.begin());
    ASSERT_TRUE(rc_elem->node<model::Index>());
    ASSERT_EQ(rc_elem->node<model::Index>()->value(), get_index());
    ASSERT_TRUE(rc_elem->node<model::Reason>());
    ASSERT_TRUE(rc_elem->node<model::IndexUUID>());
    ASSERT_TRUE(rc_elem->node<model::Type>());
    ASSERT_TRUE(rc_elem->node<model::ResourceId>());
    ASSERT_EQ(rc_elem->node<model::ResourceId>()->value(), get_index());
    ASSERT_TRUE(rc_elem->node<model::ResourceType>());
}

TEST_F(IMFixture, request_with_duplicate)
{
    using namespace elasticsearch::v7::index_mapping;
    transaction req(get_host(), transaction::Tag<StubLeafNode, StubLeafNodeSerializer> {}, true);
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));

    std::optional<response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    ASSERT_TRUE(ans_ptr->node<model::Ack>());
    ASSERT_TRUE(ans_ptr->node<model::Ack>()->value());

    ASSERT_TRUE(ans_ptr->node<model::Index>());
    ASSERT_EQ(ans_ptr->node<model::Index>()->value(), get_index());

    ASSERT_TRUE(ans_ptr->node<model::ShardsAck>());

    // repeat it again to get bad response
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));
    ASSERT_NO_THROW(ans_ptr = req.get_response(tracer));

    // Check Ack
    ASSERT_FALSE(ans_ptr->node<model::Ack>());

    // Check status
    ASSERT_TRUE(ans_ptr->node<model::Status>());
    ASSERT_EQ(ans_ptr->node<model::Status>()->value(), 400);

    // Check Error
    ASSERT_TRUE(ans_ptr->node<Error>());
    ASSERT_TRUE(ans_ptr->node<Error>()->node<model::Index>());
    ASSERT_EQ(ans_ptr->node<Error>()->node<model::Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr->node<Error>()->node<model::Type>());
    ASSERT_TRUE(ans_ptr->node<Error>()->node<model::Reason>());
    ASSERT_TRUE(ans_ptr->node<Error>()->node<model::IndexUUID>());
    ASSERT_TRUE(ans_ptr->node<Error>()->node<RootCause>());

    // Check RC
    const std::optional<RootCause> &rc =
                        ans_ptr->node<Error>()->node<RootCause>();
    ASSERT_EQ(rc->value().size(), 1);
    const std::vector<std::optional<typename RootCause::element_t>> &rc_array = rc->value();
    std::optional<typename RootCause::element_t> rc_elem = *(rc_array.begin());
    ASSERT_TRUE(rc_elem->node<model::Index>());
    ASSERT_EQ(rc_elem->node<model::Index>()->value(), get_index());
    ASSERT_TRUE(rc_elem->node<model::Reason>());
    ASSERT_TRUE(rc_elem->node<model::IndexUUID>());
    ASSERT_TRUE(rc_elem->node<model::Type>());
}


TEST_F(IMFixture, map_and_update)
{
    using namespace elasticsearch::v7;

    // create index
    index_mapping::transaction req(get_host(), index_mapping::transaction::Tag<CustomModel, CustomModelSerializer> {}, true);
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));

    std::optional<index_mapping::response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    ASSERT_TRUE(ans_ptr->node<model::Ack>());
    ASSERT_TRUE(ans_ptr->node<model::Ack>()->value());

    ASSERT_TRUE(ans_ptr->node<model::Index>());
    ASSERT_EQ(ans_ptr->node<model::Index>()->value(), get_index());

    ASSERT_TRUE(ans_ptr->node<model::ShardsAck>());

    //update index
    index_mapping_update::transaction req_update(get_host(), index_mapping_update::transaction::Tag<CustomModelToAdd, CustomModelToAddSerializer> {}, true, tracer);
    ASSERT_NO_THROW(req_update.execute(get_index(), curl_verbose()));

    std::optional<index_mapping_update::response> ans_update_ptr;
    ASSERT_NO_THROW(ans_update_ptr = req_update.get_response());

    ASSERT_TRUE(ans_update_ptr->node<model::Ack>());
    ASSERT_TRUE(ans_update_ptr->node<model::Ack>()->value());
}
}
