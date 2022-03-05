#include <iostream>

#include "interface_impl/upload_data_provider/json_upload_data_provider.hpp"

#include <gtest/gtest.h>

namespace tests
{
TEST(CURL_CPP_Uploader, json_uploader_init_access)
{
    std::string string_json = R"({"sort":{"type":"string","value":999}})";
    nlohmann::json obj = nlohmann::json::parse(string_json);
    ASSERT_EQ(obj.dump(), string_json);

    JSONUploadDataProvider provider(std::move(obj));
    ASSERT_EQ(provider.size(), string_json.size());
    {
        std::string got;
        got.resize(provider.size(), '0');
        ASSERT_EQ(provider.fetch_data(got.data(), got.size()), string_json.size());
        ASSERT_EQ(got, string_json);
    }

    {
        std::string modeled_string;
        {
            auto accessor = provider.access();
            accessor.object()["ext"] = "extended";
            accessor.object()["new"] = "new field";

            modeled_string = accessor.object().dump();
        }
        ASSERT_EQ(provider.size(), modeled_string.size());
        std::string got;
        got.resize(modeled_string.size(), '0');
        ASSERT_EQ(provider.fetch_data(got.data(), got.size()), modeled_string.size());
        ASSERT_EQ(got, modeled_string);
    }
}
}
