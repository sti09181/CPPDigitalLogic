////////////////////
///// Main.cpp /////
////////////////////

#include "QuineMcCluskey.hpp"

int main()
{
    std::vector<std::vector<int>> inputs =
    {
        { 4, 10, 0, 1, 4, 5, 13, 15, 10, 11 },
        { 4, 12, 0, 1, 2, 3, 4, 6, 8, 9, 10, 11, 12, 14 },
        { 4, 10, 0, 1, 3, 4, 5, 7, 8, 10, 12, 14},
        { 4, 9, 4, 5, 6, 8, 9, 10, 12, 13, 14 },
        { 4, 11, 0, 1, 3, 4, 5, 7, 8, 12, 13, 14, 15},
        { 5, 18, 0, 1, 3, 4, 5, 7, 8, 12, 13, 14, 15, 17, 22, 23, 25, 27, 31 },
        { 5, 24, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 17, 20, 21, 22, 23, 25, 26, 27, 30, 31 },
        { 5, 25, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27, 30, 31 },
    };

    for (const auto& currentInput : inputs)
    {
        std::size_t countOfVariables = currentInput[0];
        std::set<int> decimalFormOfMinterms(currentInput.begin() + 2, currentInput.end());
        std::set<int> decimalFormOfDontCareTerms;

        solution(countOfVariables, decimalFormOfMinterms, decimalFormOfDontCareTerms);
    }

    return EXIT_SUCCESS;
}
