#ifndef FACILITYMANAGER_H
#define FACILITYMANAGER_H

#include "Event.hpp"
#include "User.hpp"
#include "Schedule.hpp"
#include <vector>
#include <map>

// Class for managing facility events and budget
class FacilityManager {
public:
    FacilityManager();

    // Adds an event to the facility manager
    void addEvent(Event* event);   
    // Creates a new event
    void createEvent(std::map<std::string, Event*>& events, const std::map<std::string, User*>& users, Schedule& schedule); 
    // Cancels an existing event  
    void cancelEvent(std::map<std::string, Event*>& events, std::map<int, Reservation*>& reservations, const std::string& eventName);  
    
    // Returns the schedule of events
    Schedule& getSchedule();   
    // Returns the current facility budget
    double getFacilityBudget() const;   
    // Updates the facility budget by adding the specified amount
    void updateFacilityBudget(double amount);  

private:
    std::map<std::string, Event*> events;
    Schedule schedule;
    double facilityBudget;  // Facility budget for tracking payments and refunds
};

#endif // FACILITYMANAGER_H
