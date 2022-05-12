#include <optional>
#include <sstream>

#include "bin/cfg_param_names.hpp"
#include "bin/v7/dispatcher.hpp"
#include "bin/utils/parse_string.hpp"

#include "elasticsearch/books/book_reader.hpp"
#include "elasticsearch/images/image_reader.hpp"

namespace bin
{
namespace v7
{
namespace details
{
template <class Element, class ElementValue, class ...Args>
std::optional<Element> get_match_elem(const std::map<std::string, std::string> &data_storage, Args&& ...args)
{
    if (auto it = data_storage.find(std::string(Element::class_name())); it != data_storage.end())
    {
        return std::optional<Element>(ElementValue{it->second, std::forward<Args>(args)...});
    }
    return {};
}

template<>
std::optional<std::string>
get_match_elem<std::string,
               elasticsearch::common_model::CreationDateTime>(const std::map<std::string, std::string> &data_storage)
{
    if (auto it = data_storage.find(std::string(elasticsearch::common_model::CreationDateTime::class_name())); it != data_storage.end())
    {
        return std::optional<std::string>(it->second);
    }
    return {};
}

template<>
std::optional<std::string>
get_match_elem<std::string,
               elasticsearch::image::model::element::DigitizeTime>(const std::map<std::string, std::string> &data_storage)
{
    if (auto it = data_storage.find(std::string(elasticsearch::image::model::element::DigitizeTime::class_name())); it != data_storage.end())
    {
        return std::optional<std::string>(it->second);
    }
    return {};
}

template<>
std::optional<std::string>
get_match_elem<std::string,
               elasticsearch::image::model::element::OriginalTime>(const std::map<std::string, std::string> &data_storage)
{
    if (auto it = data_storage.find(std::string(elasticsearch::image::model::element::OriginalTime::class_name())); it != data_storage.end())
    {
        return std::optional<std::string>(it->second);
    }
    return {};
}

template <>
std::optional<elasticsearch::image::search::tag::geo_bbox>
get_match_elem<elasticsearch::image::search::tag::geo_bbox,
               elasticsearch::image::search::tag::geo_bbox, char>(const std::map<std::string, std::string> &data_storage, char &&sep)
{
    if (auto it = data_storage.find(std::string(elasticsearch::image::search::tag::geo_bbox::class_name())); it != data_storage.end())
    {
        const size_t float_num = 4;
        const char *pStart = it->second.c_str();
        std::vector<float> values;
        const char *pEnd = pStart;
        while (*pStart && values.size() != float_num)
        {
            const char *pEnd = elasticsearch::utils::get_next_char_if(pStart, [sep]( const char* sym) { return (*sym == sep);});
            try
            {
                values.push_back(stof(std::string(pStart, pEnd - pStart)));
            }
            catch (const std::exception &ex)
            {
                throw std::runtime_error(std::string("Cannot parse geo bounding box: ") +
                                         "4 float values, which enumerated by \"" + sep +
                                         "\", are expected, got: " + it->second);
            }
            pStart = pEnd + 1;
        }

        if (float_num != values.size())
        {
            throw std::runtime_error(std::string("Invalid geo bounding box: ") +
                                         "4 float values required, got: " + it->second);
        }

        return std::make_optional<elasticsearch::image::search::tag::geo_bbox>(model::search::geo::BBTopLeft {values[0], values[1]},
                                                                          model::search::geo::BBBottomRight {values[2], values[3]});
    }
    return {};
}


template<class Model>
using record_t = std::pair<std::string, std::shared_ptr<Model>>;

namespace helper
{
template<class Model, class SearchRequest, class Tracer>
std::vector<record_t<Model>>
extract_model_records(const std::shared_ptr<SearchRequest> &search_ptr, Tracer tracer)
{
    using transaction = SearchRequest;
    using data = Model;

    if (!search_ptr)
    {
        throw std::runtime_error("invalid search request");
    }

    std::shared_ptr<typename transaction::response> search_ans_ptr = search_ptr->get_response(tracer);
    auto status = search_ans_ptr->template getValue<::model::Status>();
    if (status->getValue() != 200)
    {
        auto fail_reason = search_ans_ptr->template getValue<elasticsearch::v7::search::Error>()->template getValue<::model::Reason>();
        auto caused_reason = search_ans_ptr->template getValue<elasticsearch::v7::search::Error>()->template getValue<::model::CausedBy>()->template getValue<::model::Reason>();
        throw std::runtime_error("unexpected search response, status: " + std::to_string(status->getValue()) + "\nError: " + fail_reason->getValue() +
                                 ", details: " + caused_reason->getValue());
    }

    auto hits = search_ans_ptr->template getValue<::model::HitsNode<data>>();
    if (!hits)
    {
        throw std::runtime_error("unexpected search request answer: no `HintNode` detected");
    }

    auto hits_array = hits->template getValue<::model::HitsArray<data>>()->getValue();
    std::vector<record_t<data>> ret;
    ret.reserve(hits_array.size());
    for (const auto &hit : hits_array)
    {
        auto source_ptr = hit->template getValue<::model::_Source<data>>();
        if (source_ptr && source_ptr->template getValue<data>())
        {
            ret.emplace_back(hit->template getValue<::model::_Type>()->getValue() + "/" + hit->template getValue<::model::_Id>()->getValue(),
                             source_ptr->template getValue<data>());
        }
    }
    return ret;
}
template<class Model, class RequestOperation, class ...ElementToMatch>
bool find_duplicate_records(const Model& model_value, RequestOperation op, std::ostream &out)
{
    using data = Model;

    std::map<std::string, std::string> search_duplicates_params;
    //bin::data_manipulation::extract_from_model<data, ElementToMatch...>(model_value, search_duplicates_params);
    bin::data_manipulation::extract_from_model<data, COMMON_DATA_MODEL_ELEMENTS>(model_value, search_duplicates_params);
    std::vector<bin::v7::details::record_t<data>> list = op(search_duplicates_params);
    if (!list.empty())
    {
        out << "Duplicated records count: " << list.size() << std::endl;
        for (const auto& r : list)
        {
            out << r.first << "\t";
            bin::models::print_elements<data, ElementToMatch...>(out, *r.second);
            bin::models::print_elements<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
            out << std::endl;
        }
        out << "use --force flag to forcibly insert new records" << std::endl;
        out << "our 'es_update` command with appropriate <document id> from list above to update the former one" << std::endl;
        return true;
    }
    return false;
}
}
template<class Tracer>
std::vector<record_t<elasticsearch::book::model::data>>
request_book_search_match(const dispatcher &d,
                          const std::map<std::string, std::string>& match_params,
                          const std::map<std::string, std::string>& sort_params,
                          Tracer tracer)
{
    using namespace std::chrono_literals;

    using namespace elasticsearch::book::model;
    using namespace elasticsearch::book::search;

    std::shared_ptr<transaction> search_ptr;
    const size_t max_count = 1000;
    const auto pit_interval = 10s;
    if (match_params.empty())
    {
        if (sort_params.empty())
        {
            search_ptr = d.execute_request<transaction>(schema_indices[0], schema_indices[0],
                                                        max_count, pit_interval,
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
        else
        {
            search_ptr = d.execute_request<transaction>(schema_indices[0], schema_indices[0],
                                                        max_count, pit_interval,
                                                        tag::sort<element::Contributor> ({::model::Order("desc")}),
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
    }
    else
    {
        auto mu = tag::create::must_tag(details::get_match_elem<element::Contributor, std::string>(match_params),
                                        details::get_match_elem<element::Creator, std::string>(match_params),
                                        details::get_match_elem<element::Identifier, std::string>(match_params),
                                        details::get_match_elem<element::Language, std::string>(match_params),
                                        details::get_match_elem<element::Title, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::BinaryBlob, std::string>(match_params),
                                        //details::get_match_elem<elasticsearch::common_model::CreationDateTime, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Description, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Format, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::OriginalPath, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Preview, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::SourceName, std::string>(match_params),
                                        /*std::optional<std::list<std::string>>{}*/
                                        details::get_match_elem<elasticsearch::common_model::Tags, elasticsearch::common_model::Tags>(match_params, ","));
        auto boo = tag::create::boolean_tag(mu);
        search_ptr = d.execute_request<transaction>(schema_indices[0], schema_indices[0],
                                                    max_count, pit_interval,
                                                    tag::create::query_tag(boo),
                                                    tag::sort<element::Contributor> ({::model::Order("desc")}),
                                                    d.get_settings().curl_verbose,
                                                    tracer);
    }
    return helper::extract_model_records<data>(search_ptr, tracer);
}

template<class Tracer>
std::vector<record_t<elasticsearch::image::model::data>>
request_image_search_match(const dispatcher &d,
                        const std::map<std::string, std::string>& match_params,
                        const std::map<std::string, std::string>& sort_params,
                        Tracer tracer)
{
    using namespace std::chrono_literals;

    using namespace elasticsearch::image::model;
    using namespace elasticsearch::image::search;

    std::shared_ptr<transaction> search_ptr;
    const size_t max_count = 1000;
    const auto pit_interval = 10s;
    if (match_params.empty())
    {
        if (sort_params.empty())
        {
            search_ptr = d.execute_request<transaction>(schema_indices[1], schema_indices[1],
                                                        max_count, pit_interval,
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
        else
        {
            search_ptr = d.execute_request<transaction>(schema_indices[1], schema_indices[1],
                                                        max_count, pit_interval,
                                                        tag::sort<element::Camera> ({::model::Order("desc")}),
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
    }
    else
    {
        auto mu = tag::create::must_tag(details::get_match_elem<element::Camera, std::string>(match_params),
                                        details::get_match_elem<element::CameraModel, std::string>(match_params),
                                        details::get_match_elem<element::DigitizeTime, std::string>(match_params),
                                        //tag::make(details::get_match_elem<element::Location, std::string>(match_params)),
                                        details::get_match_elem<element::OriginalTime, std::string>(match_params),
                                        //tag::make(details::get_match_elem<element::Resolution, std::string>(match_params)),
                                        details::get_match_elem<element::Title, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::BinaryBlob, std::string>(match_params),
                                        //details::get_match_elem<elasticsearch::common_model::CreationDateTime, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Description, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Format, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::OriginalPath, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Preview, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::SourceName, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Tags, elasticsearch::common_model::Tags>(match_params, ","));

                                        auto fff = details::get_match_elem<tag::geo_bbox, tag::geo_bbox, char>(match_params, ',');
        auto fi = tag::create::filter_tag(fff);
        auto boo = tag::create::boolean_tag(mu, fi);

        auto r = tag::create::range_tag<elasticsearch::common_model::CreationDateTime,
                                        element::DigitizeTime,
                                        element::OriginalTime>(
                {details::get_match_elem<std::string, elasticsearch::common_model::CreationDateTime>(match_params),
                 details::get_match_elem<std::string, element::DigitizeTime>(match_params),
                 details::get_match_elem<std::string, element::OriginalTime>(match_params)});
        auto query = tag::create::query_tag(boo, r);
        search_ptr = d.execute_request<transaction>(schema_indices[1], schema_indices[1],
                                                    max_count, pit_interval,
                                                    query,
                                                    tag::sort<element::Camera> ({::model::Order("desc")}),
                                                    d.get_settings().curl_verbose,
                                                    tracer);
    }
    return helper::extract_model_records<data>(search_ptr, tracer);
}

template <class Tracer>
void request_book_index_mapping(const dispatcher &d, Tracer)
{
    d.execute_request<elasticsearch::book::index_mapping::transaction>(schema_indices[0], schema_indices[0], d.get_settings().curl_verbose);
    d.execute_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[0], doc_id_schema_indices[0], d.get_settings().curl_verbose);
}

template <class Tracer>
void request_image_index_mapping(const dispatcher &d, Tracer)
{
    d.execute_request<elasticsearch::image::index_mapping::transaction>(schema_indices[1], schema_indices[1], d.get_settings().curl_verbose);
    d.execute_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[1], doc_id_schema_indices[1], d.get_settings().curl_verbose);
}

template <class Tracer>
void request_book_index_mapping_delete(const dispatcher &d, Tracer)
{
    d.execute_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[0], schema_indices[0], d.get_settings().curl_verbose);
    d.execute_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[0], doc_id_schema_indices[0], d.get_settings().curl_verbose);
}

template <class Tracer>
void request_image_index_mapping_delete(const dispatcher &d, Tracer)
{
    d.execute_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[1], schema_indices[1], d.get_settings().curl_verbose);
    d.execute_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[1], doc_id_schema_indices[1], d.get_settings().curl_verbose);
}

template<class Tracer>
void request_rm_data(const dispatcher &d, std::ostream &out, const char *index, const char *doc_path_id, Tracer tracer)
{
    std::shared_ptr<elasticsearch::v7::delete_data::response> ans_ptr;
    if (!strcmp(index, schema_indices[0]))
    {
        ans_ptr = d.execute_request<elasticsearch::book::del::transaction>(
                                    index,
                                    std::string(index) + "/" + doc_path_id,
                                    d.get_settings().curl_verbose)->get_response();
    }
    else if (!strcmp(index, schema_indices[1]))
    {
        ans_ptr = d.execute_request<elasticsearch::image::del::transaction>(
                                    index,
                                    std::string(index) + "/" + doc_path_id,
                                    d.get_settings().curl_verbose)->get_response();
    }
    else
    {
        throw std::runtime_error(std::string("invalid index: ") + index);
    }
    if (ans_ptr->getValue<model::Result>())
    {
        out << ans_ptr->getValue<model::Result>()->getValue() << std::endl;
    }
    else
    {
        throw std::runtime_error(std::string("invalid result of delete operation: ") + doc_path_id + ", in index: " + index);
    }
}


template <class Tracer>
void request_put_data(const dispatcher &d, std::ostream &out,
                      const char *file_path,
                      const std::map<std::string, std::string>& override_model_params,
                      bool ignore_existing,
                      Tracer tracer)
{
    // determine content type
    try
    {
        using namespace elasticsearch::image::model;
        elasticsearch::image::reader r(file_path);

        auto model_value = r.to_model(tracer);

        bin::data_manipulation::inject_to_model<data, IMAGE_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);
        bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);

        // try to search duplicates
        if (!ignore_existing)
        {
            auto finder = [&d, &tracer](auto &search_duplicates_params)
            { return request_image_search_match(d, search_duplicates_params, {}, tracer);};
            if (helper::find_duplicate_records<data, decltype(finder), IMAGE_DATA_MODEL_ELEMENTS>(*model_value, finder, out))
            {
                return;
            }
        }
        auto generator_ptr =
                    d.execute_request<elasticsearch::aux::create_doc_id::transaction>(
                            schema_indices[1],
                            std::string(doc_id_schema_indices[1]) + "/_doc/0",
                            d.get_settings().curl_verbose, tracer);
        d.execute_request<elasticsearch::image::create::transaction>(
                            schema_indices[1],
                            std::string(schema_indices[1]) + "/_doc/" + std::to_string(generator_ptr->get_unique_index().getValue()),
                            *model_value,
                            d.get_settings().curl_verbose,
                            tracer);
    } catch (...) {
        try {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);
            auto model_value = r.to_model(tracer);
            bin::data_manipulation::inject_to_model<data, BOOK_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);
            bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);

            // try to search duplicates
            if (!ignore_existing)
            {
                auto finder = [&d, &tracer](auto &search_duplicates_params)
                { return request_book_search_match(d, search_duplicates_params, {}, tracer);};
                if (helper::find_duplicate_records<data, decltype(finder), BOOK_DATA_MODEL_ELEMENTS>(*model_value, finder, out))
                {
                    return;
                }
            }
            // generate new document id
            auto generator_ptr =
                    d.execute_request<elasticsearch::aux::create_doc_id::transaction>(
                                        schema_indices[0],
                                        std::string(doc_id_schema_indices[0]) + "/_doc/0",
                                        d.get_settings().curl_verbose, tracer);
            d. execute_request<elasticsearch::book::create::transaction>(
                                schema_indices[0],
                                std::string(schema_indices[0]) + "/_doc/" +  std::to_string(generator_ptr->get_unique_index().getValue()),
                                *model_value,
                                d.get_settings().curl_verbose,
                                tracer);
        } catch (const std::exception& ex) {
            throw std::runtime_error(std::string("unsupported format by path: ") + file_path + ", error: " + ex.what());
        }
    }
}

template <class Tracer>
void request_update_data(dispatcher &d,
                         std::ostream &out, const char *file_path, const char* document_id,
                         const std::map<std::string, std::string>& override_model_params,
                         Tracer tracer)
{
    // determine content type
    try
    {
        using namespace elasticsearch::image::model;
        elasticsearch::image::reader r(file_path);

        auto model_value = r.to_model(tracer);

        bin::data_manipulation::inject_to_model<data, IMAGE_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);
        bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);
        d.execute_request<elasticsearch::image::create::transaction>(
                            schema_indices[1],
                            std::string(schema_indices[1]) + "/" + document_id,
                            *model_value,
                            d.get_settings().curl_verbose,
                            tracer);
    } catch (...) {
        try {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);
            auto model_value = r.to_model(tracer);
            bin::data_manipulation::inject_to_model<data, BOOK_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);
            bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, override_model_params);

            d.execute_request<elasticsearch::book::create::transaction>(
                                schema_indices[0],
                                std::string(schema_indices[0]) + "/" + document_id,
                                *model_value,
                                d.get_settings().curl_verbose,
                                tracer);
        } catch (const std::exception& ex) {
            throw std::runtime_error(std::string("unsupported format by path: ") + file_path + ", error: " + ex.what());
        }
    }
}
template <class Tracer>
std::map<std::string, std::string>
request_collect_model_data(dispatcher &d, const char *file_path, Tracer tracer)
{
    (void)d;
    std::map<std::string, std::string> ret;
    // determine content type
    try
    {
        using namespace elasticsearch::image::model;
        elasticsearch::image::reader r(file_path);
        auto model_value = r.to_model(tracer);

        bin::data_manipulation::extract_from_model<data, IMAGE_DATA_MODEL_ELEMENTS>(*model_value, ret);
        bin::data_manipulation::extract_from_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, ret);
    } catch (...) {
        try {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);

            auto model_value = r.to_model(tracer);
            bin::data_manipulation::extract_from_model<data, BOOK_DATA_MODEL_ELEMENTS>(*model_value, ret);
            bin::data_manipulation::extract_from_model<data, COMMON_DATA_MODEL_ELEMENTS>(*model_value, ret);
        } catch (const std::exception& ex) {
            throw std::runtime_error(std::string("unsupported format by path: ") + file_path + ", error: " + ex.what());
        }
    }
    return ret;
}
}

dispatcher::dispatcher(dispatcher_settings s, bool with_logs) :
    settings(std::move(s)),
    use_logging(with_logs)
{
    init_cluster_dispatchers();
    if (use_logging)
    {
        txml::StdoutTracer std_tracer;
        create_request<elasticsearch::book::index_mapping::transaction>(schema_indices[0], std_tracer);
        create_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[0], std_tracer);
        create_request<elasticsearch::image::index_mapping::transaction>(schema_indices[1], std_tracer);
        create_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[1], std_tracer);
    }
    else
    {
        create_request<elasticsearch::book::index_mapping::transaction>(schema_indices[0]);
        create_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[0]);
        create_request<elasticsearch::image::index_mapping::transaction>(schema_indices[1]);
        create_request<elasticsearch::aux::index_mapping::transaction>(schema_indices[1]);
    }

    create_request<elasticsearch::book::create::transaction>(schema_indices[0]);
    create_request<elasticsearch::image::create::transaction>(schema_indices[1]);
    create_request<elasticsearch::book::del::transaction>(schema_indices[0]);
    create_request<elasticsearch::image::del::transaction>(schema_indices[1]);
    create_request<elasticsearch::book::get::transaction>(schema_indices[0]);
    create_request<elasticsearch::image::get::transaction>(schema_indices[1]);
    create_request<elasticsearch::book::del::transaction>(schema_indices[0]);
    create_request<elasticsearch::image::del::transaction>(schema_indices[1]);
    create_request<elasticsearch::book::search::transaction>(schema_indices[0]);
    create_request<elasticsearch::image::search::transaction>(schema_indices[1]);
    create_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[0]);
    create_request<elasticsearch::v7::index_mapping_delete::transaction>(schema_indices[1]);

    create_request<elasticsearch::aux::create_doc_id::transaction>(schema_indices[0]);
    create_request<elasticsearch::aux::create_doc_id::transaction>(schema_indices[1]);
}

std::ostream &dispatcher::data_outputstream() const
{
    return std::cout;
}
std::ostream &dispatcher::aux_outputstream() const
{
    return std::cerr;
}

const dispatcher_settings& dispatcher::get_settings() const
{
    return settings;
}

void dispatcher::init_cluster_dispatchers()
{
    for (const auto &h : settings.hosts)
    {
        dispatchers[h] = std::make_shared<base_dispatcher_t>(dispatcher_settings{settings});
    }
}


void dispatcher::all_index_mapping()
{
    book_index_mapping();
    image_index_mapping();
}

void dispatcher::book_index_mapping()
{
    details::request_book_index_mapping(*this, txml::EmptyTracer{});
}

void dispatcher::image_index_mapping()
{
    details::request_image_index_mapping(*this, txml::EmptyTracer{});
}

void dispatcher::all_index_mapping_delete()
{
    book_index_mapping_delete();
    image_index_mapping_delete();
}

void dispatcher::book_index_mapping_delete()
{
    details::request_book_index_mapping_delete(*this, txml::EmptyTracer{});
}

void dispatcher::image_index_mapping_delete()
{
    details::request_image_index_mapping_delete(*this, txml::EmptyTracer{});
}

void dispatcher::put_data(const char *file_path, const std::map<std::string, std::string>& override_model_params, bool ignore_existing)
{
    if (use_logging)
    {
        details::request_put_data(*this, std::cout, file_path, override_model_params, ignore_existing, txml::StdoutTracer{});
    }
    else
    {
        details::request_put_data(*this, std::cout, file_path, override_model_params, ignore_existing, txml::EmptyTracer{});
    }
}

void dispatcher::update_data(const char *file_path, const char *updated_document_id,
                             const std::map<std::string, std::string>& override_model_params)
{
    if (use_logging)
    {
        if (updated_document_id)
        {
            details::request_update_data(*this, std::cout, file_path, updated_document_id, override_model_params, txml::StdoutTracer{});
        }
        else
        {
            details::request_put_data(*this, std::cout, file_path, override_model_params, true, txml::StdoutTracer{});
        }
    }
    else
    {
        if (updated_document_id)
        {
            details::request_update_data(*this, std::cout, file_path, updated_document_id, override_model_params, txml::EmptyTracer{});
        }
        else
        {
            details::request_put_data(*this, std::cout, file_path, override_model_params, true, txml::EmptyTracer{});
        }
    }
}

void dispatcher::rm_data(const char* index, const char *doc_path_id)
{
    if (use_logging)
    {
        details::request_rm_data(*this, std::cout, index, doc_path_id, txml::StdoutTracer{});
    }
    else
    {
        details::request_rm_data(*this, std::cout, index, doc_path_id, txml::EmptyTracer{});
    }
}

std::map<std::string, std::string> dispatcher::collect_model_data(const char *file_path)
{
    if (use_logging)
    {
        return details::request_collect_model_data(*this, file_path, txml::StdoutTracer{});
    }
    return details::request_collect_model_data(*this, file_path, txml::EmptyTracer{});
}

void dispatcher::search_match(const char* model_name, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params)
{
    if (!strcmp(model_name, schema_indices[0]))
    {
        book_search_match(match_params, sort_params);
    }
    else if (!strcmp(model_name, schema_indices[1]))
    {
        image_search_match(match_params, sort_params);
    }
    else
    {
        throw std::runtime_error(std::string("invalid model name: ") + model_name);
    }
}

void dispatcher::book_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params)
{
    if (use_logging)
    {
        dump_book_search_match_impl(std::cout, match_params, sort_params, txml::StdoutTracer{});
    }
    else
    {
        dump_book_search_match_impl(std::cout, match_params, sort_params, txml::EmptyTracer{});
    }
}

void dispatcher::image_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params)
{
    if (use_logging)
    {
        dump_request_image_search_match(std::cout, match_params, sort_params, txml::StdoutTracer{});
    }
    else
    {
        dump_request_image_search_match(std::cout, match_params, sort_params, txml::EmptyTracer{});
    }
}

template<class Tracer>
void dispatcher::dump_book_search_match_impl(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer)
{
    using namespace elasticsearch::book::model;
    using namespace elasticsearch::book::search;
    std::vector<bin::v7::details::record_t<data>> list =
                        bin::v7::details::request_book_search_match(*this, match_params, sort_params, tracer);
    out << "records count: " << list.size() << std::endl;
    for (const auto& r : list)
    {
        out << r.first << "\t";
        bin::models::print_elements<data, BOOK_DATA_MODEL_ELEMENTS>(out, *r.second);
        bin::models::print_elements<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
        out << std::endl;
    }
}

template<class Tracer>
void dispatcher::dump_request_image_search_match(std::ostream& out, const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params, Tracer tracer)
{
    using namespace elasticsearch::image::model;
    using namespace elasticsearch::image::search;
    std::vector<bin::v7::details::record_t<data>> list =
                        bin::v7::details::request_image_search_match(*this, match_params, sort_params, tracer);
    out << "records count: " << list.size() << std::endl;
    for (const auto& r : list)
    {
        out << r.first << "\t";
        bin::models::print_elements<data, IMAGE_DATA_MODEL_ELEMENTS>(out, *r.second);
        bin::models::print_elements<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
        out << std::endl;
    }
}
} // namespace v7
} // namespace bin
