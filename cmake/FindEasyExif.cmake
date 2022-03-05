include(FetchContent)

FetchContent_Declare(easyexif
                     GIT_REPOSITORY     https://github.com/mayanklahiri/easyexif.git
                     GIT_TAG master)

FetchContent_GetProperties(easyexif)
if(NOT easyexif_POPULATED)
  FetchContent_Populate(easyexif)
  set (EASYEXIF_PATH ${easyexif_SOURCE_DIR})
endif()
