#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/books/request/book_index_mapping.hpp"
#include "elasticsearch/books/data_model/model.hpp"
#include "elasticsearch/books/data_model/serializers/schema_serializer.hpp"
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
    using namespace elasticsearch::book::index_mapping;
    transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index(), curl_verbose()));

    transaction::response ans_ptr;
    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = req.get_response());

    ASSERT_TRUE(ans_ptr.node<model::Ack>());
    ASSERT_EQ(ans_ptr.node<model::Ack>()->value(), true);
    ASSERT_TRUE(ans_ptr.node<model::Index>());
    ASSERT_EQ(ans_ptr.node<model::Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr.node<model::ShardsAck>());
}
}
