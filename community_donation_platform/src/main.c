#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "items.h"
#include "requests.h"

// Helper function to clear any leftover input.
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    int choice;
    char logged_in_user[50];
    char logged_in_role[10];

    printf("Welcome to the Community Donation Platform\n");

    // Login or Signup Menu Loop
    while (1) {
        int logged_in = 0;
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
                    logged_in = login(logged_in_user, logged_in_role);
                    if (!logged_in)
                        printf("Login failed. Try again.\n");
                    break;
                case 2:
                    signup();
                    printf("\nSignup successful! Please log in.\n");
                    break;
                case 3:
                    printf("Exiting program...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }

        // Main Menu Loop for Logged-In Users
        int logout = 0;
        while (!logout) {
            // Donor Menu
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
            // Recipient Menu
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
