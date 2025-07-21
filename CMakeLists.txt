cmake_minimum_required(VERSION 3.28)
project(kernum)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS src/*.hpp src/*.h)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.c)
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(kernum src/main.cpp ${sources} ${headers})

if (WIN32)
    target_compile_options(kernum PRIVATE -static-libstdc++ -static-libgcc)
    target_link_options(kernum PRIVATE -static-libstdc++ -static-libgcc)
    list(APPEND CMAKE_PREFIX_PATH vendor/SDL3-3.2.16/cmake vendor/SDL3_image-3.2.4/cmake)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
    find_package(SDL3_image REQUIRED CONFIG COMPONENTS SDL3_image-shared)
    target_link_libraries(kernum PRIVATE ${SDL3_LIBRARIES} SDL3_image::SDL3_image)
else()
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
target_link_libraries(kernum PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
endif()

target_include_directories(kernum PRIVATE src/include)

# For macOS, we create an application bundle.
if(APPLE)
    set_property(TARGET kernum PROPERTY MACOSX_BUNDLE ON)
    # For a real app, you would also set MACOSX_BUNDLE_INFO_PLIST and copy resources.
endif()

# A message to show where the final executable will be.
message(STATUS "Executable will be created at: ${CMAKE_BINARY_DIR}")
