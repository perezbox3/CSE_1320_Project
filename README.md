# Community Donation Platform

## Project Overview
Our Community Donation Platform is a C program designed to connect donors and recipients, facilitating the donation of essential items like clothes, furniture, and books. Users sign up as either donors or recipients, list or search for items, and request or approve donations. The system uses file-based storage to keep things simple and efficient.

## Features
- **User Management**: Signup & login system for donors and recipients.
- **Item Listings**: Donors can list items they want to donate.
- **Item Search**: Recipients can search for available donations.
- **Request System**: Recipients request items, and donors approve or reject requests.
- **File-Based Storage**: Users, items, and requests are stored in text files.

## Team Task Breakdown
### **Person 1: User Management & Authentication**
- Handles user signup and login.
- Stores user data securely in a file (`users.txt`).
- Ensures users are categorized as either **Donors** or **Recipients**.

### **Person 2: Item Listing & Searching**
- Allows donors to list items.
- Enables recipients to search for available items.
- Displays available donations in a readable format.

### **Person 3: Donation Requests & Approval System**
- Allows recipients to request items.
- Sends requests to donors (file-based notification system).
- Updates item status upon approval or rejection.

## File Structure
```
community_donation_platform/
│── src/                   # Source code files
│   ├── main.c             # Main entry point
│   ├── user.c             # User authentication functions
│   ├── user.h             # Header file for user-related functions
│   ├── items.c            # Functions for adding, listing, and searching items
│   ├── items.h            # Header file for item-related functions
│   ├── requests.c         # Functions for handling donation requests
│   ├── requests.h         # Header file for request management
│── data/                  # Stores program data
│   ├── users.txt          # Stores user details (username, password, role)
│   ├── items.txt          # Stores listed donation items
│   ├── requests.txt       # Stores pending requests for donations
│── docs/                  # Documentation & notes
│   ├── README.md          # Project documentation
│   ├── flowchart.png      # Optional: Program flowchart
│── Makefile               # Compilation instructions
```

## File Content & Functionality
### **User Management (`user.c, user.h`)**
- `signup()`: Registers users as Donors or Recipients.
- `login()`: Authenticates users.
- `load_users()`: Reads `users.txt` to check credentials.

**Data format in `users.txt`**:
```
username,password,role
john_doe,12345,donor
jane_smith,password,recipient
```

### **Item Management (`items.c, items.h`)**
- `add_item()`: Allows a donor to list an item.
- `display_items()`: Shows all available items.
- `search_items()`: Finds items by category or keyword.

**Data format in `items.txt`**:
```
item_id,donor_username,category,description,condition,status
1,john_doe,Clothes,Winter Jacket,Good,available
2,john_doe,Furniture,Wooden Chair,Fair,donated
```

### **Donation Requests (`requests.c, requests.h`)**
- `request_item()`: Recipients request an item.
- `approve_request()`: Donors approve or reject requests.
- `update_status()`: Changes item status upon approval.

**Data format in `requests.txt`**:
```
request_id,item_id,recipient_username,status
1,1,jane_smith,pending
2,2,mike_brown,approved
```

## Task Assignments
| **Person** | **Tasks** | **Files** |
|------------|----------|-----------|
| **Person 1** | User management (signup/login) | `user.c`, `user.h` |
| **Person 2** | Item listing & searching | `items.c`, `items.h` |
| **Person 3** | Request handling & approvals | `requests.c`, `requests.h` |

## Development Workflow
1. **Person 1** builds `user.c` to handle signup and login.
2. **Person 2** implements `items.c` for item management.
3. **Person 3** sets up `requests.c` to process donation requests.
4. Integrate all components into `main.c` with a **menu-driven system**.

## Additional Features (Future Enhancements)
- **Basic Admin Role** to monitor all requests.
- **Item Expiration System** to remove old listings.
- **CSV Import/Export** for better data handling.

---
This document serves as a guide for structuring and managing our project efficiently. Let’s make a positive impact with our C programming skills! 🚀

