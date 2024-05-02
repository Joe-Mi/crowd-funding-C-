#include "user.h"
#include "fundraiser.h"
#include <iostream>
#include <limits>

void displayHomePage()
{
    std::cout << "Please select an option to continue." << std::endl;
    std::cout << "1. Create Fundraiser" << std::endl;
    std::cout << "2. Search for Fundraisers" << std::endl;
    std::cout << "3. View My Fundraisers" << std::endl;
    std::cout << "4. Delete Account" << std::endl;
    std::cout << "5. Sign Out" << std::endl;
    std::cout << "Enter option: ";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <users.csv> <fundraisers.csv>" << std::endl;
        return 1;
    }

    std::string usersFilename = argv[1];
    std::string fundraisersFilename = argv[2];
    User::readUsersFromFile(usersFilename);
    Fundraiser::readFundraisersFromFile(fundraisersFilename);

    Fundraiser fundraiser;
    int option;
    std::cout << "Welcome!" << std::endl;
    std::cout << "Please select an option:" << std::endl;
    std::cout << "1. Sign In" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "Enter option: ";
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    User currentUser;
    if (option == 1)
    {
        std::string username, email;
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        std::cout << "Enter email: ";
        std::getline(std::cin, email);
        if (!User::signIn(username, email))
        {
            std::cout << "Sign in failed. Please register or try again." << std::endl;
            return 1;
        }
        currentUser.setCurrentUser(username);
    }
    else if (option == 2)
    {
        currentUser.registerUser(usersFilename);
    }
    else
    {
        std::cerr << "Invalid option. Exiting." << std::endl;
        return 1;
    }

    do
    {
        displayHomePage();
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (option)
        {
        case 1:
            fundraiser.createFundraiser(currentUser, fundraisersFilename);
            break;
        case 2:
            fundraiser.searchFundraisers(currentUser.getCurrentUser(), currentUser, currentUser.getAccountBalance(), usersFilename, fundraisersFilename);
            break;
        case 3:
            fundraiser.viewUserFundraisers(currentUser.getCurrentUser());
            break;
        case 4:
            currentUser.deleteUser(currentUser.getCurrentUser(), usersFilename);
            option = 5;
            break;
        case 5:
            std::cout << "Goodbye" << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (option != 5);

    return 0;
}
