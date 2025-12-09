cmake_minimum_required(VERSION 4.0)
project(suroi_client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendored/glimmerite EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SUROI_SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(suroi_client ${SUROI_SRC})

target_link_libraries(glimmerite PRIVATE
    SDL3::SDL3
    bgfx
    bx
    bimg_decode
    glaze::glaze
)
target_link_libraries(suroi_client PRIVATE
    SDL3::SDL3
    bgfx
    bx
    bimg_decode
    glaze::glaze
    glimmerite
)
