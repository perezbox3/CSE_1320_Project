/*
 * main.c
 *
 * This is the main program for our Community Donation Platform.
 * It handles user logins, signups, and shows different menus depending on
 * whether the user is a donor or a recipient.
 *
 * Donors can add items, check donation requests, and approve or reject them.
 * Recipients can view items, search items, request them, and check their own inventory.
 */

#include <stdio.h>      // Standard input/output functions
#include <stdlib.h>     // Standard library (exit, etc.)
#include <string.h>     // String functions like strcmp()
#include "user.h"       // User login and signup functions
#include "items.h"      // Item-related functions
#include "requests.h"   // Request-related functions

// Clears leftover characters from input buffer to avoid reading errors
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    int choice;
    char logged_in_user[50]; // Stores username after login
    char logged_in_role[10]; // Stores role ("donor" or "recipient")

    printf("Welcome to the Community Donation Platform\n");

    // Main program loop, runs until user chooses to exit
    while (1) {
        int logged_in = 0;

        // Login and Signup Menu
        while (!logged_in) {
            printf("\n===== Login Menu =====\n");
            printf("1. Login\n");
            printf("2. Signup\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");

            // Makes sure the input is a number
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            switch (choice) {
                case 1:
                    // Checks login credentials
                    logged_in = login(logged_in_user, logged_in_role);
                    if (!logged_in)
                        printf("Login failed. Try again.\n");
                    break;
                case 2:
                    // Lets user create a new account
                    signup();
                    printf("\nSignup successful! Please log in.\n");
                    break;
                case 3:
                    // Exit the entire program
                    printf("Exiting program...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }

        // Once logged in, display menus based on the user's role
        int logout = 0;
        while (!logout) {

            // Menu options for Donors
            if (strcmp(logged_in_role, "donor") == 0) {
                // Count the number of new requests waiting for the donor
                int pending_count = count_pending_requests(logged_in_user);

                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");            // Show items people can donate
                printf("2. Search for an Item\n");              // Search items by category
                printf("3. Add an Item\n");                     // Add new donation item
                printf("4. View Inbox (%d)\n", pending_count);  // Check incoming requests
                printf("5. Approve/Reject Requests\n");         // Approve or reject donations
                printf("6. Logout\n");                          // Logout to main login menu
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                switch (choice) {
                    case 1:
                        display_items();  // Lists available items
                        break;
                    case 2:
                        search_items();   // Searches items by chosen category
                        break;
                    case 3:
                        add_item();       // Adds new item for donation
                        break;
                    case 4:
                        view_inbox(logged_in_user);  // Checks donation requests
                        break;
                    case 5:
                        approve_request(logged_in_user);  // Approves or rejects donation requests
                        break;
                    case 6:
                        printf("Logging out...\n");
                        logout = 1;    // Logout and return to login menu
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }

            // Menu options for Recipients
            else if (strcmp(logged_in_role, "recipient") == 0) {
                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");  // See items available to request
                printf("2. Search for an Item\n");    // Search items by category
                printf("3. Request an Item\n");       // Request an available item
                printf("4. View Inventory\n");        // See items you've gotten approved
                printf("5. Logout\n");                // Logout to main login menu
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                switch (choice) {
                    case 1:
                        display_items();  // Lists available items
                        break;
                    case 2:
                        search_items();   // Searches items by chosen category
                        break;
                    case 3:
                        request_item(logged_in_user);  // Requests an item
                        break;
                    case 4:
                        view_inventory(logged_in_user); // Shows recipient's approved items
                        break;
                    case 5:
                        printf("Logging out...\n");
                        logout = 1;    // Logout and return to login menu
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }
        }
    }

    return 0;
}
