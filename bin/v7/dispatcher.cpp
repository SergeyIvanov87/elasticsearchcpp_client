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
            pEnd = elasticsearch::utils::get_next_char_if(pStart, [sep]( const char* sym) { return (*sym == sep);});
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
using record_t = std::pair<std::string, std::optional<Model>>;

namespace helper
{
template<class Model, class SearchRequest, class Tracer>
std::vector<record_t<Model>>
extract_model_records(const std::shared_ptr<SearchRequest> &search_ptr, Tracer tracer)
{
    using data = Model;

    if (!search_ptr)
    {
        assert(search_ptr);
        throw std::runtime_error("invalid search request");
    }

    auto&& search_ans_ptr = search_ptr->get_response(tracer);
    const auto &status = search_ans_ptr.template node<::model::Status>();
    if (status.has_value() && status->value() != 200)
    {
        const auto &fail_reason = search_ans_ptr.template node<elasticsearch::v7::search::Error>()->template node<::model::Reason>();
        const auto &caused = search_ans_ptr.template node<elasticsearch::v7::search::Error>()->template node<::model::CausedBy>();
        std::string details;
        if (caused.has_value())
        {
            const auto &caused_reason = caused->template node<::model::Reason>();
            if (caused_reason.has_value())
            {
                details = caused_reason->value();
            }
        }
        throw std::runtime_error("unexpected search response, status: " + std::to_string(status->value()) + "\nError: " + fail_reason->value() +
                                 ", details: " + (details.empty() ? "<Unknown>" : details));
    }

    const auto &hits = search_ans_ptr.template node<::model::HitsNode<data>>();
    if (!hits)
    {
        throw std::runtime_error("unexpected search request answer: no `HintNode` detected");
    }

    const auto &hits_array = hits->template node<::model::HitsArray<data>>()->value();
    std::vector<record_t<data>> ret;
    ret.reserve(hits_array.size());
    for (const auto &hit : hits_array)
    {
        const auto &source_ptr = hit->template node<::model::_Source<data>>();
        if (source_ptr.has_value() && source_ptr->template node<data>())
        {
            ret.emplace_back(hit->template node<::model::_Type>()->value() + "/" + hit->template node<::model::_Id>()->value(),
                             source_ptr->template node<data>());
        }
    }
    return ret;
}
template<class Model, class RequestOperation, class ...ElementToMatch>
size_t find_duplicate_records(const Model& model_value, RequestOperation op, std::ostream &out)
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
            bin::models::print_element_values<data, ElementToMatch...>(out, *r.second);
            bin::models::print_element_values<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
            out << std::endl;
        }
        out << std::endl;
        out << "use --force flag to forcibly insert new records" << std::endl;
        out << "or 'es_update` command with appropriate <document id> from list above to update the former one" << std::endl;
        return list.size();
    }
    return 0;
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
        auto r = tag::create::range_tag<elasticsearch::common_model::CreationDateTime>(
                {details::get_match_elem<std::string, elasticsearch::common_model::CreationDateTime>(match_params)});
        // if MUST exist, then RANGE should be part of MUST
        auto mu = tag::create::must_tag(details::get_match_elem<element::Contributor, std::string>(match_params),
                                        details::get_match_elem<element::Creator, std::string>(match_params),
                                        details::get_match_elem<element::Identifier, std::string>(match_params),
                                        details::get_match_elem<element::Language, std::string>(match_params),
                                        details::get_match_elem<element::Title, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::BinaryBlob, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Description, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Format, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::OriginalPath, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Preview, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::SourceName, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Tags, elasticsearch::common_model::Tags>(match_params, ","),
                                        decltype(r) {});
        if (mu)
        {
            // obtain RANGE into MUST
            mu->value().emplace_back(std::move(r));
            auto boo = tag::create::boolean_tag(mu);
            search_ptr = d.execute_request<transaction>(schema_indices[0], schema_indices[0],
                                                    max_count, pit_interval,
                                                    tag::create::query_tag(boo).value(),
                                                    tag::sort<element::Contributor> ({::model::Order("desc")}),
                                                    d.get_settings().curl_verbose,
                                                    tracer);
        }
        else
        {
            // if no MUST tag presents then insert RANGE into QUERY by itsels
            search_ptr = d.execute_request<transaction>(schema_indices[0], schema_indices[0],
                                                    max_count, pit_interval,
                                                    tag::create::query_tag(r).value(),
                                                    tag::sort<element::Contributor> ({::model::Order("desc")}),
                                                    d.get_settings().curl_verbose,
                                                    tracer);
        }
    }
    return helper::extract_model_records<data>(search_ptr, tracer);
}

void request_book_search_param_info(const dispatcher &, std::ostream &out)
{
    using namespace elasticsearch::book::model;
    out << "params list all:" << std::endl;
    out << '\t';
    bin::models::print_elements<BOOK_DATA_MODEL_ELEMENTS>(out);
    bin::models::print_elements<COMMON_DATA_MODEL_ELEMENTS>(out);
    out << std::endl;

    out << "params list \"must\":" <<  std::endl;
    out << '\t' << element::Contributor::class_name() << "," << element::Creator::class_name() << ","
        << element::Identifier::class_name()  << "," << element::Language::class_name() << ","
        << element::Title::class_name() << "," << elasticsearch::common_model::Description::class_name() << ","
        << elasticsearch::common_model::Format::class_name() << "," << elasticsearch::common_model::OriginalPath::class_name() << ","
        << elasticsearch::common_model::SourceName::class_name() << "," << elasticsearch::common_model::Tags::class_name() << std::endl;

    out << "details:\n";
    out << "\t" << element::Contributor::class_name() << std::endl;
    out << "\t" << element::Creator::class_name() << std::endl;
    out << "\t" << element::Identifier::class_name() << std::endl;
    out << "\t" << element::Language::class_name() << std::endl;
    out << "\t" << element::Title::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Description::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Format::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::OriginalPath::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::SourceName::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Tags::class_name() << "\t*\tLIST: Use ',' as separator" << std::endl;
    out << "\n";

    out << "params list \"range\":" <<  std::endl;
    out << '\t' << elasticsearch::common_model::CreationDateTime::class_name() << std::endl;
    out << "details:\n";
    out << "\t" << elasticsearch::common_model::CreationDateTime::class_name()
        << "\t*\t RANGE of '2' datetimes enclosed by intervals symbols \"(\" or \"[\" and \")\" or \"]\"" << std::endl;
    out << "\n";

    out << "Example:\n"
        << "./es_search book \"Language:es\" \"title:aaa\" \"creation_datetime:(2022-07-01,2022-08-08]\"" << std::endl;
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
        auto fi = tag::create::filter_tag(details::get_match_elem<tag::geo_bbox, tag::geo_bbox, char>(match_params, ','));
        auto r = tag::create::range_tag<elasticsearch::common_model::CreationDateTime,
                                        element::DigitizeTime,
                                        element::OriginalTime>(
                {details::get_match_elem<std::string, elasticsearch::common_model::CreationDateTime>(match_params),
                 details::get_match_elem<std::string, element::DigitizeTime>(match_params),
                 details::get_match_elem<std::string, element::OriginalTime>(match_params)});
        // if MUST exist, then RANGE should be part of MUST
        auto mu = tag::create::must_tag(details::get_match_elem<element::Camera, std::string>(match_params),
                                        details::get_match_elem<element::CameraModel, std::string>(match_params),
                                        //tag::make(details::get_match_elem<element::Location, std::string>(match_params)),
                                        //tag::make(details::get_match_elem<element::Resolution, std::string>(match_params)),
                                        details::get_match_elem<element::Title, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::BinaryBlob, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Description, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Format, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::OriginalPath, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Preview, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::SourceName, std::string>(match_params),
                                        details::get_match_elem<elasticsearch::common_model::Tags, elasticsearch::common_model::Tags>(match_params, ","),
                                        decltype(r) {});
        if (mu || fi)
        {
            // obtain RANGE into MUST
            mu->value().emplace_back(std::move(r));
            auto boo = tag::create::boolean_tag(mu, fi);
            auto query = tag::create::query_tag(boo);
            search_ptr = d.execute_request<transaction>(schema_indices[1], schema_indices[1],
                                                        max_count, pit_interval,
                                                        query.value(),
                                                        tag::sort<element::Camera> ({::model::Order("desc")}),
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
        else
        {
            // if no MUST tag presents then insert RANGE into QUERY by itsels
            auto query = tag::create::query_tag(r);
            search_ptr = d.execute_request<transaction>(schema_indices[1], schema_indices[1],
                                                        max_count, pit_interval,
                                                        query.value(),
                                                        tag::sort<element::Camera> ({::model::Order("desc")}),
                                                        d.get_settings().curl_verbose,
                                                        tracer);
        }
    }
    return helper::extract_model_records<data>(search_ptr, tracer);
}

void request_image_search_param_info(const dispatcher &, std::ostream &out)
{
    using namespace elasticsearch::image::model;
    out << "params list all:" << std::endl;
    out << '\t';
    bin::models::print_elements<IMAGE_DATA_MODEL_ELEMENTS>(out);
    bin::models::print_elements<COMMON_DATA_MODEL_ELEMENTS>(out);
    out << std::endl;

    out << "params list \"must\":" <<  std::endl;
    out << '\t' << element::Camera::class_name() << "," << element::CameraModel::class_name() << ","
        << element::Title::class_name() << "," << elasticsearch::common_model::Description::class_name() << ","
        << elasticsearch::common_model::Format::class_name() << "," << elasticsearch::common_model::OriginalPath::class_name() << ","
        << elasticsearch::common_model::SourceName::class_name() << "," << elasticsearch::common_model::Tags::class_name() << std::endl;
    out << "details:\n";
    out << "\t" << element::Camera::class_name() << std::endl;
    out << "\t" << element::CameraModel::class_name() << std::endl;
    out << "\t" << element::Title::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Description::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Format::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::OriginalPath::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::SourceName::class_name() << std::endl;
    out << "\t" << elasticsearch::common_model::Tags::class_name() << "\t*\tLIST: use ',' as separator" << std::endl;
    out << "\n";

    using namespace elasticsearch::image::search;
    out << "params list \"filter\":" <<  std::endl;
    out << '\t' << tag::geo_bbox::class_name() << std::endl;
    out << "details:\n";
    out << '\t' << tag::geo_bbox::class_name() << "\t*\tLIST of '4' floats with ',' as value separator:\n"
            "\t\t\t\tX_top_left, Y_top_left, X_bottom_right, Y_botton_right:" << std::endl;
    out << "\n";

    out << "params list \"range\":" <<  std::endl;
    out << '\t' << elasticsearch::common_model::CreationDateTime::class_name() << "," << element::DigitizeTime::class_name() << ","
        << element::OriginalTime::class_name() << std::endl;
    out << "details:\n";
    out << '\t' << elasticsearch::common_model::CreationDateTime::class_name()
        << "\t*\t RANGE of '2' datetimes enclosed by intervals symbols \"(\" or \"[\" and \")\" or \"]\"" << std::endl;
    out << "\t" << element::DigitizeTime::class_name()
        << "\t*\t RANGE of '2' datetimes enclosed by intervals symbols \"(\" or \"[\" and \")\" or \"]\"" << std::endl;
    out << "\t" << element::OriginalTime::class_name()
        << "\t*\t RANGE of '2' datetimes enclosed by intervals symbols \"(\" or \"[\" and \")\" or \"]\""<< std::endl;
    out << "\n";

    out << "Example:\n"
        << "./es_search image \"camera:rrr\" \"title:aaa\" \"geo_bounding_box:10,10,0,0\" \"creation_datetime:(2022-07-01,2022-08-08]\"" << std::endl;
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
void request_rm_data(const dispatcher &d, std::ostream &out, const char *index, const char *doc_path_id, Tracer)
{
    std::optional<elasticsearch::v7::delete_data::response> ans_ptr;
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
    if (ans_ptr.has_value())
    {
        out << doc_path_id << " - " << ans_ptr->node<model::Result>()->value() << std::endl;
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
    std::unique_ptr<elasticsearch::image::model::data> image_model_promise;
    std::unique_ptr<elasticsearch::book::model::data> book_model_promise;
    std::stringstream exception_logging;
    try
    {
        elasticsearch::image::reader r(file_path);

        image_model_promise.reset(new elasticsearch::image::model::data(r.to_model(tracer)));
        bin::data_manipulation::inject_to_model<elasticsearch::image::model::data,
                                                IMAGE_DATA_MODEL_ELEMENTS>(*image_model_promise, override_model_params);
    }
    catch (const std::exception &ex)
    {
        exception_logging << "Image schema parse failed, reason:\n" << ex.what() << std::endl;
        try
        {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);
            book_model_promise.reset(new elasticsearch::book::model::data(r.to_model(tracer)));
            bin::data_manipulation::inject_to_model<elasticsearch::book::model::data,
                                                    BOOK_DATA_MODEL_ELEMENTS>(*book_model_promise, override_model_params);
        }
        catch (const std::exception& exx) {
            exception_logging << "Book schema parse failed, reason:\n" << exx.what() << std::endl;
            throw std::runtime_error(std::string("unsupported format by path: ") + file_path + ", error: " + exception_logging.str());
        }
    }

    if (image_model_promise)
    {
        using namespace elasticsearch::image::model;
        bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*image_model_promise, override_model_params);

        // try to search duplicates
        if (!ignore_existing)
        {
            auto finder = [&d, &tracer](auto &search_duplicates_params)
            { return request_image_search_match(d, search_duplicates_params, {}, tracer);};
            size_t duplicates_count = helper::find_duplicate_records<data, decltype(finder), IMAGE_DATA_MODEL_ELEMENTS>(*image_model_promise, finder, out);
            if (duplicates_count)
            {
                throw std::runtime_error("FAILED");
            }
        }
        auto generator_ptr =
                    d.execute_request<elasticsearch::aux::create_doc_id::transaction>(
                            schema_indices[1],
                            std::string(doc_id_schema_indices[1]) + "/_doc/0",
                            d.get_settings().curl_verbose, tracer);
        d.execute_request<elasticsearch::image::create::transaction>(
                            schema_indices[1],
                            std::string(schema_indices[1]) + "/_doc/" + std::to_string(generator_ptr->get_unique_index().value()),
                            *image_model_promise,
                            d.get_settings().curl_verbose,
                            tracer);
    }

    if (book_model_promise)
    {
        using namespace elasticsearch::book::model;
        bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(*book_model_promise, override_model_params);

        // try to search duplicates
        if (!ignore_existing)
        {
            auto finder = [&d, &tracer](auto &search_duplicates_params)
            { return request_book_search_match(d, search_duplicates_params, {}, tracer);};
            size_t duplicates_count = helper::find_duplicate_records<data, decltype(finder), BOOK_DATA_MODEL_ELEMENTS>(*book_model_promise, finder, out);
            if (duplicates_count)
            {
                throw std::runtime_error("FAILED");
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
                           std::string(schema_indices[0]) + "/_doc/" +  std::to_string(generator_ptr->get_unique_index().value()),
                           *book_model_promise,
                           d.get_settings().curl_verbose,
                           tracer);
    }
}

template <class Tracer>
std::map<std::string, dispatcher::csv_data_t>
    request_get_data(const dispatcher &d, std::ostream &,
                     const char *index, const char **document_names, size_t document_count,
                     Tracer)
{
    std::map<std::string, dispatcher::csv_data_t> ret;
    if (!strcmp(index, schema_indices[0]))
    {
        using namespace elasticsearch::book::get;
        transaction::response return_model;
        for (size_t i = 0; i < document_count; i++)
        {
            return_model = d.execute_request<transaction>(index,
                                                          std::string(index) + "/" + document_names[i],
                                                          d.get_settings().curl_verbose)->get_response();
            ret.emplace(document_names[i],
                        std::make_tuple(return_model.template node<elasticsearch::common_model::OriginalPath>()->value(),
                                        return_model.template node<elasticsearch::common_model::BinaryBlob>()->getEncodedValue()));
        }
    }
    else if (!strcmp(index, schema_indices[1]))
    {
        using namespace elasticsearch::image::get;
        transaction::response return_model;
        for (size_t i = 0; i < document_count; i++)
        {
            return_model = d.execute_request<transaction>(index,
                                                          std::string(index) + "/" + document_names[i],
                                                          d.get_settings().curl_verbose)->get_response();
            ret.emplace(document_names[i],
                        std::make_tuple(return_model.template node<elasticsearch::common_model::OriginalPath>()->value(),
                                        return_model.template node<elasticsearch::common_model::BinaryBlob>()->getEncodedValue()));
        }
    }
    else
    {
        throw std::runtime_error(std::string("invalid index: ") + index);
    }
    return ret;
}

template <class Tracer>
void request_update_data(dispatcher &d,
                         std::ostream &, const char *file_path, const char* document_id,
                         const std::map<std::string, std::string>& override_model_params,
                         Tracer tracer)
{
    // determine content type
    try
    {
        using namespace elasticsearch::image::model;
        elasticsearch::image::reader r(file_path);

        auto&& model_value = r.to_model(tracer);

        bin::data_manipulation::inject_to_model<data, IMAGE_DATA_MODEL_ELEMENTS>(model_value, override_model_params);
        bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(model_value, override_model_params);
        d.execute_request<elasticsearch::image::create::transaction>(
                            schema_indices[1],
                            std::string(schema_indices[1]) + "/" + document_id,
                            model_value,
                            d.get_settings().curl_verbose,
                            tracer);
    } catch (...) {
        try {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);
            auto&& model_value = r.to_model(tracer);
            bin::data_manipulation::inject_to_model<data, BOOK_DATA_MODEL_ELEMENTS>(model_value, override_model_params);
            bin::data_manipulation::inject_to_model<data, COMMON_DATA_MODEL_ELEMENTS>(model_value, override_model_params);

            d.execute_request<elasticsearch::book::create::transaction>(
                                schema_indices[0],
                                std::string(schema_indices[0]) + "/" + document_id,
                                model_value,
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
        auto&& model_value = r.to_model(tracer);

        bin::data_manipulation::extract_from_model<data, IMAGE_DATA_MODEL_ELEMENTS>(model_value, ret);
        bin::data_manipulation::extract_from_model<data, COMMON_DATA_MODEL_ELEMENTS>(model_value, ret);
        ret["schema"] = schema_indices[1];
    } catch (...) {
        try {
            using namespace elasticsearch::book::model;
            elasticsearch::book::reader r(file_path);

            auto&& model_value = r.to_model(tracer);
            bin::data_manipulation::extract_from_model<data, BOOK_DATA_MODEL_ELEMENTS>(model_value, ret);
            bin::data_manipulation::extract_from_model<data, COMMON_DATA_MODEL_ELEMENTS>(model_value, ret);
            ret["schema"] = schema_indices[0];
        } catch (const std::exception& ex) {
            throw std::runtime_error(std::string("unsupported format by path: ") + file_path);
        }
    }
    return ret;
}
}

dispatcher::dispatcher(dispatcher_settings s, bool with_cluster_logs, bool with_request_logs) :
    settings(std::move(s)),
    use_request_logging(with_request_logs),
    use_cluster_logging(with_cluster_logs)
{
    init_cluster_dispatchers();
    if (use_request_logging)
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
    return buffered_aux_stream;//std::cerr;
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
    if (use_request_logging)
    {
        details::request_put_data(*this, std::cout, file_path, override_model_params, ignore_existing, txml::StdoutTracer{});
    }
    else
    {
        details::request_put_data(*this, std::cout, file_path, override_model_params, ignore_existing, txml::EmptyTracer{});
    }
}

std::map<std::string, dispatcher::csv_data_t> dispatcher::get_data(const char *index, const char **document_names, size_t document_count)
{
    if (use_request_logging)
    {
        return details::request_get_data(*this, std::cout, index, document_names, document_count, txml::StdoutTracer{});
    }
    else
    {
        return details::request_get_data(*this, std::cout, index, document_names, document_count, txml::EmptyTracer{});
    }
}

void dispatcher::update_data(const char *file_path, const char *updated_document_id,
                             const std::map<std::string, std::string>& override_model_params)
{
    if (use_request_logging)
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
    if (use_request_logging)
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
    if (use_request_logging)
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

void dispatcher::es_info(const char* model_name, std::ostream &out) const
{
    if (!strcmp(model_name, schema_indices[0]))
    {
        bin::v7::details::request_book_search_param_info(*this, out);
    }
    else if (!strcmp(model_name, schema_indices[1]))
    {
        bin::v7::details::request_image_search_param_info(*this, out);
    }
    else
    {
        throw std::runtime_error(std::string("invalid model name: ") + model_name);
    }
}

void dispatcher::book_search_match(const std::map<std::string, std::string>& match_params, const std::map<std::string, std::string>& sort_params)
{
    if (use_request_logging)
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
    if (use_request_logging)
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
        bin::models::print_element_values<data, BOOK_DATA_MODEL_ELEMENTS>(out, *r.second);
        bin::models::print_element_values<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
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
        bin::models::print_element_values<data, IMAGE_DATA_MODEL_ELEMENTS>(out, *r.second);
        bin::models::print_element_values<data, COMMON_DATA_MODEL_ELEMENTS>(out, *r.second);
        out << std::endl;
    }
}
} // namespace v7
} // namespace bin
