#ifndef _FUNDRAISER_H_
#define _FUNDRAISER_H_

#include <string>
#include <vector>
#include <list>
#include "user.h"

/*
fundraiser.h
Created:24/03/2024
last Modified: 04/04/2024
*/

class Fundraiser
{
private:
    int fundraiserID;
    std::string fundraiserName;
    std::string description;
    double budget;
    double amountRaised;
    std::string creator;

public:
    Fundraiser();
    int getFundraiserID() const;
    std::string getFundraiserName() const;
    void setFundraiserName(std::string fundraiserName);
    std::string getDescription() const;
    void setDescription(std::string description);
    double getBudget() const;
    double getAmountRaised() const;
    void setAmountRaised(double amountRaised);
    void setBudget(double budget);
    std::string getCreator() const;
    void setCreator(User user);
    void createFundraiser(User user, const std::string &filename);
    void withdrawFundraiser(const std::string &filename);
    void deleteFundraiser(const std::string &filename);
    void searchFundraisers(const std::string username, User users, double num, const std::string &usersFilename, const std::string &filename);
    void viewUserFundraisers(std::string name);
    static void readFundraisersFromFile(const std::string &filename);
    static void writeFundraisersToFile(const std::string &filename);
};

class FundraiserHash
{
private:
    int currentObjs = 0;

public:
    static const int hashGroups = 100;
    std::list<std::pair<int, Fundraiser>> table[hashGroups];
    bool isEmpty() const;
    int hashFunction(int key);
    int getCount();
    void insertObj(int key, Fundraiser project);
    void removeObj(const std::string &filename, std::string name);
    void printTable();
    void Funding(int fundraiserID, User user, double num, const std::string &usersFilename, const std::string &fundraisersFilename);
    void creatorFundraisers(std::string creator);
    void writeTofile(const std::string filename);
};
#endif
