#include "User.hpp"
#include "Reservation.hpp"
#include "Event.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Constructor for the User class
User::User(const std::string& username, const std::string& password, UserType userType)
    : username(username), password(password), userType(userType), weeklyReservationHours(0) {}

// Adds a reservation to the user
void User::addReservation(Reservation* reservation) {
    userReservations.push_back(reservation);
}

// Adds reservation hours to the user
void User::addReservationHours(int hours) {
    weeklyReservationHours += hours;
}

// Subtracts reservation hours from the user
void User::subtractReservationHours(int hours) {
    weeklyReservationHours -= hours;
}

// Checks if the user has exceeded their weekly reservation limit
bool User::checkWeeklyLimit(int duration) const {
    int weeklyLimit = 0;
    switch (userType) {
        case UserType::City:
            weeklyLimit = 48;
            break;
        case UserType::Organization:
            weeklyLimit = 36;
            break;
        default:
            weeklyLimit = 24;
            break;
    }
    return (weeklyReservationHours + duration) <= weeklyLimit;
}

// Logs the user in
void User::login() {
    std::cout << "User " << username << " logged in successfully." << std::endl;
}

// Logs the user out
void User::logout() {
    std::cout << "User " << username << " logged out successfully." << std::endl;
}

// Views the user's schedule
void User::viewSchedule() {
    std::cout << "Viewing schedule for user " << username << "." << std::endl;
}

// Makes a reservation for the user
void User::makeReservation() {
    std::cout << "Making reservation for user " << username << "." << std::endl;
}

// Cancels a reservation for the user
void User::cancelReservation(int reservationID) {
    auto it = std::find_if(userReservations.begin(), userReservations.end(),
        [reservationID](Reservation* res) { return res->getReservationID() == reservationID; });

    if (it != userReservations.end()) {
        std::cout << "Refund processed for reservation " << reservationID << "." << std::endl;
        delete *it; // Free the memory
        userReservations.erase(it); // Remove the pointer from the vector
        std::cout << "Reservation canceled successfully." << std::endl;
    } else {
        std::cout << "Reservation not found or you do not have permission to cancel this reservation." << std::endl;
    }
}

// Views the user's reservations
void User::viewReservations() {
    if (userReservations.empty()) {
        std::cout << "No reservations found for user " << username << "." << std::endl;
        return;
    }
    for (const auto& reservation : userReservations) {
        if (reservation == nullptr) {
            std::cout << "Error: Found a null reservation pointer." << std::endl;
            continue;
        }
        std::cout << "Reservation ID: " << reservation->getReservationID()
                  << "\nEvent: " << reservation->getEvent()->getEventName()
                  << "\nStatus: " << static_cast<int>(reservation->getReservationStatus())
                  << "\nPayment Status: " << static_cast<int>(reservation->getPaymentStatus())
                  << "\nTotal Cost: " << reservation->getTotalCost()
                  << "\nReservation Time: " << reservation->getReservationTime() << "\n" << std::endl;
    }
}

// Edits the user's information
void User::editUserInfo() {
    std::string newUsername, newPassword;
    std::cout << "Enter new username: ";
    std::cin >> newUsername;
    std::cout << "Enter new password: ";
    std::cin >> newPassword;

    username = newUsername;
    password = newPassword;

    std::cout << "User information updated successfully." << std::endl;
}

// Processes a refund for the user
void User::processRefund(double amount) {
    // Implement the refund process (you can adjust the logic as needed)
    std::cout << "Processing refund of amount: " << amount << " for user " << username << std::endl;
}

// Derived class constructor for city users
CityUser::CityUser(const std::string& username, const std::string& password)
    : User(username, password, UserType::City) {}

// Specific function for city users
void CityUser::specificCityFunction() {
    std::cout << "Specific function for City User " << username << "." << std::endl;
}

// Derived class constructor for organization users
OrganizationUser::OrganizationUser(const std::string& username, const std::string& password)
    : User(username, password, UserType::Organization) {}

// Specific function for organization users
void OrganizationUser::specificOrganizationFunction() {
    std::cout << "Specific function for Organization User " << username << "." << std::endl;
}

// Derived class constructor for resident users
ResidentUser::ResidentUser(const std::string& username, const std::string& password)
    : User(username, password, UserType::Resident) {}

// Specific function for resident users
void ResidentUser::specificResidentFunction() {
    std::cout << "Specific function for Resident User " << username << "." << std::endl;
}

// Derived class constructor for non-resident users
NonResidentUser::NonResidentUser(const std::string& username, const std::string& password)
    : User(username, password, UserType::NonResident) {}

// Specific function for non-resident users
void NonResidentUser::specificNonResidentFunction() {
    std::cout << "Specific function for Non-Resident User " << username << "." << std::endl;
}

// Loads users from a file
std::map<std::string, User*> User::loadUsers(const std::string& filename) {
    std::map<std::string, User*> users;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string username, password, userTypeStr;
            UserType userType = UserType::City;

            iss >> username >> password >> userTypeStr;

            if (userTypeStr == "City") userType = UserType::City;
            else if (userTypeStr == "Organization") userType = UserType::Organization;
            else if (userTypeStr == "Resident") userType = UserType::Resident;
            else if (userTypeStr == "NonResident") userType = UserType::NonResident;

            User* user;
            switch (userType) {
                case UserType::City: user = new CityUser(username, password); break;
                case UserType::Organization: user = new OrganizationUser(username, password); break;
                case UserType::Resident: user = new ResidentUser(username, password); break;
                case UserType::NonResident: user = new NonResidentUser(username, password); break;
            }
            users[username] = user;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
    return users;
}

// Saves users to a file
void User::saveUsers(const std::string& filename, const std::map<std::string, User*>& users) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : users) {
            User* user = pair.second;
            std::string userTypeStr;
            switch (user->getUserType()) {
                case UserType::City: userTypeStr = "City"; break;
                case UserType::Organization: userTypeStr = "Organization"; break;
                case UserType::Resident: userTypeStr = "Resident"; break;
                case UserType::NonResident: userTypeStr = "NonResident"; break;
            }
            file << user->getUsername() << " " << user->getPassword() << " " << userTypeStr << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}

// Checks if a username is already taken
bool User::isUsernameTaken(const std::string& username, const std::map<std::string, User*>& users) {
    return users.find(username) != users.end();
}

// Gets a user by their username
User* User::getUserByUsername(const std::string& username, const std::map<std::string, User*>& users) {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}
