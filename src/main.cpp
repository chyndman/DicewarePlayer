#include <iostream>
#include <random>
#include <cctype>

import Diceware;
import WordList;

constexpr auto COLOR_OFF = "\033[0m";
constexpr auto COLOR_BLUE = "\033[34m";
constexpr auto COLOR_CYAN = "\033[36m";
constexpr auto COLOR_GREEN = "\033[32m";
constexpr auto COLOR_RED = "\033[31m";

void printColored(std::ostream& os, const std::string& str)
{
    auto currColor = COLOR_OFF;
    auto prevColor = COLOR_OFF;
    for (char c : str) {
        if (' ' == c) {
            // No change
        } else if (islower(c)) {
            currColor = COLOR_BLUE;
        } else if (isupper(c)) {
            currColor = COLOR_CYAN;
        } else if (isdigit(c)) {
            currColor = COLOR_RED;
        } else {
            currColor = COLOR_GREEN;
        }

        if (currColor != prevColor) {
            prevColor = currColor;
            os << currColor;
        }
        os << c;
    }
    os << COLOR_OFF;
}

int main(int argc, char* argv[])
{
    DicewarePassphraseProducer<5> pphProd(WORD_LIST_EFF_LONG, std::make_unique<MyDicewarePassphraseMutator>());
    auto rolls = pphProd.getRequiredRollCount();

    if (2 <= argc) {
        unsigned argi = 1;
        unsigned ci = 0;
        while (0 < rolls--) {
            char c = argv[argi][ci];
            if ('\0' == c) {
                if (++argi == static_cast<unsigned>(argc)) {
                    break;
                } else {
                    ci = 0;
                    continue;
                }
            } else if ('1' <= c && c <= '6') {
                pphProd.handleRoll(static_cast<unsigned>(c - '1'));
                rolls--;
            }
            ci++;
        }
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned> dist(0, 5);
        while (0 < rolls--) {
            pphProd.handleRoll(dist(gen));
        }
    }

    if (pphProd.getRollCount() < pphProd.getRequiredRollCount()) {
        std::cerr << "Need " << pphProd.getRequiredRollCount() << " rolls but only got " << pphProd.getRollCount() << std::endl;
    } else {
        printColored(std::cout, pphProd.getPassphrase(true));
        std::cout << std::endl;
    }
}
