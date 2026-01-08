#include <iostream>
#include <random>

import Diceware;
import WordList;

std::string createRandomPassphrase()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dist(0, 5);

    DicewarePassphraseProducer<5> pphProd(WORD_LIST_EFF_LONG, std::make_unique<MyDicewarePassphraseMutator>());
    
    size_t rolls = pphProd.getRequiredRollCount();
    while (0 < rolls--) {
        pphProd.handleRoll(dist(gen));
    }

    return pphProd.getPassphrase(true);
}

int main(int argc, char* argv[])
{
    if (2 <= argc) {
        DicewarePassphraseProducer<5> pphProd(WORD_LIST_EFF_LONG, std::make_unique<MyDicewarePassphraseMutator>());
        auto len = strlen(argv[1]);
        for (unsigned i = 0; i < len; i++) {
            char c = argv[1][i];
            if ('1' <= c && c <= '6') {
                pphProd.handleRoll(static_cast<unsigned>(c - '1'));
                if (pphProd.getRollCount() >= pphProd.getRequiredRollCount()) {
                    std::cout << pphProd.getPassphrase(true) << std::endl;
                    return 0;
                }
            }
        }
        std::cerr << "Need " << pphProd.getRequiredRollCount() << " rolls but only got " << pphProd.getRollCount() << std::endl;
    } else {
        std::cout << createRandomPassphrase() << std::endl;
    }
}
