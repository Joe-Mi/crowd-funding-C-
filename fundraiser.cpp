#include "fundraiser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <cstring>
#include <regex>
#include <algorithm>

/*
fundraiser.cpp
Created: 24/03/2024
Last Modified: 04/04/2024
*/

FundraiserHash fundraisers;
Fundraiser::Fundraiser()
{
    this->fundraiserID = 0;
    this->fundraiserName = "";
    this->description = "";
    this->budget = 0.0;
    this->creator = "";
}

int Fundraiser::getFundraiserID() const
{
    return fundraiserID;
}

std::string Fundraiser::getFundraiserName() const
{
    return fundraiserName;
}

void Fundraiser::setFundraiserName(std::string fundraiserName)
{
    this->fundraiserName = fundraiserName;
}

std::string Fundraiser::getDescription() const
{
    return description;
}

void Fundraiser::setDescription(std::string description)
{
    this->description = description;
}

double Fundraiser::getBudget() const
{
    return budget;
}

void Fundraiser::setBudget(double budget)
{
    this->budget = budget;
}

std::string Fundraiser::getCreator() const
{
    return creator;
}

void Fundraiser::setCreator(User user)
{
    this->creator = user.getCurrentUser();
}

double Fundraiser::getAmountRaised() const
{
    return amountRaised;
}

void Fundraiser::setAmountRaised(double amountRaised)
{
    this->amountRaised = amountRaised;
}

void Fundraiser::createFundraiser(User creator, const std::string &filename)
{

    std::string name, desc;
    double amount;

    while (true)
    {
        std::cout << "Enter fundraiser name: ";
        std::string entry;
        std::getline(std::cin, entry);

        if (entry.empty())
        {
            std::cout << "\033[1;31m" << "Fundraiser name cannot be empty" << "\033[0m" << std::endl;
        }
        else
        {
            bool isValidName = true;
            for (char ch : entry)
            {
                if (!std::isalpha(ch) && !std::isspace(ch))
                {
                    isValidName = false;
                    break;
                }
            }
            if (!isValidName)
            {
                std::cout << "\033[1;31m" << "Invalid input. Please enter a valid fundraiser name" << "\033[0m" << std::endl;
            }
            else
            {
                name = entry;
                break;
            }
        }
    }

    while (true)
    {
        std::cout << "Enter description: ";
        std::getline(std::cin, desc);

        if (desc.empty())
        {
            std::cout << "\033[1;31m" << "Please enter description" << "\033[0m" << std::endl;
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        std::cout << "Enter target amount: £";
        std::string entry;
        std::getline(std::cin, entry);
        std::regex amountPattern(R"(^\d+(\.\d{2})?$)");

        if (entry.empty())
        {
            std::cout << "\033[1;31m" << "Target amount cannot be empty" << "\033[0m" << std::endl;
        }
        else
        {
            if (!std::regex_match(entry, amountPattern))
            {
                std::cout << "\033[1;31m" << "Invalid input. Please enter a valid amount." << "\033[0m" << std::endl;
            }
            else
            {
                amount = std::stod(entry);
                break;
            }
        }
    }

    Fundraiser newFundraiser;
    newFundraiser.fundraiserID = fundraisers.getCount() + 1;
    newFundraiser.setFundraiserName(name);
    newFundraiser.setDescription(desc);
    newFundraiser.setBudget(amount);
    newFundraiser.setCreator(creator);
    fundraisers.insertObj(newFundraiser.fundraiserID, newFundraiser);
    Fundraiser::writeFundraisersToFile(filename);

    std::cout << "\033[1;32m" << "Fundraiser Successfully created!" << "\033[0m" << std::endl;
    std::cout << "Fundraiser ID: " << newFundraiser.getFundraiserID() << std::endl;
    std::cout << "Fundraiser Name: " << newFundraiser.getFundraiserName() << std::endl;
    std::cout << "Description: " << newFundraiser.getDescription() << std::endl;
    std::cout << "Budget: £" << newFundraiser.getBudget() << std::endl;
    std::cout << "Creator: " << newFundraiser.getCreator() << std::endl;
}

void Fundraiser::withdrawFundraiser(const std::string &filename)
{
    std::cout << "Enter fundraiser name: ";
    std::string entry;
    std::getline(std::cin, entry);

    fundraisers.removeObj(filename, entry);

    return;
}

void Fundraiser::searchFundraisers(const std::string username, User users, double num, const std::string &usersFilename, const std::string &filename)
{
    std::string keywords;
    std::cout << "Enter keywords to search for a fundraiser project: ";
    std::getline(std::cin, keywords);

    if (fundraisers.isEmpty())
    {
        std::cout << "No fundraisers to display." << std::endl;
        return;
    }

    bool found = false;
    std::string lowercaseKeywords = keywords;
    std::transform(lowercaseKeywords.begin(), lowercaseKeywords.end(), lowercaseKeywords.begin(), ::tolower);

    std::cout << "Matching Fundraisers:" << std::endl;
    for (int i = 0; i < FundraiserHash::hashGroups; i++) // Use public hashGroups
    {
        if (fundraisers.table[i].size() == 0) // Use public table
            continue;

        auto cItr = fundraisers.table[i].begin();
        for (; cItr != fundraisers.table[i].end(); cItr++) // Use public table
        {
            std::string fundraiserName = cItr->second.getFundraiserName();
            std::transform(fundraiserName.begin(), fundraiserName.end(), fundraiserName.begin(), ::tolower);

            if (fundraiserName.find(lowercaseKeywords) != std::string::npos)
            {
                found = true;
                std::cout << "ID: " << cItr->second.getFundraiserID() << std::endl;
                std::cout << "Name: " << cItr->second.getFundraiserName() << std::endl;
                std::cout << "Description: " << cItr->second.getDescription() << std::endl;
                std::cout << "Budget: £" << cItr->second.getBudget() << std::endl;
                std::cout << "Creator: " << cItr->second.getCreator() << std::endl;
                std::cout << "-----------------------------------" << std::endl;
            }
        }
    }

    if (!found)
    {
        std::cout << "\033[1;31m" << "No matching fundraisers found." << "\033[0m" << std::endl;
    }

    if (fundraisers.isEmpty())
    {
        std::cout << "No fundraisers to display." << std::endl;
        return;
    }

    std::cout << "All Fundraisers:" << std::endl;
    fundraisers.printTable();

    int fundraiserID;
    std::cout << "Enter fundraiser id: " << std::endl;
    std::cin >> fundraiserID;

    fundraisers.Funding(fundraiserID, users, num, usersFilename, filename);

    return;
}

void Fundraiser::deleteFundraiser(const std::string &filename)
{
    std::cout << "Enter fundraiser name: ";
    std::string entry;
    std::getline(std::cin, entry);

    fundraisers.removeObj(filename, entry);

    return;
}

void Fundraiser::viewUserFundraisers(std::string name)
{
    std::cout << "My Fundraisers:" << std::endl;

    fundraisers.creatorFundraisers(name);
}

void Fundraiser::readFundraisersFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(0);
    }

    std::string header;
    std::getline(file, header);

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string id, budget, creatorName;
        Fundraiser fundraiser;

        if (std::getline(ss, id, ','))
            fundraiser.fundraiserID = std::stoi(id);
        if (std::getline(ss, fundraiser.fundraiserName, ',') &&
            std::getline(ss, fundraiser.description, ',') &&
            std::getline(ss, budget, ','))
            fundraiser.budget = std::stod(budget);
        if (std::getline(ss, creatorName, ','))
            fundraiser.creator = creatorName;

        fundraisers.insertObj(fundraiser.getFundraiserID(), fundraiser);
    }
    file.close();
}

void Fundraiser::writeFundraisersToFile(const std::string &filename)
{
    fundraisers.writeTofile(filename);
}

bool FundraiserHash::isEmpty() const
{
    int sumSize = 0;
    for (int i = 0; i < hashGroups; i++)
    {
        sumSize += table[i].size();
    }

    if (sumSize == 0)
    {
        return true;
    }
    return false;
}

int FundraiserHash::hashFunction(int key)
{
    return key % hashGroups;
}

void FundraiserHash::insertObj(int key, Fundraiser project)
{
    int hashValue = hashFunction(key);
    auto &cell = table[hashValue];
    auto cItr = begin(cell);
    bool keyExsists = false;
    for (; cItr != end(cell); cItr++)
    {
        if (cItr->first == key)
        {
            keyExsists = true;
            cItr->second = project;
            std::cout << "Note: Key exists, pair replaced" << std::endl;
            break;
        }
    }

    if (keyExsists == false)
    {
        cell.emplace_back(key, project);
    }
    return;
}

int FundraiserHash::getCount()
{
    int count = 0;
    for (int i = 0; i < hashGroups; i++)
    {
        count += table[i].size();
    }
    return count;
}

void FundraiserHash::removeObj(const std::string &filename, std::string name)
{
    std::string Name = name;
    bool nameExsists = false;
    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            if (cItr->second.getFundraiserName() == Name)
            {
                nameExsists = true;
                Fundraiser::writeFundraisersToFile(filename);
                std::cout << "\033[1;32m" << " Fundraiser successfully withdrawn." << "\033[0m" << std::endl;
                break;
            }
        }
    }

    if (!nameExsists)
    {
        std::cout << "Note: name not found, pair not removed";
    }
}

void FundraiserHash::printTable()
{
    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            std::cout << "ID: " << cItr->second.getFundraiserID() << std::endl;
            std::cout << "Name: " << cItr->second.getFundraiserName() << std::endl;
            std::cout << "Description: " << cItr->second.getDescription() << std::endl;
            std::cout << "Budget: £" << cItr->second.getBudget() << std::endl;
            std::cout << "_____________________________________________________" << std::endl;
        }
    }
    return;
}

void FundraiserHash::creatorFundraisers(std::string creator)
{
    bool presence = false;
    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            if (cItr->second.getCreator() == creator)
            {
                presence = true;
                std::cout << "ID: " << cItr->second.getFundraiserID() << std::endl;
                std::cout << "Name: " << cItr->second.getFundraiserName() << std::endl;
                std::cout << "Description: " << cItr->second.getDescription() << std::endl;
                std::cout << "Budget: £" << cItr->second.getBudget() << std::endl;
                std::cout << "-----------------------------------" << std::endl;
            }
        }
    }

    if (!presence)
    {
        std::cout << "\033[1;31m" << "No fundraisers to display." << "\033[0m" << std::endl;
    }

    return;
}

void FundraiserHash::Funding(int fundraiserID, User user, double num, const std::string &usersFilename, const std::string &fundraisersFilename)
{

    double amount;
    User donor;
    donor = user;

    bool donationCompleted = false;
    while (!donationCompleted)
    {
        int entry;
        std::cout << "Enter donation amount for Fundraiser ID " << fundraiserID << ": £";
        std::cin >> entry;
        std::regex amountPattern(R"(^\d+(\.\d{2})?$)");

        if (!std::regex_match(std::to_string(entry), amountPattern))
        {
            std::cout << "\033[1;31m" << "Donation amount cannot be empty" << "\033[0m" << std::endl;
        }
        else
        {
            if (isdigit(entry))
            {
                std::cout << "\033[1;31m" << "Invalid input. Please enter a valid amount." << "\033[0m" << std::endl;
            }
            else
            {
                amount = entry;
                if (amount > (num + amount))
                {
                    std::cout << "\033[1;31m" << "Insufficient Funds" << "\033[0m" << std::endl;
                    std::cout << "\033[1;31m" << amount << "  " << num << "\033[0m" << std::endl;
                    break;
                }
                else
                {

                    for (int i = 0; i < hashGroups; i++)
                    {
                        if (table[i].size() == 0)
                            continue;
                        auto cItr = table[i].begin();
                        for (; cItr != table[i].end(); cItr++)
                        {
                            if (cItr->second.getFundraiserID() == fundraiserID)
                            {
                                cItr->second.setAmountRaised(cItr->second.getAmountRaised() + amount);
                                donor.setAccountBalance(num - amount);
                                User::writeUsersToFile(usersFilename);
                                Fundraiser::writeFundraisersToFile(fundraisersFilename);
                                std::cout << "\033[1;32m" << "Donation successful!" << "\033[0m" << std::endl;
                                std::cout << "Donated amount: £" << amount << std::endl;
                                std::cout << "Updated account balance: £" << donor.getAccountBalance() << std::endl;
                                donationCompleted = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return;
}

void FundraiserHash::writeTofile(const std::string filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    file << "fundraiserID,fundraiserName,description,budget,creator" << std::endl;

    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            file << cItr->second.getFundraiserID() << ','
                 << cItr->second.getFundraiserName() << ','
                 << cItr->second.getDescription() << ','
                 << cItr->second.getBudget() << ','
                 << cItr->second.getCreator()
                 << std::endl;
        }
    }
    file.close();
}
