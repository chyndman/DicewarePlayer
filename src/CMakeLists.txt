set(DWP_MODULES
    Dice
    WordList
    Diceware)
foreach(mod IN LISTS DWP_MODULES)
    add_library(${mod})
    target_sources(${mod} PUBLIC FILE_SET CXX_MODULES FILES ${mod}.cppm)
endforeach()
target_link_libraries(Diceware PRIVATE Dice)

add_executable(DicewarePlayer main.cpp)
target_link_libraries(DicewarePlayer PRIVATE Diceware WordList)
