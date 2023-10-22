#ifndef COMMON_DATA_MODEL_MODEL_HPP
#define COMMON_DATA_MODEL_MODEL_HPP

#include "elasticsearch/common_model/BinaryBlob.h"
#include "elasticsearch/common_model/CreationTime.h"
#include "elasticsearch/common_model/Description.h"
#include "elasticsearch/common_model/Format.h"
#include "elasticsearch/common_model/OriginalPath.h"
#include "elasticsearch/common_model/Preview.h"
#include "elasticsearch/common_model/SchemaVersion.h"
#include "elasticsearch/common_model/SourceName.h"
#include "elasticsearch/common_model/Tag.h"

#define COMMON_DATA_MODEL_ELEMENTS          elasticsearch::common_model::BinaryBlob,               \
                                            elasticsearch::common_model::CreationDateTime,         \
                                            elasticsearch::common_model::Description,              \
                                            elasticsearch::common_model::Format,                   \
                                            elasticsearch::common_model::OriginalPath,             \
                                            elasticsearch::common_model::Preview,                  \
                                            elasticsearch::common_model::SchemaVersion,            \
                                            elasticsearch::common_model::SourceName,               \
                                            elasticsearch::common_model::Tags

#endif // COMMON_DATA_MODEL_MODEL_HPP
