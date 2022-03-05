#include "tests/common/settings.hpp"
#include "elasticsearch/v7_10/request/create_pit.hpp"
#include "elasticsearch/v7_10/request/delete_pit.hpp"
#include "elasticsearch/v7_10/request/index_mapping.hpp"
#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/v7_10/request/search.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/MatchAll.h"
#include "tests/common/Serializers.hpp"


#include "tests/common/Nodes.hpp"
#include <gtest/gtest.h>


namespace tests
{

TEST(MustTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param_0 = create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int,StubLeafNode_string>(
                                                    true, 11, std::string("my_string_0"));
    auto must_param_1 = create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(
                                                    false, 22, std::string("my_string_1"));

    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    must_param_0.serialize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"must":[{"term":{"test_stub_leaf_bool.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_leaf_bool.test_stub_leaf_int":11}},{"term":{"test_stub_leaf_bool":true}}]})");

    nlohmann::json node_1 = nlohmann::json::object();
    must_param_1.serialize(node_1, tracer);
    std::cout << node_1.dump();
    ASSERT_EQ(node_1.dump(), R"({"must":[{"term":{"test_stub_leaf_bool.test_stub_leaf_string":"my_string_1"}},{"term":{"test_stub_leaf_bool.test_stub_leaf_int":22}},{"term":{"test_stub_leaf_bool":false}}]})");
}

TEST(BooleanFromMustTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(true, 11, std::string("my_string_0"));

    auto boolean_param = create::boolean_tag<StubModel>(must_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    boolean_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"must":[{"term":{"test_stub_leaf_bool.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_leaf_bool.test_stub_leaf_int":11}},{"term":{"test_stub_leaf_bool":true}}]}})");
}

TEST(BooleanFromMustNFilterTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(true, 11, std::string("my_string_0"));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));

    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    boolean_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"filter":[{"term":{"test_stub_leaf_bool.test_stub_leaf_string":"my_string_filter"}},{"term":{"test_stub_leaf_bool.test_stub_leaf_int":22}},{"term":{"test_stub_leaf_bool":false}}],"must":[{"term":{"test_stub_leaf_bool.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_leaf_bool.test_stub_leaf_int":11}},{"term":{"test_stub_leaf_bool":true}}]}})");
}

TEST(QueryTagWithBooleanFromMustNFilterTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(true, 11, std::string("my_string_0"));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));
    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    auto query_param = create::query_tag<StubModel>(boolean_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    query_param.serialize(node, tracer);
    std::cout << node.dump();
}


class SearchTagFixtureMatchAll : public ::testing::Test,
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

TEST_F(SearchTagFixtureMatchAll, request_create_match_all)
{
    using namespace elasticsearch::v7;
    create_pit::transaction construct(get_host());
    ASSERT_NO_THROW(construct.execute(get_index(), std::chrono::seconds(10), curl_verbose()));
    model::PIT pit = construct.get_pit();
    ASSERT_TRUE(pit.getValue<model::Id>());

    txml::StdoutTracer tracer;
    search::transaction s(get_host());
    ASSERT_NO_THROW(s.execute("", 1000, curl_verbose(), tracer));
    std::shared_ptr<search::response<StubLeafNode>> search_ans_ptr;
    ASSERT_NO_THROW(search_ans_ptr = s.get_response<StubLeafNode>(tracer));

    ASSERT_NO_THROW(s.execute(1000, pit, curl_verbose()));
    ASSERT_NO_THROW(search_ans_ptr = s.get_response<StubLeafNode>(tracer));

    delete_pit::transaction destroy(get_host());
    ASSERT_NO_THROW(destroy.execute(*pit.getValue<model::Id>(), curl_verbose()));

}

class SearchTagFixtureComplex : public ::testing::Test,
                                public Settings
{
protected:

    void SetUp() override {
        // create index must be done before to generate pit
        using namespace elasticsearch::v7::index_mapping;
        transaction req(get_host(), transaction::Tag<StubModel, StubModelSerializer> {});
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

TEST_F(SearchTagFixtureComplex, request_create_match_all)
{
    using namespace elasticsearch::v7;
    create_pit::transaction construct(get_host());
    ASSERT_NO_THROW(construct.execute(get_index(), std::chrono::seconds(10), curl_verbose()));
    model::PIT pit = construct.get_pit();
    ASSERT_TRUE(pit.getValue<model::Id>());

    txml::StdoutTracer tracer;
    auto must_param = search::tag::create::must_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(true, 11, std::string("my_string_0"));
    auto filter_param = search::tag::create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));
    auto boolean_param = search::tag::create::boolean_tag<StubModel>(must_param, filter_param);

    search::transaction s(get_host());
    ASSERT_NO_THROW(s.execute(/*get_index() + */"",
                              1000,
                              search::tag::create::query_tag<StubModel>(boolean_param),
                              curl_verbose(), tracer));
    std::shared_ptr<search::response<StubLeafNode>> search_ans_ptr;
    ASSERT_NO_THROW(search_ans_ptr = s.get_response<StubLeafNode>(tracer));

    ASSERT_NO_THROW(s.execute(1000, pit,
                              search::tag::create::query_tag<StubModel>(boolean_param),
                              curl_verbose()));
    ASSERT_NO_THROW(search_ans_ptr = s.get_response<StubLeafNode>(tracer));

    delete_pit::transaction destroy(get_host());
    ASSERT_NO_THROW(destroy.execute(*pit.getValue<model::Id>(), curl_verbose()));
}
}
