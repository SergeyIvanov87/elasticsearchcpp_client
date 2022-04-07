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

//
#include "elasticsearch/v7_10/answer_model/search/object/boolean/new_Bool.h"
#include "elasticsearch/v7_10/answer_model/search/object/full_text/new_QuerySimpleString.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/new_Query.h"

namespace tests
{
using Model = StubModel;

template<class Element>
using MTerm = model::must_new::Term<Model, Element>;

template<class Element>
using MTerms = model::must_new::Terms<Model, Element>;

template<class Element>
using FTerm = model::filter_new::Term<Model, Element>;

template<class ...Elements>
using QSS = model::full_text_new::SimpleQueryString<Model, Elements...>;

TEST(NEW_MustQSS, serializer)
{
    model::MustNew<StubModel, MTerm<StubLeafNode_bool>,
                              MTerm<StubLeafNode_int>,
                              MTerms<StubLeafNode_string>,
                              QSS<StubLeafNode_string>> must_instance(MTerm<StubLeafNode_bool>(true), MTerms<StubLeafNode_string>("my_string_0"),
                                                                      QSS<StubLeafNode_string>("aaaa"));

    typename model::MustNew<StubModel, MTerm<StubLeafNode_bool>,
                              MTerm<StubLeafNode_int>,
                              MTerms<StubLeafNode_string>,
                              QSS<StubLeafNode_string>>::aggregator_serializer_type ser;
    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    must_instance.template format_serialize(ser, tracer);
    ser. template finalize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"must":[{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"aaaa"}},{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]})");
}

TEST(NEW_BooleanMustQSS, serializer)
{
    using MustTag = model::MustNew<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    model::BooleanNew<StubModel, MustTag> bool_instance(must_instance);

    typename model::BooleanNew<StubModel, MustTag>::aggregator_serializer_type ser;
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    bool_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"must":[{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"aaaa"}},{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]}})");
}

TEST(NEW_QueryBooleanQSSMustQSS, serializer)
{
    using MustTag = model::MustNew<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    using BooleanTag = model::BooleanNew<StubModel, MustTag>;
    BooleanTag bool_instance(must_instance);

    using QSSTag = QSS<StubLeafNode_string>;
    QSSTag sqt_param("acdc");

    using QueryTag = model::QueryNew<StubModel, BooleanTag, QSSTag>;
    QueryTag q_instance(bool_instance, sqt_param);
    typename QueryTag::aggregator_serializer_type ser;
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    q_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"must":[{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"aaaa"}},{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"acdc"}}})");
}

TEST(NEW_BooleanMustQSSFilter, init)
{
    using MustTag = model::MustNew<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    using FilterTag = model::FilterNew<StubModel,
                                       FTerm<StubLeafNode_bool>,
                                       FTerm<StubLeafNode_int>,
                                       FTerm<StubLeafNode_string>>;
    FilterTag filter_instance(FTerm<StubLeafNode_bool>(false),
                              FTerm<StubLeafNode_int>(22),
                              FTerm<StubLeafNode_string>("my_string_filter"));

    using BooleanTag = model::BooleanNew<StubModel, MustTag, FilterTag>;
    BooleanTag bool_instance(must_instance, filter_instance);

    using QSSTag = QSS<StubLeafNode_string>;
    QSSTag sqt_param("acdc");

    using QueryTag = model::QueryNew<StubModel, BooleanTag, QSSTag>;
    QueryTag q_instance(bool_instance, sqt_param);
    typename QueryTag::aggregator_serializer_type ser;
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    q_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"filter":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_filter"}},{"term":{"test_stub_model.test_stub_leaf_int":22}},{"term":{"test_stub_model.test_stub_leaf_bool":false}}],"must":[{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"aaaa"}},{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"acdc"}}})");
}

TEST(NEW_Sort, init)
{
    using SortTag = model::SortArray<StubModel, StubLeafNode_bool, StubLeafNode_string>;
    SortTag sort_instance({::model::Order("desc"), ::model::Order("asc")});

    typename SortTag::aggregator_serializer_type ser;
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    sort_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);

    ASSERT_EQ(node.dump(), R"({"sort":[{"test_stub_model.test_stub_leaf_string.keyword":{"order":"desc"}},{"test_stub_model.test_stub_leaf_bool.keyword":{"order":"desc"}}]})");
}

TEST(NEW_QSS, init)
{
    using QSSTag = QSS<StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>;
    QSSTag qss_instance("abba");

    typename QSSTag::aggregator_serializer_type ser;

    txml::StdoutTracer tracer;
    nlohmann::json node;
    qss_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"],"query":"abba"}})");
}



TEST(DISABLED_MustTagTest, init)
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

TEST(DISABLED_MustTagTermTest, init)
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


TEST(DISABLED_MustTagCustomTest, init)
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

TEST(DISABLED_BooleanFromMustTagTest, init)
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

TEST(DISABLED_BooleanFromMustNFilterTagTest, init)
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

TEST(DISABLED_QueryTagWithBooleanFromMustNFilterTagTest, init)
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


TEST(DISABLED_SimpleQueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>("abba");

    txml::StdoutTracer tracer;
    nlohmann::json node_0 = nlohmann::json::object();
    sqt_param.serialize(node_0, tracer);
    ASSERT_EQ(node_0.dump(), R"({"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"],"query":"abba"}})");
}

TEST(DISABLED_SimpleQueryTagWithQueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>("acdc");
    auto query_param = create::query_tag<StubModel>(sqt_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"],"query":"acdc"}}})");
}

TEST(SimpleQueryTagWith2QueryTagTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int>("zztop");
    auto sqt_param_2 = create::simple_query_string_tag<StubModel, StubLeafNode_string>("crimson");
    auto query_param = create::query_tag<StubModel>(sqt_param, sqt_param_2);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"crimson"}}})");
}

TEST(DISABLED_SimpleQueryTagWithMustTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>("rainbow");

    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto boolean_param = create::boolean_tag<StubModel>(must_param);
    auto query_param = create::query_tag<StubModel>(sqt_param, boolean_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    query_param.serialize(node, tracer);
    std::cout << node.dump() << std::endl;
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"],"query":"rainbow"}}})");
}

TEST(DISABLED_SimpleQueryTagWithMustFilterTest, init)
{
    using namespace elasticsearch::v7::search::tag;
    auto sqt_param = create::simple_query_string_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>("wintersun");

    auto must_param = create::must_tag<StubModel>(make<Term>(StubLeafNode_bool{true}),
                                                  make<Term>(StubLeafNode_int{11}),
                                                  make<Term>(StubLeafNode_string{std::string("my_string_0")}));
    auto filter_param = create::filter_tag<StubModel, StubLeafNode_bool, StubLeafNode_int, StubLeafNode_string>(false, 22, std::string("my_string_filter"));
    auto boolean_param = create::boolean_tag<StubModel>(must_param, filter_param);
    auto query_param = create::query_tag<StubModel>(boolean_param, sqt_param);

    txml::StdoutTracer tracer;
    nlohmann::json node = nlohmann::json::object();
    std::cout <<node.dump() << std::endl;
    query_param.serialize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"query":{"bool":{"filter":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_filter"}},{"term":{"test_stub_model.test_stub_leaf_int":22}},{"term":{"test_stub_model.test_stub_leaf_bool":false}}],"must":[{"term":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_int":11}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]},"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string","test_stub_model.test_stub_leaf_int","test_stub_model.test_stub_leaf_bool"],"query":"wintersun"}}})");
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
