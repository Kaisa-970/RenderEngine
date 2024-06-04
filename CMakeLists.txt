cmake_minimum_required(VERSION 3.20)

project(PKEngine         
DESCRIPTION "My Render engine based on OpenGL")   

set(CMAKE_CXX_STANDARD 17)
set(ARCHITECTURE "x64")
set(START_PROJECT "PK-Editor")
set(CONFIGURATIONS "Debug" "Release" "Dist")
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}")
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/PKEngine)
set(EDITOR_DIR ${CMAKE_SOURCE_DIR}/PK-Editor)
set(VENDOR_DIR ${ENGINE_DIR}/vendor)

add_subdirectory(PKEngine)
add_subdirectory(PK-Editor)