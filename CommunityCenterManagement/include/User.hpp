#ifndef USER_H
#define USER_H

#include <string>
#include <map>
#include <vector>

// Enums for different user types
enum class UserType {
    City,
    Organization,
    Resident,
    NonResident
};

class Reservation; // Forward declaration

// Base class representing a user
class User {
protected:
    std::string username;
    std::string password;
    UserType userType;
    int weeklyReservationHours;
    std::vector<Reservation*> userReservations;

public:
    User(const std::string& username, const std::string& password, UserType userType);
    virtual ~User() = default;

    std::string getUsername() const { return username; }
    UserType getUserType() const { return userType; }
    std::string getPassword() const { return password; }
    int getWeeklyReservationHours() const { return weeklyReservationHours; }
    const std::vector<Reservation*>& getUserReservations() const { return userReservations; }

    // Adds a reservation to the user
    void addReservation(Reservation* reservation);  
    // Adds reservation hours to the user
    void addReservationHours(int hours);  
    // Subtracts reservation hours from the user
    void subtractReservationHours(int hours);  
    // Checks if the user has exceeded their weekly reservation limit
    bool checkWeeklyLimit(int duration) const;  

    // Logs the user in
    virtual void login();  
    // Logs the user out
    virtual void logout();  
    // Views the user's schedule
    virtual void viewSchedule();  
    // Makes a reservation for the user
    virtual void makeReservation();  
    // Cancels a reservation for the user
    virtual void cancelReservation(int reservationID);  
    // Views the user's reservations
    virtual void viewReservations();  
    // Edits the user's information
    virtual void editUserInfo();  
    // Processes a refund for the user
    void processRefund(double amount);  

    // Loads users from a file
    static std::map<std::string, User*> loadUsers(const std::string& filename);  
    // Saves users to a file
    static void saveUsers(const std::string& filename, const std::map<std::string, User*>& users); 
    // Checks if a username is already taken 
    static bool isUsernameTaken(const std::string& username, const std::map<std::string, User*>& users);  
    // Gets a user by their username
    static User* getUserByUsername(const std::string& username, const std::map<std::string, User*>& users);  
};

// Derived class for city users
class CityUser : public User {
public:
    CityUser(const std::string& username, const std::string& password);
    void specificCityFunction();
};

// Derived class for organization users
class OrganizationUser : public User {
public:
    OrganizationUser(const std::string& username, const std::string& password);
    void specificOrganizationFunction();
};

// Derived class for resident users
class ResidentUser : public User {
public:
    ResidentUser(const std::string& username, const std::string& password);
    void specificResidentFunction();
};

// Derived class for non-resident users
class NonResidentUser : public User {
public:
    NonResidentUser(const std::string& username, const std::string& password);
    void specificNonResidentFunction();
};

#endif // USER_H
