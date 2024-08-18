#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <vector>
#include "Event.hpp"

// Class representing a schedule of events
class Schedule {
public:
    // Adds an event to the schedule
    void addEvent(Event* event);  
    // Removes an event from the schedule
    void removeEvent(int eventID);  
    // Checks if a time slot is available
    bool checkAvailability(const std::string& startTime, const std::string& endTime) const;  
    // Views the current schedule
    void viewSchedule() const; 
    // Finds an event by name
    Event* findEvent(const std::string& eventName) const;  

private:
    std::vector<Event*> events;
};

#endif // SCHEDULE_HPP
