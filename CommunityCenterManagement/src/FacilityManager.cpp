#include "FacilityManager.hpp"
#include "Event.hpp"
#include "HelperFunctions.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

// Constructor initializing the facility budget
FacilityManager::FacilityManager() : facilityBudget(0.0) {}

// Adds an event to the facility manager and schedule
void FacilityManager::addEvent(Event* event) {
    events[event->getEventName()] = event;
    schedule.addEvent(event);  
}

// Returns the schedule of events
Schedule& FacilityManager::getSchedule() {
    return schedule;
}

// Creates a new event
void FacilityManager::createEvent(std::map<std::string, Event*>& events, const std::map<std::string, User*>& users, Schedule& schedule) {
    std::string name, organizerUsername, startTimeStr, endTimeStr;
    int layoutStyle, maxGuests;
    double ticketPrice;
    bool isPublic, openToResidents, openToNonResidents;

    std::cout << "Enter event name: ";
    std::getline(std::cin, name);

    std::cout << "Enter organizer username: ";
    std::getline(std::cin, organizerUsername);

    std::cout << "Enter start time (YYYY-MM-DDTHH:MM): ";
    std::getline(std::cin, startTimeStr);

    std::cout << "Enter end time (YYYY-MM-DDTHH:MM): ";
    std::getline(std::cin, endTimeStr);

    std::cout << "Enter layout style (integer 1-4): ";
    std::cin >> layoutStyle;

    std::cout << "Enter max guests: ";
    std::cin >> maxGuests;

    std::cout << "Enter ticket price: ";
    std::cin >> ticketPrice;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Is this event public? (yes/no): ";
    isPublic = getYesNoInput();

    std::cout << "Open to residents only? (yes/no): ";
    openToResidents = getYesNoInput();

    std::cout << "Open to non-residents? (yes/no): ";
    openToNonResidents = getYesNoInput();

    if (events.find(name) != events.end()) {
        std::cout << "Event with this name already exists. Please choose a different name." << std::endl;
        return;
    }

    if (users.find(organizerUsername) == users.end()) {
        std::cout << "Organizer username not found." << std::endl;
        return;
    }

    User* organizer = users.at(organizerUsername);
    std::tm startTime = {};
    std::tm endTime = {};
    std::istringstream startTimeStream(startTimeStr);
    std::istringstream endTimeStream(endTimeStr);
    startTimeStream >> std::get_time(&startTime, "%Y-%m-%dT%H:%M");
    endTimeStream >> std::get_time(&endTime, "%Y-%m-%dT%H:%M");

    int duration = std::difftime(mktime(&endTime), mktime(&startTime)) / 3600;

    if (!schedule.checkAvailability(startTimeStr, endTimeStr)) {
        std::cout << "The facility is not available during this time slot." << std::endl;
        return;
    }

    if (!organizer->checkWeeklyLimit(duration)) {
        std::cout << "This reservation exceeds the weekly limit for the organizer." << std::endl;
        return;
    }

    Event tempEvent(name, organizer, startTimeStr, endTimeStr, static_cast<LayoutStyle>(layoutStyle), isPublic, maxGuests, ticketPrice);
    if (!tempEvent.isWithinOperatingHours(startTimeStr, endTimeStr)) {
        std::cout << "The event time is outside the facility's operating hours." << std::endl;
        return;
    }

    Event* newEvent = new Event(name, organizer, startTimeStr, endTimeStr, static_cast<LayoutStyle>(layoutStyle), isPublic, maxGuests, ticketPrice);
    newEvent->setOpenToResidents(openToResidents);
    newEvent->setOpenToNonResidents(openToNonResidents);

    events[name] = newEvent;
    std::cout << "Event created successfully." << std::endl;
}

// Cancels an existing event and processes refunds for reservations
void FacilityManager::cancelEvent(std::map<std::string, Event*>& events, std::map<int, Reservation*>& reservations, const std::string& eventName) {
    auto it = events.find(eventName);
    if (it == events.end()) {
        std::cout << "Event not found." << std::endl;
        return;
    }

    Event* event = it->second;
    for (const auto& reservation : event->getReservations()) {
        User* user = reservation->getUser();
        double refundAmount = reservation->getTotalCost();
        user->processRefund(refundAmount);
        reservations.erase(reservation->getReservationID());
        std::cout << "Refund processed for user: " << user->getUsername() << " for amount: " << refundAmount << std::endl;
    }

    events.erase(it);
    delete event;
    std::cout << "Event canceled successfully." << std::endl;

    Event::saveEvents("data/events.txt", events);
    Reservation::saveReservations("data/reservations.txt", reservations);
}

// Returns the current facility budget
double FacilityManager::getFacilityBudget() const {
    return facilityBudget;
}

// Updates the facility budget by adding the specified amount
void FacilityManager::updateFacilityBudget(double amount) {
    facilityBudget += amount;
    std::cout << "Budget updated. Current budget: $" << facilityBudget << std::endl;
}
