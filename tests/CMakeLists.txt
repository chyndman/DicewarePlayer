add_executable(BoostTests BoostTests.cpp)
target_link_libraries(BoostTests PRIVATE Boost::unit_test_framework)

set(DWP_MODULE_TESTS
    Dice
    Diceware)
foreach(mt IN LISTS DWP_MODULE_TESTS)
    target_sources(BoostTests PRIVATE ${mt}Test.cpp)
    target_link_libraries(BoostTests PRIVATE ${mt})
    add_test(NAME ${mt}Test COMMAND BoostTests -t ${mt}Test) 
endforeach()
target_link_libraries(BoostTests PRIVATE WordList)
