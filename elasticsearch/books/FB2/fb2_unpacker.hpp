#ifndef ELASTIC_SEARCH_BOOK_FB2_UNPACKER_HPP
#define ELASTIC_SEARCH_BOOK_FB2_UNPACKER_HPP

#include <memory>
#include <string>
//#include <txml/txml.hpp>
#include "elasticsearch/common_model/BinaryBlob.h"
#include "elasticsearch/common_model/OriginalPath.h"

namespace fb2
{
class unpacker
{
public:
    unpacker(const elasticsearch::common_model::BinaryBlob& blob,
             const std::string& file_name);
    unpacker(const elasticsearch::common_model::BinaryBlob& blob,
             const elasticsearch::common_model::OriginalPath& restore_path);

    const std::string& getFilePath() const;
private:
    std::string restored_file_path;
};
}
#endif // ELASTIC_SEARCH_BOOK_FB2_UNPACKER_HPP
