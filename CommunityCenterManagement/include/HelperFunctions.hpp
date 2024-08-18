#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include <iostream>
#include <map>
#include "User.hpp"
#include "Event.hpp"
#include "Reservation.hpp"
#include "FacilityManager.hpp"

// Function declarations for various helper functions
// Prints the main menu
void printMenu();   
// Handles the creation of a new user
void handleUserCreation(std::map<std::string, User*>& users);  
// Handles making a reservation
void handleReservation(Schedule& schedule, User* currentUser, std::map<int, Reservation*>& reservations);  
// Handles cancellation of a reservation
void handleCancellation(User* currentUser, std::map<int, Reservation*>& reservations, FacilityManager& facilityManager); 
// Lists all reservations
void listAllReservations(const std::map<int, Reservation*>& reservations);  
// Views detailed information of an event
void viewDetailedEvent(Schedule& schedule);  
// Views detailed information of a reservation
void viewDetailedReservation(const std::map<int, Reservation*>& reservations);  
// Handles event creation
void createEvent(FacilityManager& facilityManager, std::map<std::string, Event*>& events, const std::map<std::string, User*>& users, Schedule& schedule);  
// Handles user login
User* handleLogin(const std::map<std::string, User*>& users);  
// Gets a yes/no input from the user
bool getYesNoInput();  
// Handles payment for a reservation
void handlePayment(User* currentUser, std::map<int, Reservation*>& reservations, FacilityManager& facilityManager);  

#endif // HELPERFUNCTIONS_HPP
