#ifndef INTERFACES_PACKER_HPP
#define INTERFACES_PACKER_HPP

#include <filesystem>
#include <memory>

#include "elasticsearch/common_model/BinaryBlob.h"
#include "elasticsearch/common_model/OriginalPath.h"

namespace elasticsearch
{
struct packer_interface
{
    virtual ~packer_interface() = default;
    virtual void pack(const std::filesystem::path &path_to_pack) = 0;
    virtual std::shared_ptr<elasticsearch::common_model::BinaryBlob> getBlob() const = 0;
    virtual std::shared_ptr<elasticsearch::common_model::OriginalPath> getPath() const = 0;
};
}
#endif // INTERFACES_PACKER_HPP12
