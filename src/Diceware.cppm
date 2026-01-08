module;
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <span>

export module Diceware;

import Dice;

export class DicewarePassphraseMutator {
public:
    virtual ~DicewarePassphraseMutator() {}
    virtual size_t getRequiredRollCount() const = 0;
    virtual bool apply(std::vector<std::string>& terms, const std::span<unsigned>& rolls) const = 0;
};

export class MyDicewarePassphraseMutator : public DicewarePassphraseMutator {
public:
    virtual size_t getRequiredRollCount() const override;
    virtual bool apply(std::vector<std::string>& terms, const std::span<unsigned>& rolls) const override;
};

export template <size_t KWordRollCount>
class DicewarePassphraseProducer {
public:
    static constexpr size_t k_seqCount = getSequenceCountForDiceRollCount(KWordRollCount);
    static constexpr size_t k_wordCount = 6;

    explicit DicewarePassphraseProducer(const std::array<const char*, k_seqCount>& wordList, std::unique_ptr<DicewarePassphraseMutator> mutator);
    std::string getPassphrase(bool spaces = false) const;
    
    size_t getRollCount() const;
    size_t getRequiredRollCount() const;
    bool handleRoll(unsigned faceIndex);

protected:
    const std::array<const char*, k_seqCount>& m_wordList;
    std::unique_ptr<DicewarePassphraseMutator> m_mutator;
    std::vector<std::string> m_terms;
    unsigned m_wordsRollCount = 0;
    std::array<unsigned, KWordRollCount> m_wordRollBuf;
    std::vector<unsigned> m_mutatorRollBuf;
};

template <size_t KWordRollCount>
DicewarePassphraseProducer<KWordRollCount>::DicewarePassphraseProducer(const std::array<const char*, k_seqCount>& wordList, std::unique_ptr<DicewarePassphraseMutator> mutator)
    : m_wordList(wordList)
    , m_mutator(std::move(mutator))
{
    if (m_mutator) {
        m_mutatorRollBuf.reserve(m_mutator->getRequiredRollCount());
    }
}

template <size_t KWordRollCount>
std::string DicewarePassphraseProducer<KWordRollCount>::getPassphrase(bool spaces) const
{
    std::string pph;
    for (auto t : m_terms) {
        pph.append(t);
        if (spaces && !pph.empty()) {
            pph.push_back(' ');
        }
    }
    return pph;
}

template <size_t KWordRollCount>
size_t DicewarePassphraseProducer<KWordRollCount>::getRollCount() const
{
    return m_wordsRollCount + m_mutatorRollBuf.size();
}

template <size_t KWordRollCount>
size_t DicewarePassphraseProducer<KWordRollCount>::getRequiredRollCount() const
{
    return (k_wordCount * KWordRollCount) + (m_mutator ? m_mutator->getRequiredRollCount() : 0);
}

template <size_t KWordRollCount>
bool DicewarePassphraseProducer<KWordRollCount>::handleRoll(unsigned faceIndex)
{
    if (faceIndex >= DICE_CUBE_FACE_COUNT || getRequiredRollCount() <= getRollCount()) {
        return false;
    }

    if (m_wordsRollCount < k_wordCount * KWordRollCount) {
        unsigned wordsRollBufIndex = m_wordsRollCount % KWordRollCount;
        m_wordRollBuf[wordsRollBufIndex] = faceIndex;
        if (wordsRollBufIndex == KWordRollCount - 1) {
            auto wordIndex = getSequenceIndexForDiceRolls(m_wordRollBuf).value();
            auto word = m_wordList[wordIndex];
            m_terms.push_back(std::string(word));
        }
        m_wordsRollCount++;
    } else if (m_mutator && m_mutatorRollBuf.size() < m_mutator->getRequiredRollCount()) {
        m_mutatorRollBuf.push_back(faceIndex);
        if (m_mutatorRollBuf.size() == m_mutator->getRequiredRollCount()) {
            m_mutator->apply(m_terms, m_mutatorRollBuf);
        }
    }
    return true;
}

size_t MyDicewarePassphraseMutator::getRequiredRollCount() const
{
    return 4;
}

bool MyDicewarePassphraseMutator::apply(std::vector<std::string>& terms, [[maybe_unused]] const std::span<unsigned>& rolls) const
{
    if (rolls.size() != 4 || terms.size() < 6) {
        return false;
    }

    size_t seqCount = getSequenceCountForDiceRollCount(4);
    auto seqIndexOpt = getSequenceIndexForDiceRolls(rolls);
    if (!seqIndexOpt) {
        return false;
    }
    auto seqIndex = seqIndexOpt.value();

    auto fnTapIndex = [&seqCount, &seqIndex](size_t count) -> size_t {
        auto div = seqCount / count;
        auto result = seqIndex / div;
        seqCount = div;
        seqIndex %= div;
        return result;
    };

    static constexpr std::array<size_t, 2> capitalizePositions { 0, 3 };
    static constexpr std::array<size_t, 4> suffixPositions { 1, 2, 4, 5 };
    static constexpr std::array<char, 6> symbols { '@', '&', '$', '!', '#', '?' };

    auto capitalizePositionIndex = fnTapIndex(capitalizePositions.size());
    auto symbolPositionIndex = fnTapIndex(suffixPositions.size());
    auto digitPositionIndex = (symbolPositionIndex + 1 + fnTapIndex(suffixPositions.size() - 1)) % suffixPositions.size();
    auto symbolIndex = fnTapIndex(symbols.size());
    char digit = '1' + static_cast<char>(fnTapIndex(9));

    auto capitalizePosition = capitalizePositions[capitalizePositionIndex];
    auto symbolPosition = suffixPositions[symbolPositionIndex];
    auto digitPosition = suffixPositions[digitPositionIndex];
    auto symbol = symbols[symbolIndex];
    
    char c = terms[capitalizePosition][0];
    if ('a' <= c && c <= 'z') {
        c -= ('a' - 'A');
        terms[capitalizePosition][0] = c;
    }
    terms[symbolPosition].push_back(symbol);
    terms[digitPosition].push_back(digit);

    return true;
}
