module;
#include <span>
#include <optional>

export module Dice;

export constexpr unsigned DICE_CUBE_FACE_COUNT = 6;

export constexpr unsigned getSequenceCountForDiceRollCount(unsigned rolls, unsigned faceCount = DICE_CUBE_FACE_COUNT)
{
    unsigned count = 1;
    while (0 < rolls--) {
        count *= faceCount;
    }
    return count;
}

export std::optional<unsigned> getSequenceIndexForDiceRolls(const std::span<unsigned>& faceIndices, unsigned faceCount = DICE_CUBE_FACE_COUNT)
{
    unsigned si = 0;
    for (auto fi : faceIndices) {
        if (fi >= faceCount) {
            return {};
        }
        si = (si * faceCount) + fi;
    }
    return si;
}
