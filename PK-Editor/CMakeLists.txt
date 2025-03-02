project(PK-Editor)
message("Build project:"${PROJECT_NAME})
message("Vendor dir:" ${VENDOR_DIR})
file(GLOB_RECURSE SOURCES ${EDITOR_DIR}/src/*.cpp ${EDITOR_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES    
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME}    
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME}        
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE     
    ${ENGINE_DIR}/src    
    ${VENDOR_DIR}/spdlog/include    
    ${VENDOR_DIR}/GLFW/include    
    #${VENDOR_DIR}/GLAD/include    
    ${VENDOR_DIR}/imgui    
    ${VENDOR_DIR}/glm    
    ${VENDOR_DIR}/assimp/include    
    ${VENDOR_DIR}/entt/include)

target_link_libraries(${PROJECT_NAME} PRIVATE PKEngine)

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
     
if(WIN32)     
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_PLATFORM_WINDOWS)   
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_PLATFORM_LINUX)    
endif()
    
if(CMAKE_BUILD_TYPE MATCHES Debug)    
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_DEBUG PK_ENABLE_ASSERTS)

elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_RELEASE)
    #target_compile_options(${PROJECT_NAME} PRIVATE /O2)
elseif(CMAKE_BUILD_TYPE MATCHES Dist)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PK_DIST)
    #target_compile_options(${PROJECT_NAME} PRIVATE /O2)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${EDITOR_DIR}
)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${VENDOR_DIR}/assimp/lib/assimp-vc140-mt.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     COMMENT "Copying DLLs to executable directory"
# )