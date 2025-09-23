cmake_minimum_required(VERSION 3.21)
project(tictactoe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tictactoe
    src/main.cpp
    src/Pokedex.cpp
    src/SFMLHelpers.cpp
    src/Game.cpp
    src/StateMachine.cpp
    src/IntroState.cpp
    src/TeamSelectState.cpp
    src/PreBattleState.cpp
    src/BattleState.cpp
    src/VictoryState.cpp
    src/ContinueState.cpp
)

target_include_directories(tictactoe PRIVATE include)

find_package(SFML 2.5 COMPONENTS graphics window system audio)
if (SFML_FOUND)
  target_link_libraries(tictactoe PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
endif()

