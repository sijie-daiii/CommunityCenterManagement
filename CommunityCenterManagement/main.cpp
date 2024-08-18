#include <iostream>
#include <map>
#include <string>
#include "User.hpp"
#include "Event.hpp"
#include "Reservation.hpp"
#include "FacilityManager.hpp"
#include "HelperFunctions.hpp"

// Displays the main menu
void displayMenu() {
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Create New User" << std::endl;
    std::cout << "3. View Today's Schedule" << std::endl;
    std::cout << "4. Make a Reservation" << std::endl;
    std::cout << "5. Cancel a Reservation" << std::endl;
    std::cout << "6. View Your Reservations" << std::endl;
    std::cout << "7. Edit Your User Info" << std::endl;
    std::cout << "8. List All Reservations" << std::endl;
    std::cout << "9. View Detailed Event Info" << std::endl;
    std::cout << "10. View Detailed Reservation Info" << std::endl;
    std::cout << "11. Create an Event" << std::endl;
    std::cout << "12. Cancel an Event" << std::endl;
    std::cout << "13. Make a Payment" << std::endl;
    std::cout << "14. View Facility Budget" << std::endl;
    std::cout << "15. Logout" << std::endl;
    std::cout << "16. Exit" << std::endl;
}

// Main function
int main() {
    FacilityManager facilityManager;
    std::map<std::string, User*> users = User::loadUsers("data/users.txt");
    std::map<std::string, Event*> events = Event::loadEvents("data/events.txt", users);
    std::map<int, Reservation*> reservations = Reservation::loadReservations("data/reservations.txt", users, events);

    for (const auto& pair : events) {
        facilityManager.addEvent(pair.second);
    }

    bool running = true;
    User* loggedInUser = nullptr;

    while (running) {
        displayMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear the newline character

        switch (choice) {
            case 1:
                loggedInUser = handleLogin(users);
                break;
            case 2:
                handleUserCreation(users);
                break;
            case 3:
                facilityManager.getSchedule().viewSchedule();
                break;
            case 4:
                handleReservation(facilityManager.getSchedule(), loggedInUser, reservations);
                break;
            case 5:
                handleCancellation(loggedInUser, reservations, facilityManager);
                break;
            case 6:
                if (loggedInUser) {
                    loggedInUser->viewReservations();
                } else {
                    std::cout << "Please login first." << std::endl;
                }
                break;
            case 7:
                if (loggedInUser) {
                    loggedInUser->editUserInfo();
                } else {
                    std::cout << "Please login first." << std::endl;
                }
                break;
            case 8:
                listAllReservations(reservations);
                break;
            case 9:
                viewDetailedEvent(facilityManager.getSchedule());
                break;
            case 10:
                viewDetailedReservation(reservations);
                break;
            case 11:
                createEvent(facilityManager, events, users, facilityManager.getSchedule());
                break;
            case 12: {
                std::string eventName;
                std::cout << "Enter the event name to cancel: ";
                std::getline(std::cin, eventName);
                facilityManager.cancelEvent(events, reservations, eventName);
                break;
            }
            case 13:
                handlePayment(loggedInUser, reservations, facilityManager);
                break;
            case 14:
                std::cout << "Current Facility Budget: $" << facilityManager.getFacilityBudget() << std::endl;
                break;
            case 15:
                loggedInUser = nullptr;
                std::cout << "Logged out successfully." << std::endl;
                break;
            case 16:
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    // Clean up dynamically allocated memory
    for (auto& pair : users) {
        delete pair.second;
    }
    for (auto& pair : events) {
        delete pair.second;
    }
    for (auto& pair : reservations) {
        delete pair.second;
    }

    return 0;
}
