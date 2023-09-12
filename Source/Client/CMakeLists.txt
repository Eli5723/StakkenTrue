find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(msdfgen CONFIG REQUIRED)

add_executable(Stakken
"./main.cpp"
"./Application.cpp"
"./File.cpp"

"./Draw/Quad.cpp"
"./Draw/Shader.cpp"
"./Draw/Viewport.cpp"
"./Draw/Text.cpp"
"./Draw/Background.cpp"

"./Assets/Font.cpp"
"./Assets/Texture.cpp"
"./Assets/Assets.cpp"
"./Assets/MsdfFont.cpp"

"./UI/UI.cpp"
"./UI/Node.cpp"
"./UI/Components.cpp"

"./Game/DrawGame.cpp"
"./Game/Color.cpp"
"./Game/Color.hpp"

"./Input/Gamepad.cpp"
"./Input/Keyboard.cpp"

"./Music.cpp"

"gl3w.c"
)

target_link_libraries(Stakken
    SDL2::SDL2main
    SDL2::SDL2
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    glm::glm
    spdlog::spdlog
    msdfgen::msdfgen
    msdfgen::msdfgen-ext
    msdfgen::msdfgen-core
    StakkenCore
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Stakken PUBLIC IS_CLIENT)