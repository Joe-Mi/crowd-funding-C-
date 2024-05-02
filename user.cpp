#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <limits>
#include <cstring>
#include <regex>

/*
    user.cpp
    Created: 24/03/2024
    Last Modified: 21/04/2024
*/

UserHash Users;
std::string User::currentUser;
User::User()
{
    this->userID = 0;
    this->name = "";
    this->email = "";
    this->accountBalance = 0.0;
}

User::User(int id, std::string name, std::string email, double balance) : userID(id), name(name), email(email), accountBalance(balance) {}

int User::getUserID()
{
    return userID;
}

std::string User::getName()
{
    return name;
}

void User::setName(std::string name)
{
    this->name = name;
}

std::string User::getEmail()
{
    return email;
}

void User::setEmail(std::string email)
{
    this->email = email;
}

double User::getAccountBalance()
{
    return accountBalance;
}

void User::setAccountBalance(double accountBalance)
{
    this->accountBalance = accountBalance;
}

void User::setCurrentUser(std::string name)
{
    currentUser = name;
}

std::string User::getCurrentUser()
{
    return currentUser;
}

bool User::signIn(const std::string &username, const std::string &email)
{
    for (int i = 0; i < UserHash::getHashGroups(); i++)
    {
        for (auto &entry : Users.getTable()[i])
        {
            if (entry.second.getName() == username && entry.second.getEmail() == email)
            {
                User user = entry.second;
                user.setCurrentUser(username);
                return true; // Credentials match, sign in successful
            }
        }
    }
    return false; // No user found with the provided credentials
}

void User::registerUser(const std::string &filename)
{
    std::string name, email;
    double accountBalance;

    // Username entry validation
    bool validUsername = false;
    while (!validUsername)
    {
        std::cout << "Enter username: ";
        std::getline(std::cin, name);

        if (name.empty())
        {
            std::cout << "\033[1;31m" << "Invalid input. Username cannot be empty" << "\033[0m" << std::endl;
        }
        else
        {
            bool isValidName = std::all_of(name.begin(), name.end(), [](char c)
                                           { return std::isalnum(c) || c == ' '; });
            bool exists = Users.checkNames(name);
            if (!isValidName)
            {
                std::cout << "\033[1;31m" << "Invalid input. Please enter a valid name" << "\033[0m" << std::endl;
            }
            else if (exists)
            {
                std::cout << "\033[1;31m" << "Username already exists. Please choose a different one." << "\033[0m" << std::endl;
            }
            else
            {
                validUsername = true;
            }
        }
    }

    // Email entry validation
    while (true)
    {
        std::cout << "Enter the email: ";
        std::getline(std::cin, email);
        bool exists = Users.checkEmail(email);
        if (!std::regex_match(email, std::regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})")))
        {
            std::cout << "\033[1;31m" << "Invalid input. Please enter a valid email address!" << "\033[0m" << std::endl;
        }
        else if (exists)
        {
            std::cout << "\033[1;31m" << "Email already exists" << "\033[0m" << std::endl;
        }
        else
        {
            break;
        }
    }

    // Balance entry validation
    while (true)
    {
        std::cout << "Enter an amount: £";
        if (!(std::cin >> accountBalance) || accountBalance < 1 || accountBalance > 1000000)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\033[1;31m" << "Invalid input. Please enter a valid amount." << "\033[0m" << std::endl;
            std::cout << "\033[1;31m" << "Make sure the balance is between £1 and £1,000,000.00." << "\033[0m" << std::endl;
        }
        else
        {
            break;
        }
    }

    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    User newUser;
    newUser.setName(name);
    newUser.setEmail(email);
    newUser.setAccountBalance(accountBalance);

    // Automatically generate userID by using getCount()
    int newUserID = Users.getCount() + 1;
    newUser.setUserID(newUserID);

    newUser.setCurrentUser(name);
    Users.insertObj(newUserID, newUser);
    User::writeUsersToFile(filename);
    std::cout << "\033[1;32m" << "Registration Successful!" << "\033[0m" << std::endl;
    std::cout << "User ID: " << newUser.getUserID() << std::endl;
    std::cout << "Username: " << newUser.getName() << std::endl;
    std::cout << "Email: " << newUser.getEmail() << std::endl;
    std::cout << "Account Balance: £" << newUser.getAccountBalance() << std::endl;
}

void User::deleteUser(std::string name, const std::string &filename)
{
    Users.removeObj(name);
    User::writeUsersToFile(filename);
    std::cout << "\033[1;32m" << " Account deleted successfully." << "\033[0m" << std::endl;
    return;
}

void User::readUsersFromFile(const std::string &filename)
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
        std::string id, amountStr;
        User user;

        if (std::getline(ss, id, ','))
            user.userID = std::stoi(id);
        if (std::getline(ss, user.name, ',') &&
            std::getline(ss, user.email, ',') &&
            std::getline(ss, amountStr))
            user.accountBalance = std::stod(amountStr);
        Users.insertObj(user.userID, user);
    }
    file.close();
}

void User::writeUsersToFile(const std::string &filename)
{
    Users.writeTofile(filename);
}

bool UserHash::isEmpty() const
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

int UserHash::hashFunction(int key)
{
    return key % hashGroups;
}

void UserHash::insertObj(int key, User user)
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
            cItr->second = user;
            std::cout << "Note: Key exists, pair replaced" << std::endl;
            break;
        }
    }

    if (keyExsists == false)
    {
        cell.emplace_back(key, user);
    }
    return;
}

int UserHash::getCount()
{
    int count = 0;
    for (int i = 0; i < hashGroups; i++)
    {
        count += table[i].size();
    }
    return count;
}

void User::setUserID(int id)
{
    userID = id;
}

void UserHash::removeObj(std::string name)
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
            if (cItr->second.getName() == Name)
            {
                nameExsists = true;
                int userID = cItr->first;
                table[i].erase(cItr);
                std::cout << "\033[1;32m" << "User '" << name << "' successfully removed." << "\033[0m" << std::endl;
                table[i].emplace_back(userID, User(userID, "", "", 0.0));
                break;
            }
        }
    }
    if (!nameExsists)
    {
        std::cout << "Note: name not found, pair not removed";
    }
}

bool UserHash::checkNames(std::string name)
{
    std::string Name = name;
    bool presence = false;
    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            if (cItr->second.getName() == Name)
            {
                presence = true;
            }
        }
    }

    return presence;
}

bool UserHash::checkEmail(std::string email)
{
    std::string Email = email;
    bool presence = false;

    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            if (cItr->second.getEmail() == Email)
            {
                presence = true;
            }
        }
    }

    return presence;
}

void UserHash::printTable()
{
    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            std::cout << "key: " << cItr->first << std::endl;
        }
    }
    return;
}

void UserHash::writeTofile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    file << "userID,name,email,accountBalance" << std::endl;

    for (int i = 0; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto cItr = table[i].begin();
        for (; cItr != table[i].end(); cItr++)
        {
            file << cItr->second.getUserID() << ','
                 << cItr->second.getName() << ','
                 << cItr->second.getEmail() << ','
                 << cItr->second.getAccountBalance()
                 << std::endl;
        }
    }
    file.close();

    return;
}
