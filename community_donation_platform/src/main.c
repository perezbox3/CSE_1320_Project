#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "items.h"

int main() {
    int choice;
    char logged_in_user[50];
    char logged_in_role[10];

    printf("Welcome to the Community Donation Platform\n");

    // Login or Signup Menu Loop
    int logged_in = 0;
    while (!logged_in) {
        printf("1. Login\n");
        printf("2. Signup\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                logged_in = login(logged_in_user, logged_in_role);
                if (!logged_in) {
                    printf("Login failed. Try again.\n");
                }
                break;
            case 2:
                signup();
                printf("\nSignup successful! Please log in.\n");
                break; // Instead of quitting, loops back to menu
            case 3:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    // Show menu based on user type after login
    while (1) {
        printf("\n===== Main Menu =====\n");
        printf("1. View Available Items\n");
        printf("2. Search for an Item\n");

        if (strcmp(logged_in_role, "donor") == 0) {
            printf("3. Add an Item\n");
        } else if (strcmp(logged_in_role, "recipient") == 0) {
            printf("3. Request an Item (Feature Not Implemented Yet)\n");
        }

        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_items();
                break;
            case 2:
                search_items();
                break;
            case 3:
                if (strcmp(logged_in_role, "donor") == 0) {
                    add_item();
                } else {
                    printf("Feature not implemented yet.\n");
                }
                break;
            case 4:
                printf("Logging out...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
