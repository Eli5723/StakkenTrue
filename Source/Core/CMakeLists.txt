add_library(StakkenCore SHARED
"./Game/Game.cpp"
"./Game/Game.hpp"
"./Game/Pieces.cpp"
"./Game/Pieces.hpp"
"./Randomizers/XoroshiroRandomizer.cpp"
"./Randomizers/XoroshiroRandomizer.hpp"

"./Randomizers/SevenBag.cpp"
"./Randomizers/SevenBag.hpp"


"./Game/EventBuffer.cpp"
)

target_include_directories(StakkenCore PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)