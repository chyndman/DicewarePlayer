#include <boost/test/unit_test.hpp>

import Diceware;
import WordList;

BOOST_AUTO_TEST_SUITE(DicewareTest)

BOOST_AUTO_TEST_CASE(TestStdScenario1)
{
    DicewarePassphraseProducer<5> pphProd(WORD_LIST_EFF_LONG, nullptr);
    BOOST_TEST(0 == pphProd.getRollCount());
    BOOST_TEST(6 * 5 == pphProd.getRequiredRollCount());
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(1 == pphProd.getRollCount());
    BOOST_TEST("" == pphProd.getPassphrase());
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(pphProd.handleRoll(1));
    BOOST_TEST(5 == pphProd.getRollCount());
    BOOST_TEST("abdomen" == pphProd.getPassphrase());
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST("abdomen" == pphProd.getPassphrase());
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST(pphProd.handleRoll(1));
    BOOST_TEST(pphProd.handleRoll(0));
    BOOST_TEST("abdomenablaze" == pphProd.getPassphrase());
}

BOOST_AUTO_TEST_CASE(TestMyScenario1)
{
    DicewarePassphraseProducer<5> pphProd(WORD_LIST_EFF_LONG, std::make_unique<MyDicewarePassphraseMutator>());
    BOOST_TEST(0 == pphProd.getRollCount());
    BOOST_TEST(6 * 5 + 4 == pphProd.getRequiredRollCount());
    for (unsigned i = 0; i < 30; i++) {
        BOOST_TEST(pphProd.handleRoll(i % 6));
    }
    BOOST_TEST("arousalsuggeststadiumprozacipaddispatch" == pphProd.getPassphrase());
    for (unsigned i = 0; i < 4; i++) {
        BOOST_TEST(pphProd.handleRoll(i % 6));
    }
    BOOST_TEST("Arousalsuggest?stadium7prozacipaddispatch" == pphProd.getPassphrase());
}

BOOST_AUTO_TEST_SUITE_END()
