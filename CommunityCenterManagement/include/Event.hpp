#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <map>
#include <ctime> 
#include "User.hpp"
#include "Reservation.hpp"

// Enum class for different layout styles for an event
enum class LayoutStyle {
    Meeting,   // Meeting layout style
    Lecture,   // Lecture layout style
    Wedding,   // Wedding layout style
    Dance      // Dance layout style
};

// Class representing an event in the community center
class Event {
public:
    // Constructor for creating an event
    Event(const std::string& eventName, User* organizer, const std::string& startTime, const std::string& endTime,
          LayoutStyle layoutStyle, bool isPublic, int maxGuests, double ticketPrice);

    // Getters for various member variables
    // Get the name of the event
    const std::string& getEventName() const;     
    // Get the organizer of the event
    User* getOrganizer() const;          
    // Get the start time of the event        
    const std::string& getStartTime() const;     
    // Get the end time of the event
    const std::string& getEndTime() const;      
    // Get the layout style of the event 
    LayoutStyle getLayoutStyle() const;   
    // Check if the event is public       
    bool isPublicEvent() const;         
    // Get the maximum number of guests for the event        
    int getMaxGuests() const;                   
    // Get the ticket price for the event 
    double getTicketPrice() const;               
    // Get the reservations for the event
    const std::vector<Reservation*>& getReservations() const; 
    // Check if the event is open to residents
    bool isOpenToResidents() const;      
    // Check if the event is open to non-residents        
    bool isOpenToNonResidents() const;           

    // Functions
    // Print basic details of the event
    void getEventDetails() const;          
    // Print detailed view of the event      
    void getDetailedView() const;            
    // Handle ticket purchase for a user   
    void purchaseTicket(User* user); 
    // Handle ticket cancellation for a user            
    void cancelTicket(User* user);            
    // Add a reservation to the event  
    void addReservation(Reservation* reservation); 
    // Add a user to the waitlist
    void addToWaitlist(User* user); 
    // Remove a user from the waitlist             
    void removeFromWaitlist(User* user);    
    // Check if the event is within operating hours     
    bool isWithinOperatingHours(const std::string& startTime, const std::string& endTime) const; 
    // Check if the reservation time is within event time
    bool isWithinEventTime(const std::string& reservationTime) const; 

    // Setters
    // Set if the event is open to residents
    void setOpenToResidents(bool openToResidents);   
    // Set if the event is open to non-residents    
    void setOpenToNonResidents(bool openToNonResidents); 

    // Static Functions
    // Load events from a file
    static std::map<std::string, Event*> loadEvents(const std::string& filename, const std::map<std::string, User*>& users); 
    // Save events to a file
    static void saveEvents(const std::string& filename, const std::map<std::string, Event*>& events); 

private:
    // Name of the event
    std::string eventName;     
    // Organizer of the event   
    User* organizer;              
    // Start time of the event
    std::string startTime;        
    // End time of the event
    std::string endTime;          
    // Layout style of the event
    LayoutStyle layoutStyle;      
    // Whether the event is public
    bool isPublic;                
    // Maximum number of guests for the event
    int maxGuests;                
    // Ticket price for the event
    double ticketPrice;           
    // Number of tickets sold
    int ticketsSold;              
    // Whether the event is open to residents
    bool openToResidents;         
    // Whether the event is open to non-residents
    bool openToNonResidents;      
    // Waitlist of users for the event
    std::vector<User*> waitlist;  
    // Reservations for the event
    std::vector<Reservation*> reservations; 

    // Helper function to parse time strings: Parse a time string into a std::tm structure
    bool parseTime(const std::string& timeStr, struct std::tm& time) const; 
};

#endif // EVENT_H
