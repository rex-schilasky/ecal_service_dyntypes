cmake_minimum_required(VERSION 3.10)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

project(ecal_service_dyntypes)

find_package(eCAL REQUIRED)
find_package(Protobuf REQUIRED)

set(ecal_service_dyntypes_src
    src/ecal_service_dyntypes.cpp
)

set(ecal_service_dyntypes_proto
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/math.proto
)

ecal_add_sample(${PROJECT_NAME} ${ecal_service_dyntypes_src})
PROTOBUF_TARGET_CPP(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf ${ecal_service_dyntypes_proto})

target_link_libraries(${PROJECT_NAME}
    eCAL::core
    protobuf::libprotobuf
)
