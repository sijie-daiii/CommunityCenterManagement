#include "Reservation.hpp"
#include "Event.hpp"
#include "User.hpp"
#include "FacilityManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructor for the Reservation class
Reservation::Reservation(int reservationID, User* user, Event* event, const std::string& reservationTime)
    : reservationID(reservationID), user(user), event(event), reservationStatus(ReservationStatus::Pending),
      totalCost(0), paymentStatus(PaymentStatus::Unpaid), reservationTime(reservationTime) {
    totalCost = calculateCost();
    user->addReservation(this); // Add reservation to user's list
    event->addReservation(this); // Add reservation to event's list using the new method
}

// Calculates the total cost of the reservation
double Reservation::calculateCost() {
    // Implement cost calculation logic based on user type and event details
    int duration = std::stoi(event->getEndTime().substr(11, 2)) - std::stoi(event->getStartTime().substr(11, 2));
    double cost = duration * 10; // Standard service charge
    
    if (user->getUserType() == UserType::City) {
        cost += duration * 5;
    } else if (user->getUserType() == UserType::Organization) {
        cost += duration * 20;
    } else if (user->getUserType() == UserType::Resident) {
        cost += duration * 10;
    } else {
        cost += duration * 15;
    }

    return cost;
}

// Handles payment for the reservation
void Reservation::makePayment(FacilityManager& facilityManager) {
    paymentStatus = PaymentStatus::Paid;
    reservationStatus = ReservationStatus::Confirmed;
    facilityManager.updateFacilityBudget(totalCost); // Update the budget when payment is made
    std::cout << "Payment made for reservation " << reservationID << "." << std::endl;
}

// Processes a refund for the reservation
void Reservation::processRefund(FacilityManager& facilityManager) {
    paymentStatus = PaymentStatus::Unpaid;
    reservationStatus = ReservationStatus::Canceled;
    facilityManager.updateFacilityBudget(-totalCost); // Update the budget when refund is processed
    std::cout << "Refund processed for reservation " << reservationID << "." << std::endl;
}

// Prints detailed information of the reservation
void Reservation::getDetailedView() const {
    std::cout << "Reservation ID: " << reservationID << "\nUser: " << user->getUsername()
              << "\nEvent: " << event->getEventName() << "\nStatus: " << static_cast<int>(reservationStatus)
              << "\nPayment Status: " << static_cast<int>(paymentStatus) << "\nTotal Cost: " << totalCost
              << "\nReservation Time: " << reservationTime << std::endl;
}

// Loads reservations from a file
std::map<int, Reservation*> Reservation::loadReservations(const std::string& filename, const std::map<std::string, User*>& users, const std::map<std::string, Event*>& events) {
    std::map<int, Reservation*> reservations;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            int reservationID;
            std::string username, eventName, reservationStatusStr, paymentStatusStr, reservationTime;
            ReservationStatus reservationStatus = ReservationStatus::Pending;
            PaymentStatus paymentStatus = PaymentStatus::Unpaid;
            double totalCost;
            iss >> reservationID >> username >> eventName >> reservationStatusStr >> paymentStatusStr >> totalCost >> reservationTime;

            if (reservationStatusStr == "Pending") reservationStatus = ReservationStatus::Pending;
            else if (reservationStatusStr == "Confirmed") reservationStatus = ReservationStatus::Confirmed;
            else if (reservationStatusStr == "Canceled") reservationStatus = ReservationStatus::Canceled;

            if (paymentStatusStr == "Unpaid") paymentStatus = PaymentStatus::Unpaid;
            else if (paymentStatusStr == "Paid") paymentStatus = PaymentStatus::Paid;

            User* user = users.at(username);
            Event* event = events.at(eventName);
            Reservation* reservation = new Reservation(reservationID, user, event, reservationTime);
            reservation->reservationStatus = reservationStatus;
            reservation->totalCost = totalCost;
            reservation->paymentStatus = paymentStatus;
            reservations[reservationID] = reservation;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
    return reservations;
}

// Saves reservations to a file
void Reservation::saveReservations(const std::string& filename, const std::map<int, Reservation*>& reservations) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : reservations) {
            Reservation* reservation = pair.second;
            std::string reservationStatusStr, paymentStatusStr;
            switch (reservation->reservationStatus) {
                case ReservationStatus::Pending: reservationStatusStr = "Pending"; break;
                case ReservationStatus::Confirmed: reservationStatusStr = "Confirmed"; break;
                case ReservationStatus::Canceled: reservationStatusStr = "Canceled"; break;
            }
            switch (reservation->paymentStatus) {
                case PaymentStatus::Unpaid: paymentStatusStr = "Unpaid"; break;
                case PaymentStatus::Paid: paymentStatusStr = "Paid"; break;
            }
            file << reservation->getReservationID() << " " << reservation->getUser()->getUsername() << " " << reservation->getEvent()->getEventName() << " "
                 << reservationStatusStr << " " << paymentStatusStr << " " << reservation->getTotalCost() << " " << reservation->getReservationTime() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}
