project(PKEngine)
message("Build project:" ${PROJECT_NAME})
message("Engine dir:" ${ENGINE_DIR})

file(GLOB_RECURSE SOURCES ${ENGINE_DIR}/src/*.cpp ${ENGINE_DIR}/src/*.h)
file(GLOB_RECURSE TS 
    ${VENDOR_DIR}/stb_image/*.cpp
    ${VENDOR_DIR}/stb_image/*.h
    ${VENDOR_DIR}/glm/glm/*.hpp
    ${VENDOR_DIR}/glm/glm/*.inl
    )

list(APPEND SOURCES ${TS})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>:/MTd /Zi /RTC1 /Od> 
        $<$<CONFIG:Release>:/MT /O2>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>:-g> 
        $<$<CONFIG:Release>:-O3>
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES    
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME}    
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME}        
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME})
    
target_precompile_headers(${PROJECT_NAME} PRIVATE ${ENGINE_DIR}/src/pkpch.h)
message("PCH:" ${ENGINE_DIR}/src/pkpch.h)

target_include_directories(${PROJECT_NAME} PRIVATE     
    ${ENGINE_DIR}/src    
    ${VENDOR_DIR}/spdlog/include    
    ${VENDOR_DIR}/GLFW/include    
    ${VENDOR_DIR}/GLAD/include    
    ${VENDOR_DIR}/imgui    
    ${VENDOR_DIR}/glm    
    ${VENDOR_DIR}/assimp/include    
    ${VENDOR_DIR}/entt/include    
    ${VENDOR_DIR}/stb_image)
    
target_link_libraries(${PROJECT_NAME} PRIVATE    
    GLFW    
    Glad    
    opengl32.lib    
    imgui)
     
if(WIN32)     
    message("Platform:WIndows!")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_PLATFORM_WINDOWS PK_BUILD_DLL)       
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)        
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
endif()
    
if(CMAKE_BUILD_TYPE MATCHES Debug)    
    message("Build:Debug!")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_DEBUG PK_ENABLE_ASSERTS)
    #target_link_directories(${PROJECT_NAME} PRIVATE ${VENDOR_DIR}/assimp/lib)    
    target_link_libraries(${PROJECT_NAME} PUBLIC ${VENDOR_DIR}/assimp/lib/assimp-vc140-mt.lib)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message("Build:Release!")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_RELEASE)
    #target_compile_options(${PROJECT_NAME} PRIVATE /O2)
endif()

add_subdirectory(${VENDOR_DIR}/spdlog)
add_subdirectory(${VENDOR_DIR}/GLFW)
add_subdirectory(${VENDOR_DIR}/GLAD)
add_subdirectory(${VENDOR_DIR}/imgui)