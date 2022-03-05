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

        std::shared_ptr<transaction::response> ans_ptr;
        ASSERT_NO_THROW(ans_ptr = req.get_response());
        ASSERT_TRUE(ans_ptr->getValue<model::Ack>());
        ASSERT_EQ(ans_ptr->getValue<model::Ack>()->getValue(), true);
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

    std::shared_ptr<create::transaction::response> ans_ptr;
    ASSERT_NO_THROW(ans_ptr = req.get_response(std_tracer));
    ASSERT_TRUE(ans_ptr->getValue<::model::Result>());
    ASSERT_EQ(ans_ptr->getValue<::model::Result>()->getValue(), "created");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Version>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Version>()->getValue(), 1);
    ASSERT_TRUE(ans_ptr->getValue<::model::_Id>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Id>()->getValue(), "1");
    ASSERT_TRUE(ans_ptr->getValue<::model::_Index>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Index>()->getValue(), get_index());
    ASSERT_TRUE(ans_ptr->getValue<::model::_Type>());
    ASSERT_EQ(ans_ptr->getValue<::model::_Type>()->getValue(), "_doc");



    get::transaction get_image(get_host());
    ASSERT_NO_THROW(get_image.execute(get_index() + "/_doc/1", curl_verbose()));
    std::shared_ptr<get::transaction::response> return_model;
    ASSERT_NO_THROW(return_model = get_image.get_response(std_tracer));


    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue(),
              i2m.data_model->getValue<elasticsearch::common_model::OriginalPath>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue(),
              i2m.data_model->getValue<elasticsearch::common_model::BinaryBlob>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::Format>()->getValue(),
              i2m.data_model->getValue<elasticsearch::common_model::Format>()->getValue());

    ASSERT_EQ(return_model->getValue<elasticsearch::image::model::element::Camera>()->getValue(),
              jpg_package.getValue<elasticsearch::image::model::element::Camera>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::image::model::element::CameraModel>()->getValue(),
              jpg_package.getValue<elasticsearch::image::model::element::CameraModel>()->getValue());
    ASSERT_EQ(return_model->getValue<elasticsearch::image::model::element::Resolution>()->getValue().to_string(),
              jpg_package.getValue<elasticsearch::image::model::element::Resolution>()->getValue().to_string());

    ASSERT_EQ(return_model->getValue<elasticsearch::common_model::Description>()->getValue(),
              jpg_package.getValue<JPG::Description>()->getValue());
}
}
