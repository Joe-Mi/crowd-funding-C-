#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <vector>
#include <list>

/*
    user.h
    Created:24/03/2024
    Last Modified: 21/04/2024
*/

class User
{
private:
    int userID;
    std::string name;
    std::string email;
    double accountBalance;
    static std::string currentUser;

public:
    User();
    User(int id, std::string name, std::string email, double balance);
    int getUserID();
    void setUserID(int id);
    std::string getName();
    void setName(std::string name);
    std::string getEmail();
    void setEmail(std::string email);
    double getAccountBalance();
    void setCurrentUser(std::string name);
    std::string getCurrentUser();
    void setAccountBalance(double accountBalance);
    void registerUser(const std::string &filename);
    void deleteUser(std::string name, const std::string &filename);
    static void readUsersFromFile(const std::string &filename);
    static void writeUsersToFile(const std::string &filename);
    static bool signIn(const std::string &username, const std::string &email);
};

class UserHash
{
private:
    static const int hashGroups = 300;
    std::list<std::pair<int, User>> table[hashGroups];
    int currentObjs = 0;

public:
    static int getHashGroups() { return hashGroups; }
    std::list<std::pair<int, User>> (&getTable())[hashGroups] { return table; }
    bool isEmpty() const;
    int hashFunction(int key);
    int getCount();
    void insertObj(int key, User user);
    void removeObj(std::string name);
    std::string searchTable(int key);
    bool checkNames(std::string name);
    bool checkEmail(std::string email);
    void printTable();
    void writeTofile(const std::string &filename);
};

#endif
