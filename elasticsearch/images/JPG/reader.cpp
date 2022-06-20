#include "exif.h"
#include <string>

#include <txml/txml.hpp>

#include "elasticsearch/images/JPG/reader.hpp"
#include "elasticsearch/images/JPG/serializer/image_model_serializer.hpp"
#include "elasticsearch/images/data_model/serializers/serializer.hpp"
#include "elasticsearch/utils/file_to_base64_packer.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/serializer.hpp"
#include "elasticsearch/common_model/serializers/serializer.hpp"

namespace JPG
{
reader::reader(const std::filesystem::path &file_path)
{
    std::unique_ptr<FILE, decltype(&fclose)> packed_file (fopen(file_path.c_str(), "rb"), fclose);
    if (!packed_file)
    {
        throw std::runtime_error(std::string("cannot open compressed filepath: ") + file_path.c_str());
    }
    fseek(packed_file.get(), 0, SEEK_END);
    long file_size = ftell(packed_file.get());
    fseek(packed_file.get(), 0, SEEK_SET);

    std::vector<uint8_t> file_data(file_size, 0);
    size_t total_bytes_read = 0;
    size_t bytes_read = 0;
    do {
        bytes_read = fread(file_data.data() + total_bytes_read,
                           1, file_size - total_bytes_read,
                           packed_file.get());
        if (bytes_read > 0)
        {
            total_bytes_read += bytes_read;
        }
    } while(bytes_read != 0 or errno == EINTR);


    easyexif::EXIFInfo result;
    int res = result.parseFrom(file_data.data(), file_data.size());
    if (res != PARSE_EXIF_SUCCESS)
    {
        throw std::runtime_error("Cannot parse EXIF data, easyexif error: " + std::to_string(res));
    }

    image_data = std::make_optional<ExifData>();
    image_data->emplace<elasticsearch::image::model::element::Camera>(std::string(result.Make.c_str()));
    image_data->emplace<elasticsearch::image::model::element::CameraModel>(result.Model.c_str());
    image_data->emplace<elasticsearch::image::model::element::Resolution>(result.ImageWidth, result.ImageHeight);

    // easyexif::EXIFInfo returns space-filled string in case of empty description
    // check on "empty": if each symbol is space in string then skip it
    std::string fixed_description = result.ImageDescription;
    auto it = std::remove_if(fixed_description.begin(), fixed_description.end(),  [](int x) { return std::isspace(x); });
    auto r = std::distance(it, fixed_description.end());
    fixed_description.erase(it, fixed_description.end());
    if (!fixed_description.empty())
    {
        image_data->emplace<Description>(result.ImageDescription.c_str());
    }
    image_data->emplace<elasticsearch::image::model::element::OriginalTime>(result.DateTimeOriginal.c_str());
    image_data->emplace<elasticsearch::image::model::element::DigitizeTime>(result.DateTimeDigitized.c_str());
    image_data->emplace<elasticsearch::image::model::element::Location>(result.GeoLocation.Latitude,
                                                                        result.GeoLocation.Longitude,
                                                                        result.GeoLocation.Altitude);

    // prepare binary
    packer_impl.reset(new elasticsearch::utils::packer(file_path));
}

reader::~reader() = default;

std::optional<ExifData> reader::getImageModel() const
{
    return image_data;
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
std::optional<elasticsearch::image::model::data> reader::to_model_impl(Tracer tracer) const
{
    elasticsearch::image::model::jpg::to_model_data i2m;
    getImageModel()->format_serialize(i2m, tracer);
    // postproc
    i2m.data_model->insert(getBlob());
    i2m.data_model->insert(getPath());
    return i2m.data_model;
}

std::optional<elasticsearch::image::model::data> reader::to_model(txml::EmptyTracer tracer) const
{
    return to_model_impl(std::move(tracer));
}
std::optional<elasticsearch::image::model::data> reader::to_model(txml::StdoutTracer tracer) const
{
    return to_model_impl(std::move(tracer));
}


template<class Tracer>
nlohmann::json reader::to_json_impl(Tracer tracer) const
{
    auto &&model_value = to_model_impl(tracer);

    elasticsearch::v7::put_json_data::RequestSerializer<elasticsearch::image::model::data,
                                                        elasticsearch::image::to_data,
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
