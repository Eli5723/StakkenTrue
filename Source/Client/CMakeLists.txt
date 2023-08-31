find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(msdfgen CONFIG REQUIRED)

add_executable(Stakken
"main.cpp"
"Application.cpp"
"gl3w.c"
)

target_link_libraries(Stakken
    SDL2::SDL2main
    SDL2::SDL2
    glm::glm
    spdlog::spdlog
    msdfgen::msdfgen
    msdfgen::msdfgen-ext
    msdfgen::msdfgen-core
)

target_compile_definitions(Stakken PUBLIC IS_CLIENT)