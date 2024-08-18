# Community Center Management System
![1](https://github.com/user-attachments/assets/d4d3dba7-8564-4993-9564-ab6a3f40734c)


## Overview

The Community Center Management System is a C++ application designed to manage the reservations, events, and user interactions for a community center. The system includes functionalities for different types of users (City, Residents, Non-residents, and Organizations) to create, view, reserve, and cancel events. It also maintains the facility's budget, handles payments, and ensures the enforcement of reservation rules.

## Features

1. **User Management:**
   - Create new users with different roles (City, Organization, Resident, Non-resident).
   - Login and logout functionalities.
   - Edit user information.

2. **Event Management:**
   - Create new events with specific details like event name, organizer, start and end times, layout style, max guests, and ticket price.
   - View detailed information about events.
   - Cancel events and process refunds for reservations.

3. **Reservation Management:**
   - Make reservations for events.
   - View all reservations and detailed reservation information.
   - Cancel reservations and process refunds.
   - Purchase and cancel tickets for events.

4. **Schedule Management:**
   - View today's schedule of events.
   - Check facility availability for event creation.
   - Maintain facility's budget with detailed updates on payments and refunds.

5. **Facility Manager Functions:**
   - Enforce reservation rules, including overlapping slots and maximum reservation time per week.
   - Check for payments and bookings by users.
   - Maintain the facility budget and process refunds upon cancellation.

## Installation

1. Clone the repository from GitHub:
   ```sh
   git clone https://github.com/your-repo/community-center-management.git
   cd community-center-management
   ```

2. Compile the program using the provided Makefile:
   ```sh
   make
   ```

## Usage

1. Run the executable:
   ```sh
   ./CommunityCenterManagement
   ```

2. Follow the on-screen menu to navigate through different functionalities:
   - Login with a username and password.
   - Create new users.
   - View today's schedule of events.
   - Make and cancel reservations.
   - Create and cancel events.
   - Make payments and view the facility budget.

## File Organization

- **src/**: Contains all the source code files for different classes and functionalities.
  - `main.cpp`: The main entry point of the application.
  - `User.cpp`: Implementation of user-related functionalities.
  - `Event.cpp`: Implementation of event-related functionalities.
  - `Reservation.cpp`: Implementation of reservation-related functionalities.
  - `FacilityManager.cpp`: Implementation of facility manager functionalities.
  - `HelperFunctions.cpp`: Implementation of helper functions used across the application.

- **include/**: Contains all the header files defining the interfaces and data structures.
  - `User.hpp`: Definition of user classes and types.
  - `Event.hpp`: Definition of event class and related types.
  - `Reservation.hpp`: Definition of reservation class and related types.
  - `FacilityManager.hpp`: Definition of facility manager class.
  - `HelperFunctions.hpp`: Declaration of helper functions.

- **data/**: Directory to store persistent data files.
  - `users.txt`: Stores user information.
  - `events.txt`: Stores event information.
  - `reservations.txt`: Stores reservation information.

- **obj/**: Directory to store compiled object files.

- **Makefile**: Build script to compile the project.

## Design

### UML Diagrams

- **Class Diagrams**: Show relationships between different classes such as `User`, `Event`, `Reservation`, and `FacilityManager`.
- **Sequence Diagrams**: Illustrate the interactions between objects for key functionalities like making a reservation, creating an event, and processing a payment.

### CRC Cards

- **User Class**:
  - **Responsibilities**: Manage user information, handle login/logout, view/edit user info, make/cancel reservations.
  - **Collaborators**: `Reservation`, `Event`, `FacilityManager`.

- **Event Class**:
  - **Responsibilities**: Manage event details, handle ticket sales/cancellations, manage waitlist, check operating hours.
  - **Collaborators**: `User`, `Reservation`, `FacilityManager`.

- **Reservation Class**:
  - **Responsibilities**: Manage reservation details, calculate costs, handle payments/refunds.
  - **Collaborators**: `User`, `Event`, `FacilityManager`.

- **FacilityManager Class**:
  - **Responsibilities**: Manage facility budget, enforce reservation rules, create/cancel events.
  - **Collaborators**: `User`, `Event`, `Reservation`.

### Discussion of Design

The design follows object-oriented principles to ensure modularity, reusability, and maintainability. Each class encapsulates its responsibilities and interacts with other classes through well-defined interfaces. This separation of concerns makes the system extensible and easy to manage.

### Quality of Design

- **Design Principles**:
  - **Encapsulation**: Each class encapsulates its data and provides public methods for interaction.
  - **Modularity**: The system is divided into modules with specific responsibilities.
  - **Reusability**: Code is organized to promote reuse of components.
  - **Maintainability**: Clear separation of concerns and use of inheritance enhance maintainability.

- **UML Diagrams and CRC Cards**: Provided in the documentation, illustrating the relationships and interactions between different components of the system.

## Testing

The system has been tested for various scenarios to ensure its robustness:
- Creating and managing users.
- Creating and managing events.
- Making and cancelling reservations.
- Handling payments and refunds.
- Enforcing reservation rules and maintaining the facility budget.

## Data Persistence

All user, event, and reservation data are saved to text files (`users.txt`, `events.txt`, `reservations.txt`) to ensure data persistence across sessions. The system can successfully save the state and load it back without losing any data.

## Memory Management

The system uses proper memory management techniques, including the use of smart pointers where necessary, to ensure there are no memory leaks. All dynamic memory is properly allocated and deallocated.

## Compilation

The program compiles successfully without errors using the provided Makefile. It has been tested on the Khoury server to ensure compatibility and stability.
