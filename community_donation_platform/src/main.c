// main.c
// This is the main entry point for the Community Donation Platform.
// The program supports two types of users – donors and recipients – 
// and displays different menus depending on the user role.
// Donors can add items, view pending donation requests (inbox), and approve/reject requests.
// Recipients can view available items, search for items, request items, and view their inventory.

#include <stdio.h>      // Standard I/O functions (printf, scanf, etc.)
#include <stdlib.h>     // Standard library functions (exit, etc.)
#include <string.h>     // String manipulation functions
#include "user.h"       // Functions and definitions for user authentication (login, signup)
#include "items.h"      // Functions and definitions for item management (add, display, search, update_status)
#include "requests.h"   // Functions and definitions for donation requests and notifications

//------------------------------------
// clear_input_buffer()
// -----------------------------------
// This helper function discards any remaining characters
// in the input buffer (stdin) until a newline or EOF is reached.
// This is useful after reading input (e.g., with scanf) to ensure
// that stray newline characters do not interfere with subsequent input.
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//------------------------------------
// main()
// ------------------------------------
// The main function implements the primary loop of the program.
// It begins with a login/signup loop and, once the user is authenticated,
// displays a menu appropriate for the user's role (donor or recipient).
int main() {
    int choice;
    char logged_in_user[50];  // Holds the username of the currently logged-in user.
    char logged_in_role[10];    // Holds the role ("donor" or "recipient") of the logged-in user.

    // Print the welcome message.
    printf("Welcome to the Community Donation Platform\n");

    // Outer loop: continues running until the user selects to exit the program.
    while (1) {
        int logged_in = 0;

        // Login/Signup Menu Loop: Continue to prompt until the user logs in successfully.
        while (!logged_in) {
            printf("\n===== Login Menu =====\n");
            printf("1. Login\n");
            printf("2. Signup\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");

            // If the user enters non-numeric input, prompt again.
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            // Process the user's selection.
            switch (choice) {
                case 1:
                    // Call login() which verifies the username and password.
                    logged_in = login(logged_in_user, logged_in_role);
                    if (!logged_in)
                        printf("Login failed. Try again.\n");
                    break;
                case 2:
                    // Call signup() to register a new user.
                    signup();
                    printf("\nSignup successful! Please log in.\n");
                    break;
                case 3:
                    // Exit the program.
                    printf("Exiting program...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
            }
        } // End of login/signup loop.

        // Main Menu Loop for Logged-In Users.
        // This loop remains active until the user chooses to log out.
        int logout = 0;
        while (!logout) {
            // Check the role of the logged-in user to display the appropriate menu.
            // Donor Menu: If user is a donor.
            if (strcmp(logged_in_role, "donor") == 0) {
                // Calculate the number of pending requests (notifications) for the donor.
                int pending_count = count_pending_requests(logged_in_user);

                // Display the donor main menu with six options.
                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");     // List all items that are available to be donated.
                printf("2. Search for an Item\n");          // Allow the donor to search items by category.
                printf("3. Add an Item\n");                 // Let the donor add a new donation item.
                printf("4. View Inbox (%d)\n", pending_count); // Show number of pending requests.
                printf("5. Approve/Reject Requests\n");     // Allow the donor to approve or reject donation requests.
                printf("6. Logout\n");
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                // Switch based on donor's menu selection.
                switch (choice) {
                    case 1:
                        display_items();  // Display all available donation items.
                        break;
                    case 2:
                        search_items();   // Search available items by selected category.
                        break;
                    case 3:
                        add_item();       // Prompt for adding a new item to the donation list.
                        break;
                    case 4:
                        view_inbox(logged_in_user);  // Display pending donation requests (inbox) for the donor.
                        break;
                    case 5:
                        approve_request(logged_in_user); // Provide a list of pending requests and allow approval/rejection.
                        break;
                    case 6:
                        printf("Logging out...\n");
                        logout = 1;    // Break out of the main menu loop to return to the login/signup menu.
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }
            // Recipient Menu: If user is a recipient.
            else if (strcmp(logged_in_role, "recipient") == 0) {
                // Display the recipient main menu with five options.
                printf("\n===== Main Menu =====\n");
                printf("1. View Available Items\n");     // List all items that are available.
                printf("2. Search for an Item\n");          // Allow searching for items by category.
                printf("3. Request an Item\n");             // Let the recipient request an item.
                printf("4. View Inventory\n");              // Show items that have been approved (received) by the recipient.
                printf("5. Logout\n");
                printf("Enter your choice: ");

                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                // Switch based on recipient's menu selection.
                switch (choice) {
                    case 1:
                        display_items();   // Display available items.
                        break;
                    case 2:
                        search_items();    // Search for items by category.
                        break;
                    case 3:
                        request_item(logged_in_user); // Request an item; the function lists available items first.
                        break;
                    case 4:
                        view_inventory(logged_in_user); // Display the recipient's inventory of approved items.
                        break;
                    case 5:
                        printf("Logging out...\n");
                        logout = 1;    // Exit recipient's main menu.
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            }
        } // End of main menu loop for logged-in user.
    } // End of outer loop (program runs continuously until exit is selected).

    return 0;
}
