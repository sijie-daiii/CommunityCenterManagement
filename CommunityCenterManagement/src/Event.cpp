#include "Event.hpp"
#include "User.hpp"
#include "Reservation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>

// Constructor for the Event class
Event::Event(const std::string& eventName, User* organizer, const std::string& startTime, const std::string& endTime,
             LayoutStyle layoutStyle, bool isPublic, int maxGuests, double ticketPrice)
    : eventName(eventName), organizer(organizer), startTime(startTime), endTime(endTime), layoutStyle(layoutStyle),
      isPublic(isPublic), maxGuests(maxGuests), ticketPrice(ticketPrice), openToResidents(true), openToNonResidents(true), ticketsSold(0) {}

// Getters for various member variables
const std::string& Event::getEventName() const {
    return eventName;
}

User* Event::getOrganizer() const {
    return organizer;
}

const std::string& Event::getStartTime() const {
    return startTime;
}

const std::string& Event::getEndTime() const {
    return endTime;
}

LayoutStyle Event::getLayoutStyle() const {
    return layoutStyle;
}

bool Event::isPublicEvent() const {
    return isPublic;
}

int Event::getMaxGuests() const {
    return maxGuests;
}

double Event::getTicketPrice() const {
    return ticketPrice;
}

const std::vector<Reservation*>& Event::getReservations() const {
    return reservations;
}

bool Event::isOpenToResidents() const {
    return openToResidents;
}

bool Event::isOpenToNonResidents() const {
    return openToNonResidents;
}

// Helper function to parse time strings
bool Event::parseTime(const std::string& timeStr, struct std::tm& time) const {
    std::istringstream ss(timeStr);
    ss >> std::get_time(&time, "%Y-%m-%dT%H:%M");
    return !ss.fail();
}

// Checks if the event is within operating hours
bool Event::isWithinOperatingHours(const std::string& startTime, const std::string& endTime) const {
    // Define operating hours
    std::tm facilityOpenTime = {}, facilityCloseTime = {};
    facilityOpenTime.tm_hour = 8;  // Facility opens at 8 AM
    facilityCloseTime.tm_hour = 23;  // Facility closes at 11 PM

    // Parse event times
    std::tm eventStartTime = {}, eventEndTime = {};
    if (!parseTime(startTime, eventStartTime) || !parseTime(endTime, eventEndTime)) {
        return false;
    }

    // Compare event times with operating hours
    return (eventStartTime.tm_hour >= facilityOpenTime.tm_hour && eventEndTime.tm_hour <= facilityCloseTime.tm_hour);
}

// Prints event details
void Event::getEventDetails() const {
    std::cout << "Event: " << eventName << "\nOrganizer: " << organizer->getUsername() << "\nStart Time: " << startTime << "\nEnd Time: " << endTime
              << "\nLayout Style: " << static_cast<int>(layoutStyle) << "\nPublic: " << (isPublic ? "Yes" : "No")
              << "\nMax Guests: " << maxGuests << "\nTicket Price: $" << ticketPrice << "\nTickets Sold: " << ticketsSold
              << "\nReservations: " << std::endl;
    for (const auto& reservation : reservations) {
        std::cout << " - Reservation ID: " << reservation->getReservationID() << ", User: " << reservation->getUser()->getUsername() << std::endl;
    }
}

// Prints detailed view of the event
void Event::getDetailedView() const {
    std::cout << "Event: " << eventName << "\nOrganizer: " << organizer->getUsername() << "\nStart Time: " << startTime << "\nEnd Time: " << endTime
              << "\nLayout Style: " << static_cast<int>(layoutStyle) << "\nPublic: " << (isPublic ? "Yes" : "No")
              << "\nMax Guests: " << maxGuests << "\nTicket Price: $" << ticketPrice << "\nTickets Sold: " << ticketsSold
              << "\nWaitlist: " << std::endl;
    for (const auto& user : waitlist) {
        std::cout << " - " << user->getUsername() << std::endl;
    }
    std::cout << "Reservations: " << std::endl;
    for (const auto& reservation : reservations) {
        std::cout << " - Reservation ID: " << reservation->getReservationID() << ", User: " << reservation->getUser()->getUsername() << std::endl;
    }
}

// Handles ticket purchase
void Event::purchaseTicket(User* user) {
    if (!openToNonResidents && user->getUserType() == UserType::NonResident) {
        std::cout << "This event is not open to non-residents. Ticket purchase denied for user " << user->getUsername() << "." << std::endl;
        return;
    }
    if (ticketsSold < maxGuests) {
        ++ticketsSold;
        std::cout << "Ticket purchased by user " << user->getUsername() << "." << std::endl;
    } else {
        addToWaitlist(user);
        std::cout << "Event full. User " << user->getUsername() << " added to waitlist." << std::endl;
    }
}

// Handles ticket cancellation
void Event::cancelTicket(User* user) {
    if (ticketsSold > 0) {
        --ticketsSold;
        std::cout << "Ticket canceled by user " << user->getUsername() << "." << std::endl;
    } else {
        std::cout << "No tickets to cancel for user " << user->getUsername() << "." << std::endl;
    }
    // Implement waitlist management
}

// Adds a user to the waitlist
void Event::addToWaitlist(User* user) {
    waitlist.push_back(user);
}

// Removes a user from the waitlist
void Event::removeFromWaitlist(User* user) {
    waitlist.erase(std::remove(waitlist.begin(), waitlist.end(), user), waitlist.end());
}

// Adds a reservation to the event
void Event::addReservation(Reservation* reservation) {
    reservations.push_back(reservation);
}

// Sets if the event is open to residents
void Event::setOpenToResidents(bool openToResidents) {
    this->openToResidents = openToResidents;
}

// Sets if the event is open to non-residents
void Event::setOpenToNonResidents(bool openToNonResidents) {
    this->openToNonResidents = openToNonResidents;
}

// Loads events from a file
std::map<std::string, Event*> Event::loadEvents(const std::string& filename, const std::map<std::string, User*>& users) {
    std::map<std::string, Event*> events;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string eventName, organizerName, startTime, endTime, layoutStyleStr, isPublicStr;
            LayoutStyle layoutStyle = LayoutStyle::Meeting;
            bool isPublic;
            int maxGuests;
            double ticketPrice;
            bool openToResidents, openToNonResidents;
            iss >> eventName >> organizerName >> startTime >> endTime >> layoutStyleStr >> isPublicStr >> maxGuests >> ticketPrice >> openToResidents >> openToNonResidents;

            if (layoutStyleStr == "Meeting") layoutStyle = LayoutStyle::Meeting;
            else if (layoutStyleStr == "Lecture") layoutStyle = LayoutStyle::Lecture;
            else if (layoutStyleStr == "Wedding") layoutStyle = LayoutStyle::Wedding;
            else if (layoutStyleStr == "Dance") layoutStyle = LayoutStyle::Dance;

            isPublic = (isPublicStr == "true");

            if (users.find(organizerName) == users.end()) {
                std::cerr << "Error: Organizer " << organizerName << " not found in users map." << std::endl;
                continue;
            }

            User* organizer = users.at(organizerName);
            Event* event = new Event(eventName, organizer, startTime, endTime, layoutStyle, isPublic, maxGuests, ticketPrice);
            event->setOpenToResidents(openToResidents);
            event->setOpenToNonResidents(openToNonResidents);
            events[eventName] = event;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
    return events;
}

// Saves events to a file
void Event::saveEvents(const std::string& filename, const std::map<std::string, Event*>& events) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : events) {
            Event* event = pair.second;
            std::string layoutStyleStr;
            switch (event->layoutStyle) {
                case LayoutStyle::Meeting: layoutStyleStr = "Meeting"; break;
                case LayoutStyle::Lecture: layoutStyleStr = "Lecture"; break;
                case LayoutStyle::Wedding: layoutStyleStr = "Wedding"; break;
                case LayoutStyle::Dance: layoutStyleStr = "Dance"; break;
            }
            file << event->getEventName() << " " << event->getOrganizer()->getUsername() << " " << event->getStartTime() << " " << event->getEndTime() << " "
                 << layoutStyleStr << " " << (event->isPublic ? "true" : "false") << " " << event->getMaxGuests() << " " << event->getTicketPrice() << " "
                 << event->isOpenToResidents() << " " << event->isOpenToNonResidents() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}

// Checks if the reservation time is within the event time
bool Event::isWithinEventTime(const std::string& reservationTime) const {
    std::tm resTime = {};
    if (!parseTime(reservationTime, resTime)) {
        return false;
    }

    std::tm eventStartTime = {}, eventEndTime = {};
    if (!parseTime(startTime, eventStartTime) || !parseTime(endTime, eventEndTime)) {
        return false;
    }

    return (mktime(&resTime) >= mktime(&eventStartTime) && mktime(&resTime) <= mktime(&eventEndTime));
}
