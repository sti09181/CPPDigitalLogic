//////////////////////////////
///// QuineMcCluskey.hpp /////
//////////////////////////////

#pragma once
#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <set>
#include <map>

std::ostream& operator<<(std::ostream& os, const std::set<std::string>& target)
{
    for (auto currentKey : target)
    {
        std::replace(currentKey.begin(), currentKey.end(), '2', '-');

        os << currentKey << " ";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::set<std::string>>& target)
{
    os << "--------------------------------------------------" << std::endl;

    for (const auto& currentTarget : target)
    {
        auto currentKey = currentTarget.first;
        const auto& currentMappedSet = currentTarget.second;

        std::replace(currentKey.begin(), currentKey.end(), '2', '-');

        os << currentKey << ": " << currentMappedSet << std::endl;
    }

    os << "--------------------------------------------------";

    return os;
}

namespace QuineMcCluskey
{
    std::string decimalToBinary(int decimalFormOfTerms, const std::size_t countOfVariables)
    {
        std::string binaryFormOfTerms;

        while (decimalFormOfTerms)
        {
            if (decimalFormOfTerms & 1)
                binaryFormOfTerms += '1';
            else
                binaryFormOfTerms += '0';

            decimalFormOfTerms >>= 1;
        }

        std::reverse(binaryFormOfTerms.begin(), binaryFormOfTerms.end());

        if (binaryFormOfTerms.size() < countOfVariables)
        {
            std::string forSizeBalance;

            for (std::size_t countForSizeBalance = 0, sizeDifference = countOfVariables - binaryFormOfTerms.size(); countForSizeBalance < sizeDifference; countForSizeBalance++)
                forSizeBalance += '0';

            binaryFormOfTerms = forSizeBalance + binaryFormOfTerms;
        }

        return binaryFormOfTerms;
    }

    std::map<std::string, std::set<std::string>> swapKeyAndMappedSet(const std::map<std::string, std::set<std::string>>& target)
    {
        std::map<std::string, std::set<std::string>> result;

        for (const auto& [currentKey, currentMappedSet] : target)
            for (const auto& currentElementOfMappedSet : currentMappedSet)
                result[currentElementOfMappedSet].emplace(currentKey);

        return result;
    }

    std::vector<std::string> getFinalResult(const std::map<std::string, std::set<std::string>>& target)
    {
        std::vector<std::string> result;

        result.reserve(result.size() + target.size());

        for (const auto& [currentTarget, mintermsForCurrentTarget] : target)
            result.emplace_back(currentTarget);

        if (!std::is_sorted(result.begin(), result.end()))
            std::sort(result.begin(), result.end());

        for (auto& currentResult : result)
            std::replace(currentResult.begin(), currentResult.end(), '2', '-');

        return result;
    }

    std::map<std::string, std::set<std::string>> findPI(const std::set<std::string>& binaryFormOfMinterms, const std::set<std::string>& binaryFormOfDontCareTerms, const std::size_t countOfVariables)
    {
        auto combineOperation = [&countOfVariables](const std::map<std::string, std::set<std::string>>& target)
                -> std::tuple<std::map<std::string, std::set<std::string>>, std::map<std::string, std::set<std::string>>>
        {
            std::map<std::string, std::set<std::string>> result;
            std::map<std::string, std::set<std::string>> reminder(target);

            for (const auto& [currentImplicant, mintermsForCurrentImplicant] : target)
            {
                for (const auto& [anotherImplicant, mintermsForAnotherImplicant] : target)
                {
                    if (currentImplicant == anotherImplicant)
                        continue;

                    int differenceOfVariables = 0;

                    for (std::size_t currentVariable = 0; currentVariable < countOfVariables; currentVariable++)
                        if (currentImplicant[currentVariable] != anotherImplicant[currentVariable])
                            differenceOfVariables++;

                    if (differenceOfVariables == 1)
                    {
                        std::string combinedImplicant;

                        std::set<std::string> mintermsForCombinedImplicant;

                        for (std::size_t currentVariable = 0; currentVariable < countOfVariables; currentVariable++)
                        {
                            if (currentImplicant[currentVariable] != anotherImplicant[currentVariable])
                                combinedImplicant += '2';
                            else
                                combinedImplicant += currentImplicant[currentVariable];
                        }

                        auto currentImplicantIterator = reminder.find(currentImplicant);

                        if (currentImplicantIterator != reminder.end())
                            reminder.erase(currentImplicantIterator);

                        mintermsForCombinedImplicant.insert(mintermsForCurrentImplicant.begin(), mintermsForCurrentImplicant.end());
                        mintermsForCombinedImplicant.insert(mintermsForAnotherImplicant.begin(), mintermsForAnotherImplicant.end());

                        result.emplace(combinedImplicant, mintermsForCombinedImplicant);
                    }
                }
            }

            return { result, reminder };
        };

        std::map<std::string, std::set<std::string>> target;
        std::map<std::string, std::set<std::string>> reminder;
        std::map<std::string, std::set<std::string>> result;

        for (const auto& currentBinaryFormOfMinterm : binaryFormOfMinterms)
            target.emplace(currentBinaryFormOfMinterm, std::initializer_list<std::string>{ currentBinaryFormOfMinterm });

        for (const auto& currentBinaryFormOfDontCareTerm : binaryFormOfDontCareTerms)
            target.emplace(currentBinaryFormOfDontCareTerm, std::initializer_list<std::string>{ currentBinaryFormOfDontCareTerm });

        while (!target.empty())
        {
            std::tie(target, reminder) = combineOperation(target);
            result.insert(reminder.begin(), reminder.end());
        }

        result = QuineMcCluskey::swapKeyAndMappedSet(result);

        for (const auto& currentBinaryFormOfDontCareTerm : binaryFormOfDontCareTerms)
        {
            auto currentBinaryFormOfDontCareTermIterator = result.find(currentBinaryFormOfDontCareTerm);

            if (currentBinaryFormOfDontCareTermIterator != result.end())
                result.erase(currentBinaryFormOfDontCareTermIterator);
        }

        return QuineMcCluskey::swapKeyAndMappedSet(result);
    }

    std::map<std::string, std::set<std::string>> findEPI(const std::map<std::string, std::set<std::string>>& PI)
    {
        std::map<std::string, std::set<std::string>> result;

        for (const auto& [currentMinterm, PIsForCurrentMinterm] : QuineMcCluskey::swapKeyAndMappedSet(PI))
            if (PIsForCurrentMinterm.size() == 1)
            {
                auto currentEPI = *PIsForCurrentMinterm.begin();
                result.emplace(currentEPI, PI.at(currentEPI));
            }

        return result;
    }

    namespace MinimizeCover
    {
        std::map<std::string, std::set<std::string>> findNEPI(const std::map<std::string, std::set<std::string>>& PI, const std::map<std::string, std::set<std::string>>& EPI)
        {
            std::map<std::string, std::set<std::string>> result(PI);

            for (const auto& [currentEPI, mintermsForCurrentEPI] : EPI)
            {
                auto currentEPIIterator = result.find(currentEPI);

                if (currentEPIIterator != result.end())
                    result.erase(currentEPIIterator);
            }

            return result;
        }

        std::map<std::string, std::set<std::string>> deleteInterchangable(const std::map<std::string, std::set<std::string>>& target)
        {
            std::map<std::string, std::set<std::string>> result(target);

            for (const auto& [currentKey, currentMappedSet] : target)
            {
                for (const auto& [anotherKey, anotherMappedSet] : target)
                {
                    if (currentKey == anotherKey or currentMappedSet.size() != anotherMappedSet.size())
                        continue;

                    bool isInterchangeable = true;

                    for (const auto& currentAnotherMappedSet : anotherMappedSet)
                    {
                        auto currentAnotherMappedSetIterator = currentMappedSet.find(currentAnotherMappedSet);

                        if (currentAnotherMappedSetIterator == currentMappedSet.end())
                        {
                            isInterchangeable = false;
                            break;
                        }
                    }

                    if (isInterchangeable)
                    {
                        auto currentKeyIterator = result.find(currentKey);
                        auto anotherKeyIterator = result.find(anotherKey);

                        if (currentKeyIterator != result.end() and anotherKeyIterator != result.end())
                            result.erase(anotherKeyIterator);
                    }
                }
            }

            return result;
        }

        std::map<std::string, std::set<std::string>> deleteEPIMintermsInNEPI(const std::map<std::string, std::set<std::string>>& NEPI, const std::map<std::string, std::set<std::string>>& EPI)
        {
            std::map<std::string, std::set<std::string>> target(QuineMcCluskey::MinimizeCover::deleteInterchangable(NEPI));
            std::map<std::string, std::set<std::string>> result;

            for (auto& [currentNEPI, mintermsForCurrentNEPI] : target)
            {
                for (const auto& [currentEPI, mintermsForCurrentEPI] : EPI)
                    for (const auto& currentMintermForCurrentEPI : mintermsForCurrentEPI)
                    {
                        auto currentMintermForCurrentEPIIterator = mintermsForCurrentNEPI.find(currentMintermForCurrentEPI);

                        if (currentMintermForCurrentEPIIterator != mintermsForCurrentNEPI.end())
                            mintermsForCurrentNEPI.erase(currentMintermForCurrentEPIIterator);
                    }

                if (!mintermsForCurrentNEPI.empty())
                    result.emplace(currentNEPI, mintermsForCurrentNEPI);
            }

            return QuineMcCluskey::MinimizeCover::deleteInterchangable(result);
        }

        std::tuple<bool, std::map<std::string, std::set<std::string>>> columnDominance(const std::map<std::string, std::set<std::string>>& NEPIWithoutEPIMinterms)
        {
            std::map<std::string, std::set<std::string>> target(QuineMcCluskey::MinimizeCover::deleteInterchangable(QuineMcCluskey::swapKeyAndMappedSet(NEPIWithoutEPIMinterms)));
            std::map<std::string, std::set<std::string>> result(target);

            bool isChanged = false;

            for (const auto& [currentMinterm, NEPIsForCurrentMinterm] : target)
            {
                for (const auto& [anotherMinterm, NEPIsForAnotherMinterm] : target)
                {
                    if (currentMinterm == anotherMinterm)
                        continue;

                    bool isDominated = true;

                    for (const auto& currentNEPIForAnotherMinterm : NEPIsForAnotherMinterm)
                    {
                        auto currentNEPIForAnotherMintermIterator = NEPIsForCurrentMinterm.find(currentNEPIForAnotherMinterm);

                        if (currentNEPIForAnotherMintermIterator == NEPIsForCurrentMinterm.end())
                        {
                            isDominated = false;
                            break;
                        }
                    }

                    if (isDominated)
                    {
                        auto currentMintermIterator = result.find(currentMinterm);

                        if (currentMintermIterator != result.end())
                            result.erase(currentMintermIterator);

                        isChanged = true;
                    }
                }
            }

            return { isChanged, QuineMcCluskey::MinimizeCover::deleteInterchangable(QuineMcCluskey::swapKeyAndMappedSet(result)) };
        }

        std::tuple<bool, std::map<std::string, std::set<std::string>>> rowDominance(const std::map<std::string, std::set<std::string>>& NEPIWithoutEPIMinterms)
        {
            std::map<std::string, std::set<std::string>> target(QuineMcCluskey::MinimizeCover::deleteInterchangable(NEPIWithoutEPIMinterms));
            std::map<std::string, std::set<std::string>> result(target);

            bool isChanged = false;

            for (const auto& [currentNEPI, mintermsForCurrentNEPI] : target)
            {
                for (const auto& [anotherNEPI, mintermsForAnotherNEPI] : target)
                {
                    if (currentNEPI == anotherNEPI)
                        continue;

                    bool isDominated = true;

                    for (const auto& currentMintermForAnotherNEPI : mintermsForAnotherNEPI)
                    {
                        auto currentMintermForAnotherNEPIIterator = mintermsForCurrentNEPI.find(currentMintermForAnotherNEPI);

                        if (currentMintermForAnotherNEPIIterator == mintermsForCurrentNEPI.end())
                        {
                            isDominated = false;
                            break;
                        }
                    }

                    if (isDominated)
                    {
                        auto anotherPIIterator = result.find(anotherNEPI);

                        if (anotherPIIterator != result.end())
                            result.erase(anotherPIIterator);

                        isChanged = true;
                    }
                }
            }

            return { isChanged, QuineMcCluskey::MinimizeCover::deleteInterchangable(result) };
        }

        std::vector<std::string> minimizeCover(const std::map<std::string, std::set<std::string>>& PI, const std::map<std::string, std::set<std::string>>& EPI)
        {
            std::map<std::string, std::set<std::string>> result(EPI);

            auto NEPIWithoutEPIMinterms = QuineMcCluskey::MinimizeCover::deleteEPIMintermsInNEPI(QuineMcCluskey::MinimizeCover::findNEPI(PI, EPI), EPI);

            auto EPIOfNEPIWithoutEPIMinterms = QuineMcCluskey::findEPI(NEPIWithoutEPIMinterms);

            result.insert(EPIOfNEPIWithoutEPIMinterms.begin(), EPIOfNEPIWithoutEPIMinterms.end());

            NEPIWithoutEPIMinterms = QuineMcCluskey::MinimizeCover::deleteEPIMintermsInNEPI(QuineMcCluskey::MinimizeCover::findNEPI(NEPIWithoutEPIMinterms, EPIOfNEPIWithoutEPIMinterms), EPIOfNEPIWithoutEPIMinterms);

            while (!NEPIWithoutEPIMinterms.empty())
            {
                bool isColumnDominanceMadeChange = false;
                bool isRowDominanceMadeChange = false;

                std::tie(isColumnDominanceMadeChange, NEPIWithoutEPIMinterms) = QuineMcCluskey::MinimizeCover::columnDominance(NEPIWithoutEPIMinterms);
                std::tie(isRowDominanceMadeChange, NEPIWithoutEPIMinterms) = QuineMcCluskey::MinimizeCover::rowDominance(NEPIWithoutEPIMinterms);

                if (!isColumnDominanceMadeChange and !isRowDominanceMadeChange)
                    break;

                EPIOfNEPIWithoutEPIMinterms = QuineMcCluskey::findEPI(NEPIWithoutEPIMinterms);

                result.insert(EPIOfNEPIWithoutEPIMinterms.begin(), EPIOfNEPIWithoutEPIMinterms.end());

                NEPIWithoutEPIMinterms = QuineMcCluskey::MinimizeCover::deleteEPIMintermsInNEPI(QuineMcCluskey::MinimizeCover::findNEPI(NEPIWithoutEPIMinterms, EPIOfNEPIWithoutEPIMinterms), EPIOfNEPIWithoutEPIMinterms);
            }

            if (!NEPIWithoutEPIMinterms.empty())
            {
                std::cerr << "Operation is Failed!" << std::endl;
                std::exit(EXIT_FAILURE);
            }

            return QuineMcCluskey::getFinalResult(result);
        }
    }
}

void solution(const std::size_t countOfVariables, const std::set<int>& decimalFormOfMinterms, const std::set<int>& decimalFormOfDontCareTerms)
{
    std::set<std::string> binaryFormOfMinterms;
    std::set<std::string> binaryFormOfDontCareTerms;

    for (const auto& currentDecimalFormOfMinterm : decimalFormOfMinterms)
    {
        std::string binaryFormOfCurrentMinterm = QuineMcCluskey::decimalToBinary(currentDecimalFormOfMinterm, countOfVariables);
        binaryFormOfMinterms.emplace(binaryFormOfCurrentMinterm);
    }

    for (const auto& currentDecimalFormOfDontCareTerm : decimalFormOfDontCareTerms)
    {
        std::string binaryFormOfCurrentDontCareTerm = QuineMcCluskey::decimalToBinary(currentDecimalFormOfDontCareTerm, countOfVariables);
        binaryFormOfDontCareTerms.emplace(binaryFormOfCurrentDontCareTerm);
    }

    auto PI = QuineMcCluskey::findPI(binaryFormOfMinterms, binaryFormOfDontCareTerms, countOfVariables);
    auto EPI = QuineMcCluskey::findEPI(PI);

    auto result = QuineMcCluskey::MinimizeCover::minimizeCover(PI, EPI);

    for (const auto& currentResult : result)
    {
        for (std::size_t currentVariable = 0; currentVariable < countOfVariables; currentVariable++)
        {
            if (currentResult[currentVariable] == '1')
                std::cout << "x" << currentVariable + 1;
            else if (currentResult[currentVariable] == '0')
                std::cout << "x" << currentVariable + 1 << "\'";
        }

        std::cout << " + ";
    }

    std::cout << "\b\b\b   " << std::endl;
}
