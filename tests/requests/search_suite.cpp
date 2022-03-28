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

struct CtorTracer {
    static size_t created;
    static size_t copy_created;
    static size_t move_created;
    static size_t moved;
    static size_t copied;
    static size_t destroyed;
    CtorTracer(std::string s) {
        created ++;
        impl = s;
    }
    CtorTracer(const CtorTracer&src) {
        copy_created ++;
        impl = src.impl;
    }
    CtorTracer(CtorTracer&&src) {
        move_created ++;
        impl = src.impl;
    }
    CtorTracer& operator=(const CtorTracer&src) {
        copied ++;
        impl = src.impl;
        return *this;
    }
    CtorTracer& operator=(CtorTracer&&src) {
        moved ++;
        impl = src.impl;
        return *this;
    }
    ~CtorTracer() {
        destroyed++;
    }

    std::string impl;
};
size_t CtorTracer::created = 0;
size_t CtorTracer::copy_created = 0;
size_t CtorTracer::move_created = 0;
size_t CtorTracer::moved = 0;
size_t CtorTracer::copied = 0;
size_t CtorTracer::destroyed = 0;
TEST(TagHelperTest, value_based_helper)
{
    using namespace elasticsearch::v7::search::tag;
    {
        auto a = elasticsearch::v7::search::tag::details::CArg<CtorTracer, Term>(CtorTracer{"string"});
        (void)a;
    }
    ASSERT_EQ(CtorTracer::created, 1);
    EXPECT_EQ(CtorTracer::move_created, 1);
    ASSERT_EQ(CtorTracer::copied, 0);
    EXPECT_EQ(CtorTracer::destroyed, 2);

    {
        CtorTracer::created = 0;;
        CtorTracer::move_created = 0;
        CtorTracer::copied = 0;
        CtorTracer::destroyed = 0;
        auto a = make<Term, CtorTracer>(CtorTracer{"string"});
        EXPECT_EQ(a.m_arg.impl, std::string("string"));
    }
    ASSERT_EQ(CtorTracer::created, 1);
    EXPECT_EQ(CtorTracer::move_created, 0);
    ASSERT_EQ(CtorTracer::copied, 0);
    EXPECT_EQ(CtorTracer::destroyed, 1);

}

TEST(MustTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param_0 = create::must_tag<StubModel>(make<Term, StubLeafNode_bool>(true),
                                                    make<Term, StubLeafNode_int>(11),
                                                    make<Term, StubLeafNode_string>(std::string("my_string_0")));
    auto must_param_1 = create::must_tag<StubModel>(make<Term, StubLeafNode_bool>(false),
                                                    make<Term, StubLeafNode_int>(22),
                                                    make<Term, StubLeafNode_string>(std::string("my_string_1")));

    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    must_param_0.serialize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]})");

    nlohmann::json node_1 = nlohmann::json::object();
    must_param_1.serialize(node_1, tracer);
    std::cout << node_1.dump();
    ASSERT_EQ(node_1.dump(), R"({"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_1"}},{"term":{"test_stub_model.test_stub_leaf_int":22}},{"term":{"test_stub_model.test_stub_leaf_bool":false}}]})");
}

TEST(MustTagTermTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param_0 = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                    make<Term>(StubLeafNode_int{11}),
                                                    make<Term>(StubLeafNode_string{std::string("my_string_0")}));

    auto must_param_1 = create::must_tag<StubModel>(make<Terms>(StubLeafNode_bool{false}),
                                                    make<Terms>(StubLeafNode_int{22}),
                                                    make<Terms>(StubLeafNode_string{std::string("my_string_1")}));

    auto must_param_2 = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                    make<Terms>(StubLeafNode_int{33}),
                                                    make<Term>(StubLeafNode_string{std::string("my_string_2")}));



    txml::StdoutTracer tracer;

    nlohmann::json node_0 = nlohmann::json::object();
    must_param_0.serialize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]})");

    nlohmann::json node_1 = nlohmann::json::object();
    must_param_1.serialize(node_1, tracer);
    std::cout << node_1.dump();
    ASSERT_EQ(node_1.dump(), R"({"must":[{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_1"}},{"terms":{"test_stub_model.test_stub_leaf_int":22}},{"terms":{"test_stub_model.test_stub_leaf_bool":false}}]})");

    nlohmann::json node_2 = nlohmann::json::object();
    must_param_2.serialize(node_2, tracer);
    std::cout << node_2.dump();
    ASSERT_EQ(node_2.dump(), R"({"must":[{"terms":{"test_stub_model.test_stub_leaf_int":33}},{"term":{"test_stub_model.test_stub_leaf_string":"my_string_2"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]})");
}

template<class Parent>
TXML_PREPARE_SERIALIZER_DISPATCHABLE_CLASS(CustomModelSerializer, Parent, ToJSON,
                                                  ::model::must::ElementToQuery<CustomModel, StubLeafNode_bool>,
                                                  ::model::must::ElementToQuery<CustomModel, StubLeafNode_int>,
                                                  ::model::must::ElementToQuery<CustomModel, StubLeafNode_string>,
                                                  ::model::must::ElementToQuery<CustomModel, CustomNode>)
{
    TXML_SERIALIZER_DISPATCHABLE_OBJECT

    template<class Tracer>
    void serialize_impl(const ::model::must::ElementToQuery<CustomModel, CustomNode> &val, Tracer tracer)
    {
        this->json_object_stack_helper->push(nlohmann::json::object({{::model::must::ElementToQuery<CustomModel, CustomNode>::class_name(),"aha!!!"}}));
    }
};


TEST(MustTagCustomTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param_0 = create::must_tag<CustomModel>(make<Term, StubLeafNode_bool>(true),
                                                      make<Term, StubLeafNode_int>(11),
                                                      make<Term, StubLeafNode_string>(std::string("my_string_0")),
                                                      make<Term, CustomNode>(CustomStruct{}));
    auto must_param_1 = create::must_tag<CustomModel>(make<Term, StubLeafNode_bool>(false),
                                                      make<Term, StubLeafNode_int>(22),
                                                      make<Term, StubLeafNode_string>(std::string("my_string_1")),
                                                      make<Term, CustomNode>(CustomStruct{}));

    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    must_param_0.custom_serialize<CustomModelSerializer>(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"must":[{"term":{"test_custom_model.test_custom_leaf":"aha!!!"}},{"term":{"test_custom_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_custom_model.test_stub_leaf_int":11}},{"term":{"test_custom_model.test_stub_leaf_bool":true}}]})");

    nlohmann::json node_1 = nlohmann::json::object();
    must_param_1.custom_serialize<CustomModelSerializer>(node_1, tracer);
    std::cout << node_1.dump();
    ASSERT_EQ(node_1.dump(), R"({"must":[{"term":{"test_custom_model.test_custom_leaf":"aha!!!"}},{"term":{"test_custom_model.test_stub_leaf_string":"my_string_1"}},{"term":{"test_custom_model.test_stub_leaf_int":22}},{"term":{"test_custom_model.test_stub_leaf_bool":false}}]})");
}

TEST(BooleanFromMustTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));

    auto boolean_param = create::boolean_tag<StubModel>(must_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    boolean_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]}})");
}

TEST(BooleanFromMustNFilterTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));

    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    boolean_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"filter":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_filter"}},{"term":{"test_stub_model.test_stub_leaf_int":22}},{"term":{"test_stub_model.test_stub_leaf_bool":false}}],"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]}})");
}

TEST(QueryTagWithBooleanFromMustNFilterTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));
    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    auto query_param = create::query_tag<StubModel>(boolean_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    query_param.serialize(node, tracer);
    std::cout << node.dump();
}


TEST(SimpleQueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>();

    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    sqt_param.serialize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"]}})");
}

TEST(SimpleQueryTagWithQueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>();
    auto query_param = create::query_tag<StubModel>(sqt_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"]}}})");
}
TEST(SimpleQueryTagWith2QueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int>();
    auto sqt_param_2 = create::simple_query_string_tag<StubModel, StubLeafNode_string>();
    auto query_param = create::query_tag<StubModel>(sqt_param, sqt_param_2);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"]}}})");
}

TEST(SimpleQueryTagWithMustTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>();

    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto boolean_param = create::boolean_tag<StubModel>(must_param);
    auto query_param = create::query_tag<StubModel>(boolean_param, sqt_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"]}}})");
}

TEST(SimpleQueryTagWithMustFilterTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>();

    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));
    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    auto query_param = create::query_tag<StubModel>(boolean_param, sqt_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"filter":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_filter"}},{"term":{"test_stub_model.test_stub_leaf_int":22}},{"term":{"test_stub_model.test_stub_leaf_bool":false}}],"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"]}}})");
}
/*
TEST(QueryTagWithBooleanFromMustNFilterTagCustomSerializerTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto must_param = create::must_tag<CustomModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string, CustomNode>(true, 11, std::string("my_string_0"), CustomStruct {});
    auto filter_param = create::filter_tag<CustomModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string, CustomNode>(false, 22, std::string("my_string_filter"), CustomStruct {});
    auto boolean_param = create::boolean_tag<CustomModel>(must_param, filter_param);
    auto query_param = create::query_tag<CustomModel>(boolean_param);
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;

    query_param.template custom_serialize<CustomModelSerializer>(node, tracer);
    std::cout << node.dump();
}
*/

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
    auto must_param = search::tag::create::must_tag<StubModel>(search::tag::make<search::tag::Term>(StubLeafNode_bool{true}),
                                                               search::tag::make<search::tag::Term>(StubLeafNode_int{11}),
                                                               search::tag::make<search::tag::Term>(StubLeafNode_string{std::string("my_string_0")}));
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
