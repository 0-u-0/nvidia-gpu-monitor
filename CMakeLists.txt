cmake_minimum_required(VERSION 3.10.2)

set (CMAKE_CXX_STANDARD 14)

project (gpu_monitor)
include_directories(${CMAKE_JS_INC})

find_package(CUDA)
find_package(PkgConfig)

pkg_check_modules(NVIDIAML REQUIRED IMPORTED_TARGET nvidia-ml-11.2)


file(GLOB SOURCE_FILES "internal/*.cc" "internal/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Release/)
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})


if(CUDA_FOUND)
    message("CUDA FOUND")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GPU_ENABLED)
    # third party libraries
   
    target_link_libraries(${PROJECT_NAME} PkgConfig::NVIDIAML)
else()
    message("NO CUDA")
endif(CUDA_FOUND)

#FOR IDE, IDE_INC=CMAKE_JS_INC : ./node_modules/.bin/cmake-js print-configure
if(DEFINED ENV{IDE_INC})
    message(STATUS "IDE_INC = " $ENV{IDE_INC})
    target_include_directories(${PROJECT_NAME} PRIVATE $ENV{IDE_INC})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "internal/service")
