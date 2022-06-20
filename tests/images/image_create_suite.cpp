#include <iostream>

#include "tests/common/settings.hpp"
#include "elasticsearch/images/request/image_index_mapping.hpp"
#include "elasticsearch/images/request/create_image.hpp"
#include "elasticsearch/images/request/get_image.hpp"
#include "elasticsearch/v7_10/answer_model/data_creation/deserializer.hpp"
#include "elasticsearch/images/data_model/serializers/serializer.hpp"
#include "elasticsearch/images/data_model/serializers/schema_serializer.hpp"

#include "elasticsearch/images/data_model/model.hpp"

#include "elasticsearch/v7_10/request/index_mapping_delete.hpp"
#include "elasticsearch/images/JPG/model/ExifData.h"
#include "elasticsearch/images/JPG/serializer/image_model_serializer.hpp"

#include <gtest/gtest.h>

namespace tests
{
class ImageCreateFixture : public ::testing::Test,
                          public Settings
{
protected:

    void SetUp() override {
        using namespace elasticsearch::image::index_mapping;
        transaction req(get_host());
        ASSERT_NO_THROW(req.execute(get_index(), false));

        std::optional<transaction::response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->node<model::Ack>());
        ASSERT_EQ(ans_ptr->node<model::Ack>()->value(), true);
    }

    void TearDown() override {
        elasticsearch::v7::index_mapping_delete::transaction delete_index_mapping(get_host());
        delete_index_mapping.execute(get_index(), curl_verbose());
    }
};


TEST_F(ImageCreateFixture, request_jpg)
{
    using namespace elasticsearch::image;
    using namespace elasticsearch::image::model;

    JPG::ExifData jpg_package;
    jpg_package.emplace<elasticsearch::image::model::element::Camera>("xamera");
    jpg_package.emplace<elasticsearch::image::model::element::CameraModel>("xamera_model");
    jpg_package.emplace<elasticsearch::image::model::element::Resolution>(666, 666);
    jpg_package.emplace<JPG::Description>("test data description");

    jpg_package.emplace<elasticsearch::image::model::element::OriginalTime>("2021-01-01");
    jpg_package.emplace<elasticsearch::image::model::element::DigitizeTime>("2021-01-01");
    jpg_package.emplace<elasticsearch::image::model::element::Location>(44.f,
                                                                        55.f,
                                                                        0.05f);

    jpg::to_model_data i2m;
    txml::StdoutTracer std_tracer;
    jpg_package.format_serialize(i2m, std_tracer);

    // postproc
    i2m.data_model->emplace<elasticsearch::common_model::BinaryBlob>(std::vector<uint8_t>{0x1, 0x2, 0x46, 0x19});
    i2m.data_model->emplace<elasticsearch::common_model::OriginalPath>("test_path");

    create::transaction req(get_host());
    ASSERT_NO_THROW(req.execute(get_index() + "/_doc/1", *i2m.data_model,
                                curl_verbose(), std_tracer));

    std::optional<create::transaction::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr->node<::model::Result>());
    ASSERT_EQ(ans_ptr->node<::model::Result>()->value(), "created");
    ASSERT_TRUE(ans_ptr->node<::model::_Version>());
    ASSERT_EQ(ans_ptr->node<::model::_Version>()->value(), 1);
    ASSERT_TRUE(ans_ptr->node<::model::_Id>());
    ASSERT_EQ(ans_ptr->node<::model::_Id>()->value(), "1");
    ASSERT_TRUE(ans_ptr->node<::model::_Index>());
    ASSERT_EQ(ans_ptr->node<::model::_Index>()->value(), get_index());
    ASSERT_TRUE(ans_ptr->node<::model::_Type>());
    ASSERT_EQ(ans_ptr->node<::model::_Type>()->value(), "_doc");



    get::transaction get_image(get_host());
    ASSERT_NO_THROW(get_image.execute(get_index() + "/_doc/1", curl_verbose()));
    std::optional<get::transaction::response> return_model;
    ASSERT_NO_THROW(return_model = get_image.get_response(std_tracer));


    ASSERT_EQ(return_model->node<elasticsearch::common_model::OriginalPath>()->value(),
              i2m.data_model->node<elasticsearch::common_model::OriginalPath>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::common_model::BinaryBlob>()->value(),
              i2m.data_model->node<elasticsearch::common_model::BinaryBlob>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::common_model::Format>()->value(),
              i2m.data_model->node<elasticsearch::common_model::Format>()->value());

    ASSERT_EQ(return_model->node<elasticsearch::image::model::element::Camera>()->value(),
              jpg_package.node<elasticsearch::image::model::element::Camera>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::image::model::element::CameraModel>()->value(),
              jpg_package.node<elasticsearch::image::model::element::CameraModel>()->value());
    ASSERT_EQ(return_model->node<elasticsearch::image::model::element::Resolution>()->value().to_string(),
              jpg_package.node<elasticsearch::image::model::element::Resolution>()->value().to_string());

    ASSERT_EQ(return_model->node<elasticsearch::common_model::Description>()->value(),
              jpg_package.node<JPG::Description>()->value());
}
}
