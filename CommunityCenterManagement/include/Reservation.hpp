#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <map>
#include <vector>
#include "User.hpp"

// Forward declaration of Event class
class Event;
class FacilityManager;

// Enums for reservation and payment status
enum class ReservationStatus {
    Pending,
    Confirmed,
    Canceled
};

enum class PaymentStatus {
    Unpaid,
    Paid
};

// Class representing a reservation
class Reservation {
private:
    int reservationID;
    User* user;
    Event* event;  // Use the forward-declared Event class
    ReservationStatus reservationStatus;
    double totalCost;
    PaymentStatus paymentStatus;
    std::string reservationTime;
    static int nextReservationID; // Static variable for unique IDs

public:
    Reservation(int reservationID, User* user, Event* event, const std::string& reservationTime);
    
    int getReservationID() const { return reservationID; }
    User* getUser() const { return user; }
    Event* getEvent() const { return event; }
    ReservationStatus getReservationStatus() const { return reservationStatus; }
    double getTotalCost() const { return totalCost; }
    PaymentStatus getPaymentStatus() const { return paymentStatus; }
    std::string getReservationTime() const { return reservationTime; }

    double calculateCost();  // Calculates the total cost of the reservation
    void makePayment(FacilityManager& facilityManager);  // Handles payment for the reservation
    void processRefund(FacilityManager& facilityManager); // Processes a refund for the reservation

    void getDetailedView() const;  // Prints detailed information of the reservation

    static void saveReservations(const std::string& filename, const std::map<int, Reservation*>& reservations);  // Saves reservations to a file
    static std::map<int, Reservation*> loadReservations(const std::string& filename, const std::map<std::string, User*>& users, const std::map<std::string, Event*>& events);  // Loads reservations from a file
};

#endif // RESERVATION_H
