#include <boost/test/unit_test.hpp>

import Dice;

BOOST_AUTO_TEST_SUITE(DiceTest)

BOOST_AUTO_TEST_CASE(TestGetSequenceCountForDiceRollCount)
{
    BOOST_TEST(1 == getSequenceCountForDiceRollCount(0));
    BOOST_TEST(6 == getSequenceCountForDiceRollCount(1));
    BOOST_TEST(216 == getSequenceCountForDiceRollCount(3));
}

BOOST_AUTO_TEST_CASE(TestGetSequenceIndexForDiceRolls)
{
    std::vector<unsigned> good0 { 0, 0, 0, 0 };
    BOOST_TEST(0 == *getSequenceIndexForDiceRolls(good0));
    std::vector<unsigned> good1 { 0, 0, 0, 1 };
    BOOST_TEST(1 == *getSequenceIndexForDiceRolls(good1));
    std::vector<unsigned> good216 { 1, 0, 0, 0 };
    BOOST_TEST(216 == *getSequenceIndexForDiceRolls(good216));
    std::vector<unsigned> good1295 { 5, 5, 5, 5 };
    BOOST_TEST(1295 == *getSequenceIndexForDiceRolls(good1295));
    std::vector<unsigned> badOob { 0, 0, 0, 6 };
    BOOST_TEST(!getSequenceIndexForDiceRolls(badOob));
}

BOOST_AUTO_TEST_SUITE_END()
