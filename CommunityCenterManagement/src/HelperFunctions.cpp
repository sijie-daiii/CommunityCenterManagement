#include "HelperFunctions.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>

// Prints the main menu
void printMenu() {
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

// Handles the creation of a new user
void handleUserCreation(std::map<std::string, User*>& users) {
    std::string username, password, userTypeStr;
    UserType userType;
    std::cout << "Enter new username: ";
    std::cin >> username;
    if (User::isUsernameTaken(username, users)) {
        std::cout << "Username already taken. Please choose a different username." << std::endl;
        return;
    }
    std::cout << "Enter new password: ";
    std::cin >> password;
    std::cout << "Enter user type (City, Organization, Resident, NonResident): ";
    std::cin >> userTypeStr;
    if (userTypeStr == "City") userType = UserType::City;
    else if (userTypeStr == "Organization") userType = UserType::Organization;
    else if (userTypeStr == "Resident") userType = UserType::Resident;
    else if (userTypeStr == "NonResident") userType = UserType::NonResident;
    else {
        std::cout << "Invalid user type." << std::endl;
        return;
    }
    User* newUser;
    switch (userType) {
        case UserType::City: newUser = new CityUser(username, password); break;
        case UserType::Organization: newUser = new OrganizationUser(username, password); break;
        case UserType::Resident: newUser = new ResidentUser(username, password); break;
        case UserType::NonResident: newUser = new NonResidentUser(username, password); break;
    }
    users[username] = newUser;
    User::saveUsers("data/users.txt", users); // Save the updated user list to the file
    std::cout << "User created successfully." << std::endl;
}

// Handles making a reservation
void handleReservation(Schedule& schedule, User* currentUser, std::map<int, Reservation*>& reservations) {
    if (!currentUser) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    // Display available events
    std::cout << "Available events:" << std::endl;
    schedule.viewSchedule();

    std::string eventName;
    std::cout << "Enter the event name to make a reservation: ";
    std::cin >> eventName;

    Event* event = schedule.findEvent(eventName);
    if (event) {
        // Check for duplicate reservation
        for (const auto& reservation : currentUser->getUserReservations()) {
            if (reservation->getEvent()->getEventName() == eventName) {
                std::cout << "You already have a reservation for this event." << std::endl;
                return;
            }
        }

        std::string reservationTime;
        std::cout << "Enter reservation time (YYYY-MM-DDTHH:MM): ";
        std::cin >> reservationTime;

        if (!event->isWithinEventTime(reservationTime)) {
            std::cout << "The reservation time is outside the event's start and end times." << std::endl;
            return;
        }

        int reservationID = reservations.size() + 1;
        Reservation* reservation = new Reservation(reservationID, currentUser, event, reservationTime);
        reservations[reservationID] = reservation;
        event->purchaseTicket(currentUser);

        // Save the updated reservations to the file
        Reservation::saveReservations("data/reservations.txt", reservations);

        std::cout << "Reservation made successfully." << std::endl;
    } else {
        std::cout << "Event not found." << std::endl;
    }
}

// Handles cancellation of a reservation
void handleCancellation(User* currentUser, std::map<int, Reservation*>& reservations, FacilityManager& facilityManager) {
    if (!currentUser) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    int reservationID;
    std::cout << "Enter the reservation ID to cancel: ";
    while (!(std::cin >> reservationID)) {
        std::cout << "Invalid input. Please enter a valid reservation ID: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    auto it = reservations.find(reservationID);
    if (it != reservations.end() && it->second->getUser() == currentUser) {
        it->second->processRefund(facilityManager);
        delete it->second;
        reservations.erase(it);

        // Save the updated reservations to the file
        Reservation::saveReservations("data/reservations.txt", reservations);

        std::cout << "Reservation canceled successfully." << std::endl;
    } else {
        std::cout << "Reservation not found or you do not have permission to cancel this reservation." << std::endl;
    }
}

// Lists all reservations
void listAllReservations(const std::map<int, Reservation*>& reservations) {
    if (reservations.empty()) {
        std::cout << "No reservations found." << std::endl;
        return;
    }
    for (const auto& pair : reservations) {
        Reservation* reservation = pair.second;
        std::cout << "Reservation ID: " << reservation->getReservationID() << "\nUser: " << reservation->getUser()->getUsername()
                  << "\nEvent: " << reservation->getEvent()->getEventName() << "\nStatus: " << static_cast<int>(reservation->getReservationStatus())
                  << "\nPayment Status: " << static_cast<int>(reservation->getPaymentStatus()) << "\nTotal Cost: " << reservation->getTotalCost()
                  << "\nReservation Time: " << reservation->getReservationTime() << "\n" << std::endl;
    }
}

// Views detailed information of an event
void viewDetailedEvent(Schedule& schedule) {
    std::string eventName;
    std::cout << "Enter the event name: ";
    std::cin >> eventName;

    Event* event = schedule.findEvent(eventName);
    if (event) {
        event->getDetailedView();
    } else {
        std::cout << "Event not found." << std::endl;
    }
}

// Views detailed information of a reservation
void viewDetailedReservation(const std::map<int, Reservation*>& reservations) {
    int reservationID;
    std::cout << "Enter the reservation ID: ";
    while (!(std::cin >> reservationID)) {
        std::cout << "Invalid input. Please enter a valid reservation ID: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    auto it = reservations.find(reservationID);
    if (it != reservations.end()) {
        it->second->getDetailedView();
    } else {
        std::cout << "Reservation not found." << std::endl;
    }
}

// Handles event creation
void createEvent(FacilityManager& facilityManager, std::map<std::string, Event*>& events, const std::map<std::string, User*>& users, Schedule& schedule) {
    std::string eventName, organizerUsername, startTime, endTime, layoutStyleStr, isPublicStr, openToResidentsStr, openToNonResidentsStr;
    LayoutStyle layoutStyle;
    bool isPublic, openToResidents, openToNonResidents;
    int maxGuests;
    double ticketPrice;

    std::cout << "Enter event name: ";
    std::getline(std::cin, eventName);
    std::cout << "Enter organizer username: ";
    std::getline(std::cin, organizerUsername);
    std::cout << "Enter start time (YYYY-MM-DDTHH:MM): ";
    std::getline(std::cin, startTime);
    std::cout << "Enter end time (YYYY-MM-DDTHH:MM): ";
    std::getline(std::cin, endTime);
    std::cout << "Enter layout style (integer 1-4): ";
    std::getline(std::cin, layoutStyleStr);
    std::cout << "Enter max guests: ";
    std::cin >> maxGuests;
    std::cout << "Enter ticket price: ";
    std::cin >> ticketPrice;
    std::cin.ignore();
    std::cout << "Is this event public? (yes/no): ";
    std::getline(std::cin, isPublicStr);
    std::cout << "Open to residents only? (yes/no): ";
    std::getline(std::cin, openToResidentsStr);
    std::cout << "Open to non-residents? (yes/no): ";
    std::getline(std::cin, openToNonResidentsStr);

    if (layoutStyleStr == "1") layoutStyle = LayoutStyle::Meeting;
    else if (layoutStyleStr == "2") layoutStyle = LayoutStyle::Lecture;
    else if (layoutStyleStr == "3") layoutStyle = LayoutStyle::Wedding;
    else if (layoutStyleStr == "4") layoutStyle = LayoutStyle::Dance;

    isPublic = (isPublicStr == "yes");
    openToResidents = (openToResidentsStr == "yes");
    openToNonResidents = (openToNonResidentsStr == "yes");

    if (users.find(organizerUsername) == users.end()) {
        std::cerr << "Error: Organizer " << organizerUsername << " not found in users map." << std::endl;
        return;
    }

    User* organizer = users.at(organizerUsername);
    Event* newEvent = new Event(eventName, organizer, startTime, endTime, layoutStyle, isPublic, maxGuests, ticketPrice);
    newEvent->setOpenToResidents(openToResidents);
    newEvent->setOpenToNonResidents(openToNonResidents);

    if (schedule.checkAvailability(startTime, endTime)) {
        schedule.addEvent(newEvent);
        events[eventName] = newEvent;
        Event::saveEvents("data/events.txt", events); // Save events after creation
        std::cout << "Event created successfully." << std::endl;
    } else {
        delete newEvent; // Clean up memory if event cannot be added
        std::cout << "The facility is not available during this time slot." << std::endl;
    }
}

// Handles user login
User* handleLogin(const std::map<std::string, User*>& users) {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    auto it = users.find(username);
    if (it != users.end() && it->second->getPassword() == password) {
        std::cout << "Login successful." << std::endl;
        return it->second;
    } else {
        std::cout << "Invalid username or password." << std::endl;
        return nullptr;
    }
}

// Gets a yes/no input from the user
bool getYesNoInput() {
    std::string input;
    while (true) {
        std::cin >> input;
        if (input == "yes" || input == "Yes" || input == "y" || input == "Y") {
            return true;
        } else if (input == "no" || input == "No" || input == "n" || input == "N") {
            return false;
        } else {
            std::cout << "Invalid input. Please enter yes or no: ";
        }
    }
}

// Handles payment for a reservation
void handlePayment(User* currentUser, std::map<int, Reservation*>& reservations, FacilityManager& facilityManager) {
    if (!currentUser) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    std::string eventName;
    std::cout << "Enter the event name to make a payment: ";
    std::cin >> eventName;

    for (const auto& pair : reservations) {
        Reservation* reservation = pair.second;
        if (reservation->getEvent()->getEventName() == eventName && reservation->getUser() == currentUser) {
            reservation->makePayment(facilityManager);
            Reservation::saveReservations("data/reservations.txt", reservations);
            std::cout << "Payment made successfully." << std::endl;
            return;
        }
    }

    std::cout << "Reservation not found or you do not have permission to make a payment for this reservation." << std::endl;
}
