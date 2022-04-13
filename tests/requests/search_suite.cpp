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
#include "elasticsearch/v7_10/answer_model/search/object/boolean/Bool.h"
#include "elasticsearch/v7_10/answer_model/search/object/full_text/QuerySimpleString.hpp"
#include "elasticsearch/v7_10/answer_model/search/object/Query.h"

namespace tests
{
using Model = StubModel;

template<class Element>
using MTerm = model::search::must::Term<Model, Element>;

template<class Element>
using MTerms = model::search::must::Terms<Model, Element>;

template<class Element>
using FTerm = model::search::filter::Term<Model, Element>;

template<class ...Elements>
using QSS = model::search::full_text::SimpleQueryString<Model, Elements...>;

TEST(NEW_MustQSS, serializer)
{
    model::search::Must<StubModel, MTerm<StubLeafNode_bool>,
                              MTerm<StubLeafNode_int>,
                              MTerms<StubLeafNode_string>,
                              QSS<StubLeafNode_string>> must_instance(MTerm<StubLeafNode_bool>(true), MTerms<StubLeafNode_string>("my_string_0"),
                                                                      QSS<StubLeafNode_string>("aaaa"));

    typename model::search::Must<StubModel, MTerm<StubLeafNode_bool>,
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
    using MustTag = model::search::Must<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    model::search::BooleanNew<StubModel, MustTag> bool_instance(must_instance);

    typename model::search::BooleanNew<StubModel, MustTag>::aggregator_serializer_type ser;
    nlohmann::json node = nlohmann::json::object();
    txml::StdoutTracer tracer;
    bool_instance.template format_serialize(ser, tracer);
    ser. template finalize(node, tracer);
    ASSERT_EQ(node.dump(), R"({"bool":{"must":[{"simple_query_string":{"fields":["test_stub_model.test_stub_leaf_string"],"query":"aaaa"}},{"terms":{"test_stub_model.test_stub_leaf_string":"my_string_0"}},{"term":{"test_stub_model.test_stub_leaf_bool":true}}]}})");
}

TEST(NEW_QueryBooleanQSSMustQSS, serializer)
{
    using MustTag = model::search::Must<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    using BooleanTag = model::search::BooleanNew<StubModel, MustTag>;
    BooleanTag bool_instance(must_instance);

    using QSSTag = QSS<StubLeafNode_string>;
    QSSTag sqt_param("acdc");

    using QueryTag = model::search::QueryNew<StubModel, BooleanTag, QSSTag>;
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
    using MustTag = model::search::Must<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerms<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_instance(MTerm<StubLeafNode_bool>(true),
                          MTerms<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("aaaa"));

    using FilterTag = model::search::Filter<StubModel,
                                       FTerm<StubLeafNode_bool>,
                                       FTerm<StubLeafNode_int>,
                                       FTerm<StubLeafNode_string>>;
    FilterTag filter_instance(FTerm<StubLeafNode_bool>(false),
                              FTerm<StubLeafNode_int>(22),
                              FTerm<StubLeafNode_string>("my_string_filter"));

    using BooleanTag = model::search::BooleanNew<StubModel, MustTag, FilterTag>;
    BooleanTag bool_instance(must_instance, filter_instance);

    using QSSTag = QSS<StubLeafNode_string>;
    QSSTag sqt_param("acdc");

    using QueryTag = model::search::QueryNew<StubModel, BooleanTag, QSSTag>;
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
    using MustTag = model::search::Must<StubModel,
                                   MTerm<StubLeafNode_bool>,
                                   MTerm<StubLeafNode_int>,
                                   MTerm<StubLeafNode_string>,
                                   QSS<StubLeafNode_string>>;
    MustTag must_param(MTerm<StubLeafNode_bool>(true),
                          MTerm<StubLeafNode_int>(11),
                          MTerm<StubLeafNode_string>("my_string_0"),
                          QSS<StubLeafNode_string>("my_string_0"));

    using FilterTag = model::search::Filter<StubModel,
                                   FTerm<StubLeafNode_bool>,
                                   FTerm<StubLeafNode_int>,
                                   FTerm<StubLeafNode_string>>;
    FilterTag filter_param(FTerm<StubLeafNode_bool>(false),
                          FTerm<StubLeafNode_int>(22),
                          FTerm<StubLeafNode_string>("my_string_filter"));

    using BooleanTag = model::search::BooleanNew<StubModel, MustTag, FilterTag>;
    BooleanTag boolean_param(must_param, filter_param);

    using QueryTag = model::search::QueryNew<StubModel, BooleanTag>;
    QueryTag q_instance(boolean_param);

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
