// main.c
// This program starts our Community Donation Platform. We let people log in as a donor or recipient.
// Donors can add items, see requests, and approve or reject them. Recipients can look at what's available, 
// search by category, request items, and see what they've gotten.

#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Standard library functions
#include <string.h>     // String handling
#include "user.h"       // User authentication stuff
#include "items.h"      // Functions for item management
#include "requests.h"   // Functions for handling requests

// Clears leftover input so that stray characters won't mess up the next input
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    int choice;
    char logged_in_user[50];  // stores the username of the current user
    char logged_in_role[10];  // "donor" or "recipient"

    // Greet the user
    printf("Welcome to the Community Donation Platform\n");

    // Run until user chooses to exit
    while (1) {
        int logged_in = 0;

        // Keep trying to log in or sign up until success
        while (!logged_in) {
            printf("\n===== Login Menu =====\n");
            printf("1. Login\n");
            printf("2. Signup\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");

            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            switch (choice) {
                case 1:
                    // Try logging in
                    logged_in = login(logged_in_user, logged_in_role);
                    if (!logged_in)
                        printf("Login failed. Try again.\n");
                    break;
                case 2:
                    // Sign up a new user
                    signup();
                    printf("\nSignup successful! Please log in.\n");
                    break;
                case 3:
                    // Quit the program
                    printf("Exiting program...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }

        // If we get here, user is logged in
        int logout = 0;
        while (!logout) {
            // Show donor menu
            if (strcmp(logged_in_role, "donor") == 0) {
                int pending_count = count_pending_requests(logged_in_user);

                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");
                printf("2. Search for an Item\n");
                printf("3. Add an Item\n");
                printf("4. View Inbox (%d)\n", pending_count);
                printf("5. Approve/Reject Requests\n");
                printf("6. Logout\n");
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                switch (choice) {
                    case 1:
                        display_items();
                        break;
                    case 2:
                        search_items();
                        break;
                    case 3:
                        add_item();
                        break;
                    case 4:
                        view_inbox(logged_in_user);
                        break;
                    case 5:
                        approve_request(logged_in_user);
                        break;
                    case 6:
                        printf("Logging out...\n");
                        logout = 1;
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }
            // Show recipient menu
            else if (strcmp(logged_in_role, "recipient") == 0) {
                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");
                printf("2. Search for an Item\n");
                printf("3. Request an Item\n");
                printf("4. View Inventory\n");
                printf("5. Logout\n");
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                switch (choice) {
                    case 1:
                        display_items();
                        break;
                    case 2:
                        search_items();
                        break;
                    case 3:
                        request_item(logged_in_user);
                        break;
                    case 4:
                        view_inventory(logged_in_user);
                        break;
                    case 5:
                        printf("Logging out...\n");
                        logout = 1;
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }
        }
    }

    return 0;
}
