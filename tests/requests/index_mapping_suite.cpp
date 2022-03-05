#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
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

    std::shared_ptr<index_mapping_delete::response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = delete_index_mapping.get_response(tracer));

    ASSERT_FALSE(ans_ptr->getValue<model::Ack>());

    // Check Error
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::Index>());
    ASSERT_EQ(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::ResourceId>());
    ASSERT_EQ(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::ResourceId>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::ResourceType>());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::Type>());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::Reason>());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<model::IndexUUID>());
    ASSERT_TRUE(ans_ptr->getValue<index_mapping_delete::Error>()->getValue<index_mapping_delete::RootCause>());

    // Check RC
    std::shared_ptr<index_mapping_delete::RootCause> rc =
                        ans_ptr->getValue<index_mapping_delete::Error>()->getValue<index_mapping_delete::RootCause>();
    ASSERT_EQ(rc->getValue().size(), 1);
    const std::vector<std::shared_ptr<typename index_mapping_delete::RootCause::element_t>> &rc_array = rc->getValue();
    std::shared_ptr<typename index_mapping_delete::RootCause::element_t> rc_elem = *(rc_array.begin());
    ASSERT_TRUE(rc_elem->getValue<model::Index>());
    ASSERT_EQ(rc_elem->getValue<model::Index>()->getValue(), get_index());
    ASSERT_TRUE(rc_elem->getValue<model::Reason>());
    ASSERT_TRUE(rc_elem->getValue<model::IndexUUID>());
    ASSERT_TRUE(rc_elem->getValue<model::Type>());
    ASSERT_TRUE(rc_elem->getValue<model::ResourceId>());
    ASSERT_EQ(rc_elem->getValue<model::ResourceId>()->getValue(), get_index());
    ASSERT_TRUE(rc_elem->getValue<model::ResourceType>());
}

TEST_F(IMFixture, request_with_duplicate)
{
    using namespace elasticsearch::v7::index_mapping;
    transaction req(get_host(), transaction::Tag<StubLeafNode, StubLeafNodeSerializer> {}, true);
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));

    std::shared_ptr<response> ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    ASSERT_TRUE(ans_ptr->getValue<model::Ack>());
    ASSERT_TRUE(ans_ptr->getValue<model::Ack>()->getValue());

    ASSERT_TRUE(ans_ptr->getValue<model::Index>());
    ASSERT_EQ(ans_ptr->getValue<model::Index>()->getValue(), get_index());

    ASSERT_TRUE(ans_ptr->getValue<model::ShardsAck>());

    // repeat it again to get bad response
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));
    ASSERT_NO_THROW(ans_ptr = req.get_response(tracer));

    // Check Ack
    ASSERT_FALSE(ans_ptr->getValue<model::Ack>());

    // Check status
    ASSERT_TRUE(ans_ptr->getValue<model::Status>());
    ASSERT_EQ(ans_ptr->getValue<model::Status>()->getValue(), 400);

    // Check Error
    ASSERT_TRUE(ans_ptr->getValue<Error>());
    ASSERT_TRUE(ans_ptr->getValue<Error>()->getValue<model::Index>());
    ASSERT_EQ(ans_ptr->getValue<Error>()->getValue<model::Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<Error>()->getValue<model::Type>());
    ASSERT_TRUE(ans_ptr->getValue<Error>()->getValue<model::Reason>());
    ASSERT_TRUE(ans_ptr->getValue<Error>()->getValue<model::IndexUUID>());
    ASSERT_TRUE(ans_ptr->getValue<Error>()->getValue<RootCause>());

    // Check RC
    std::shared_ptr<RootCause> rc =
                        ans_ptr->getValue<Error>()->getValue<RootCause>();
    ASSERT_EQ(rc->getValue().size(), 1);
    const std::vector<std::shared_ptr<typename RootCause::element_t>> &rc_array = rc->getValue();
    std::shared_ptr<typename RootCause::element_t> rc_elem = *(rc_array.begin());
    ASSERT_TRUE(rc_elem->getValue<model::Index>());
    ASSERT_EQ(rc_elem->getValue<model::Index>()->getValue(), get_index());
    ASSERT_TRUE(rc_elem->getValue<model::Reason>());
    ASSERT_TRUE(rc_elem->getValue<model::IndexUUID>());
    ASSERT_TRUE(rc_elem->getValue<model::Type>());
}
}
