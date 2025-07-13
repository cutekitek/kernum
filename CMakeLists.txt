cmake_minimum_required(VERSION 3.28)
project(kernum)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vendor/SDL3-3.0.0")

find_package(SDL3 REQUIRED)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS src/*.hpp src/*.h)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
message("sources are" ${sources})

add_executable(kernum src/main.cpp ${sources} ${headers})

target_link_libraries(kernum PRIVATE SDL3::SDL3)

if(WIN32)
    target_link_libraries(kernum PRIVATE SDL3::SDL3main)
    set_property(TARGET kernum PROPERTY WIN32_EXECUTABLE ON)

    add_custom_command(
            TARGET kernum POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_PREFIX_PATH}/lib/x64/SDL3.dll" # Adjust path for your architecture (x64/x86)
            $<TARGET_FILE_DIR:kernum>
            COMMENT "Copying SDL3.dll to executable directory"
    )
endif()

set(CMAKE_CXX_FLAGS "-O3")


# For macOS, we create an application bundle.
if(APPLE)
    set_property(TARGET kernum PROPERTY MACOSX_BUNDLE ON)
    # For a real app, you would also set MACOSX_BUNDLE_INFO_PLIST and copy resources.
endif()

# A message to show where the final executable will be.
message(STATUS "Executable will be created at: ${CMAKE_BINARY_DIR}")
