#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/create_pit.hpp"
#include "elasticsearch/v7_10/request/delete_pit.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "tests/common/Serializers.hpp"

#include <gtest/gtest.h>

namespace tests
{
class PITFixture : public ::testing::Test,
                  public Settings
{
protected:

    void SetUp() override {
        // create index must be done before to generate pit
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

TEST_F(PITFixture, request_create)
{
    using namespace elasticsearch::v7;
    create_pit::transaction construct(get_host());
    ASSERT_NO_THROW(construct.execute(get_index(), std::chrono::seconds(10), curl_verbose()));
    model::PIT pit = construct.get_pit();

    delete_pit::transaction destroy(get_host());
    ASSERT_TRUE(pit.getValue<model::Id>());
    ASSERT_NO_THROW(destroy.execute(*pit.getValue<model::Id>(), curl_verbose()));

    txml::StdoutTracer tracer;
    std::shared_ptr<delete_pit::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = destroy.get_response(tracer));

    ASSERT_TRUE(ans_ptr->getValue<::model::Succeeded>());
    ASSERT_TRUE(ans_ptr->getValue<::model::Succeeded>()->getValue());

    ASSERT_TRUE(ans_ptr->getValue<::model::NumFreed>());
    ASSERT_NE(ans_ptr->getValue<::model::NumFreed>()->getValue(), 0);
}

TEST_F(PITFixture, request_delete_non_existent)
{
    // try delete non-existent PIT
    using namespace elasticsearch::v7;
    model::Id pit_bad(std::string("TESTCREATEDBADPIT"));

    delete_pit::transaction destroy(get_host());
    std::shared_ptr<delete_pit::response> ans_ptr;
    ASSERT_NO_THROW(destroy.execute(pit_bad, curl_verbose()));

    txml::StdoutTracer tracer;
    ASSERT_NO_THROW(ans_ptr = destroy.get_response(tracer));

    ASSERT_FALSE(ans_ptr->getValue<::model::Succeeded>());
    ASSERT_FALSE(ans_ptr->getValue<::model::NumFreed>());

    ASSERT_TRUE(ans_ptr->getValue<::model::Status>());
    ASSERT_EQ(ans_ptr->getValue<::model::Status>()->getValue(), 400);

    ASSERT_TRUE(ans_ptr->getValue<delete_pit::Error>());
    auto error_ptr = ans_ptr->getValue<delete_pit::Error>();
    ASSERT_TRUE(error_ptr->getValue<model::Type>());
    ASSERT_TRUE(error_ptr->getValue<model::Reason>());
    ASSERT_TRUE(error_ptr->getValue<model::CausedBy>());
    ASSERT_TRUE(error_ptr->getValue<model::CausedBy>()->getValue<model::Type>());
    ASSERT_TRUE(error_ptr->getValue<model::CausedBy>()->getValue<model::Reason>());
    ASSERT_TRUE(error_ptr->getValue<delete_pit::RootCause>());
    auto rc_ptr = error_ptr->getValue<delete_pit::RootCause>();
    ASSERT_EQ(rc_ptr->getValue().size(), 1);
    const std::vector<std::shared_ptr<typename delete_pit::RootCause::element_t>> &rc_array = rc_ptr->getValue();
    std::shared_ptr<typename delete_pit::RootCause::element_t> rc_elem = *(rc_array.begin());
    ASSERT_TRUE(rc_elem->getValue<model::Reason>());
    ASSERT_TRUE(rc_elem->getValue<model::Type>());
}
}
