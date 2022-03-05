#ifndef COMMON_DATA_MODEL_SCHEMA_DECORATOR_H
#define COMMON_DATA_MODEL_SCHEMA_DECORATOR_H

#include <txml/applications/json/json.hpp>

#include "elasticsearch/common_model/model.hpp"

namespace elasticsearch
{
namespace common_model
{
namespace decorator
{
/*inline const nlohmann::json &make_keyword()
{
    static nlohmann::json::object kw
    (
        {{"fields", {
            {"keyword", {
                {"type", "keyword"}
            }},
        }}}
    );
    return kw;
}
/ **/
inline const nlohmann::json &make_keyword()
{
    static nlohmann::json kw
    {
        {"fields", {
            {"keyword", {
                {"type", "keyword"}
            }},
        }},
    };
    return kw;
}
}
}
}
#endif // COMMON_DATA_MODEL_SCHEMA_DECORATOR_H
