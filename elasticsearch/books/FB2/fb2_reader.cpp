#include <txml/applications/fb2/fb2.hpp>

#include "elasticsearch/books/FB2/fb2_reader.hpp"
#include "elasticsearch/books/FB2/fb2_packer.hpp"
#include "elasticsearch/books/FB2/serializer/book_model_serializer.hpp"
#include "elasticsearch/books/data_model/serializers/serializer.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/serializer.hpp"
#include "elasticsearch/common_model/serializers/serializer.hpp"

namespace fb2
{
reader::reader(const std::string& file_path)
{
    txml::TextReaderWrapper xml_reader(file_path);
    if (!xml_reader.read())
    {
        throw std::runtime_error(std::string("Cannot read file content: ") + file_path);
    }

    book_ptr = txml::XMLCreator::try_create<ShortFictionBook>(xml_reader);
    if (!book_ptr)
    {
        throw std::runtime_error("Cannot restore FB2 book from file: " + file_path);
    }

    // prepare binary
    packer_impl.reset(new packer(file_path));
}

reader::~reader() = default;

std::optional<fb2::ShortFictionBook> reader::getBook() const
{
    return book_ptr;
}
std::optional<elasticsearch::common_model::BinaryBlob> reader::getBlob() const
{
    return packer_impl->getBlob();
}

std::optional<elasticsearch::common_model::OriginalPath> reader::getPath() const
{
    return packer_impl->getPath();
}

void reader::pack(const std::filesystem::path &path_to_pack)
{
    packer_impl->pack(path_to_pack);
}
template<class Tracer>
std::optional<elasticsearch::book::model::data> reader::to_model_impl(Tracer tracer) const
{
    elasticsearch::book::model::fb2::to_model_data i2m;
    getBook()->format_serialize(i2m, tracer);
    // postproc
    i2m.data_model->insert(getBlob());
    i2m.data_model->insert(getPath());
    return i2m.data_model;
}

std::optional<elasticsearch::book::model::data> reader::to_model(txml::EmptyTracer tracer) const
{
    return to_model_impl(std::move(tracer));
}
std::optional<elasticsearch::book::model::data> reader::to_model(txml::StdoutTracer tracer) const
{
    return to_model_impl(std::move(tracer));
}

template<class Tracer>
nlohmann::json reader::to_json_impl(Tracer tracer) const
{
    auto &&model_value = to_model_impl(tracer);
    elasticsearch::v7::put_json_data::RequestSerializer<elasticsearch::book::model::data,
                                                        elasticsearch::book::to_data,
                                                        elasticsearch::common_model::to_data> serializer;

    model_value->template format_serialize(serializer, tracer);
    return serializer.finalize(tracer);
}

nlohmann::json reader::to_json(txml::EmptyTracer tracer) const
{
    return to_json_impl(std::move(tracer));
}
nlohmann::json reader::to_json(txml::StdoutTracer tracer) const
{
    return to_json_impl(std::move(tracer));
}
}
