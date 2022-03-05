#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/images/request/image_index_mapping.hpp"
#include "elasticsearch/images/data_model/model.hpp"
#include "elasticsearch/images/data_model/serializers/schema_serializer.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"

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


TEST_F(IMFixture, request)
{
    using namespace elasticsearch::image::index_mapping;
    txml::StdoutTracer tracer;
    transaction req(get_host(), tracer);
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));

    std::shared_ptr<transaction::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    ASSERT_TRUE(ans_ptr->getValue<model::Ack>());
    ASSERT_EQ(ans_ptr->getValue<model::Ack>()->getValue(), true);
    ASSERT_TRUE(ans_ptr->getValue<model::Index>());
    ASSERT_EQ(ans_ptr->getValue<model::Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<model::ShardsAck>());
}
}
