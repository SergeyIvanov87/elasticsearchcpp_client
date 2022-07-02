#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <zlib.h>
#include <zlib/contrib/minizip/unzip.h>
#include <txml/txml.hpp>

#include "elasticsearch/books/EPUB/epub_reader.hpp"
#include "elasticsearch/books/EPUB/serializer/book_model_serializer.hpp"
#include "elasticsearch/books/data_model/serializers/serializer.hpp"
#include "elasticsearch/utils/file_to_base64_packer.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/serializer.hpp"
#include "elasticsearch/common_model/serializers/serializer.hpp"

namespace EPUB
{
reader::reader(const std::string& file_path)
{
    unzFile epub_file = unzOpen(file_path.c_str());
    if (!epub_file)
    {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    static const char *epub_content_file_name = "content.opf";
    if (UNZ_OK != unzLocateFile(epub_file, epub_content_file_name, 0))
    {
        unzClose(epub_file);
        throw std::runtime_error(std::string("Unsupported EPUB: ") + epub_content_file_name + " is not found");
    }

    unz_file_info file_info {};
    unzGetCurrentFileInfo(epub_file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);

    if (UNZ_OK != unzOpenCurrentFile(epub_file))
    {
        unzClose(epub_file);
        throw std::runtime_error(std::string("Cannot open compressed file: ") + epub_content_file_name);
    }

    // read file into buffer
    std::string opf_data;
    unsigned len = 4096;
    int err = 0;
    std::unique_ptr<char[]> read_buffer(new char[len]);
    do
    {
        err = unzReadCurrentFile(epub_file, read_buffer.get(), len);
        if (err > 0)
        {
            opf_data += read_buffer.get();
        }
    } while(err > 0);
    if (err != 0)
    {
        unzCloseCurrentFile(epub_file);
        unzClose(epub_file);
        throw std::runtime_error(std::string("Cannot read compressed file, error: ") + std::to_string(err));
    }


    if (UNZ_OK != unzCloseCurrentFile(epub_file))
    {
        unzClose(epub_file);
        throw std::runtime_error(std::string("Cannot close compressed file: ") + epub_content_file_name);
    }
    unzClose(epub_file);


    // open xml text reader
    txml::TextReaderWrapper reader(opf_data.data(), opf_data.size(), nullptr, nullptr, 0);

    if (!reader.read())
    {
        throw std::runtime_error(std::string("Cannot read file content: ") + epub_content_file_name);
    }

    auto &&pack = txml::XMLCreator::try_create<Package>(reader);
    if (!pack)
    {
        throw std::runtime_error(std::string("Cannot parse content in epub::Package model: ") + epub_content_file_name);
    }

    open_packaging_format_ptr = std::move(pack.value());
    // prepare binary
    packer_impl.reset(new elasticsearch::utils::packer(file_path));
}

reader::~reader() = default;

const Package& reader::getOPF() const
{
    return open_packaging_format_ptr;
}

const elasticsearch::common_model::BinaryBlob& reader::getBlob() const
{
    return packer_impl->getBlob();
}

const elasticsearch::common_model::OriginalPath& reader::getPath() const
{
    return packer_impl->getPath();
}

void reader::pack(const std::filesystem::path &path_to_pack)
{
    packer_impl->pack(path_to_pack);
}


template<class Tracer>
elasticsearch::book::model::data reader::to_model_impl(Tracer tracer) const
{
    elasticsearch::book::model::epub::to_model_data i2m;
    getOPF().format_serialize(i2m, tracer);
    // postproc
    i2m.data_model->insert(getBlob());
    i2m.data_model->insert(getPath());
    return i2m.data_model.value();
}

elasticsearch::book::model::data reader::to_model(txml::EmptyTracer tracer) const
{
    return to_model_impl(std::move(tracer));
}
elasticsearch::book::model::data reader::to_model(txml::StdoutTracer tracer) const
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

    model_value.template format_serialize(serializer, tracer);
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
