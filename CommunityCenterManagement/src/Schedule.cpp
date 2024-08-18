#include "Schedule.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

// Adds an event to the schedule
void Schedule::addEvent(Event* event) {
    events.push_back(event);
}

// Removes an event from the schedule
void Schedule::removeEvent(int eventID) {
    // Implement event removal logic
    std::cout << "Event removed." << std::endl;
}

// Views the current schedule
void Schedule::viewSchedule() const {
    std::cout << "Viewing schedule." << std::endl;
    if (events.empty()) {
        std::cout << "No events scheduled." << std::endl;
    } else {
        for (const auto& event : events) {
            event->getEventDetails();
        }
    }
}

// Checks if a time slot is available
bool Schedule::checkAvailability(const std::string& startTime, const std::string& endTime) const {
    std::cout << "Checking availability." << std::endl;
    std::tm newStartTime = {};
    std::tm newEndTime = {};
    std::istringstream newStartTimeStream(startTime);
    std::istringstream newEndTimeStream(endTime);
    newStartTimeStream >> std::get_time(&newStartTime, "%Y-%m-%dT%H:%M");
    newEndTimeStream >> std::get_time(&newEndTime, "%Y-%m-%dT%H:%M");

    for (const auto& event : events) {
        std::tm eventStartTime = {};
        std::tm eventEndTime = {};
        std::istringstream eventStartTimeStream(event->getStartTime());
        std::istringstream eventEndTimeStream(event->getEndTime());
        eventStartTimeStream >> std::get_time(&eventStartTime, "%Y-%m-%dT%H:%M");
        eventEndTimeStream >> std::get_time(&eventEndTime, "%Y-%m-%dT%H:%M");

        if (mktime(&newStartTime) < mktime(&eventEndTime) && mktime(&newEndTime) > mktime(&eventStartTime)) {
            std::cout << "Time slot is not available due to overlap with event: " << event->getEventName() << std::endl;
            return false;
        }
    }
    return true;
}

// Finds an event by name
Event* Schedule::findEvent(const std::string& eventName) const {
    for (const auto& event : events) {
        if (event->getEventName() == eventName) {
            return event;
        }
    }
    std::cout << "Event " << eventName << " not found." << std::endl;
    return nullptr;
}
