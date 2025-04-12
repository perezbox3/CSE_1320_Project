// user.c
// -----------
// This file contains functions for managing user accounts in the donation platform.
// Users can sign up, log in, and their information is saved in a text file (users.txt).
// It also includes helper functions to clear input and validate user credentials.

#include "user.h"
#include <stdio.h>
#include <string.h>

// Helper function to clear leftover input from the buffer.
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Lets a new user create an account by choosing a username, password, and role.
void signup() {
    User newUser;

    // Ask for username.
    printf("Enter username: ");
    if (scanf("%49s", newUser.username) != 1) {
        printf("Invalid username input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Ask for password.
    printf("Enter password: ");
    if (scanf("%49s", newUser.password) != 1) {
        printf("Invalid password input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Choose user role (Donor or Recipient).
    char roleChar;
    printf("Enter role (D for Donor, R for Recipient): ");
    if (scanf(" %c", &roleChar) != 1) {
        printf("Invalid role input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Assign role based on user's choice.
    if (roleChar == 'D' || roleChar == 'd') {
        strcpy(newUser.role, "donor");
    } else if (roleChar == 'R' || roleChar == 'r') {
        strcpy(newUser.role, "recipient");
    } else {
        printf("Invalid role selection.\n");
        return;
    }

    // Save new user to file.
    save_user(newUser);
    printf("Signup successful!\n");
}

// Allows existing users to log into the system by verifying their username and password.
int login(char *logged_in_user, char *logged_in_role) {
    char username[50], password[50];

    // Get username.
    printf("Enter username: ");
    if (scanf("%49s", username) != 1) {
        printf("Invalid username input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Get password.
    printf("Enter password: ");
    if (scanf("%49s", password) != 1) {
        printf("Invalid password input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Check if username and password are correct.
    if (validate_credentials(username, password, logged_in_role)) {
        strcpy(logged_in_user, username);
        return 1;
    } else {
        printf("Invalid credentials!\n");
        return 0;
    }
}

// Loads user information from file into an array.
void load_users(User users[], int *user_count) {
    FILE *file = fopen("../data/users.txt", "r");
    if (file == NULL) {
        printf("No user data found.\n");
        return;
    }

    *user_count = 0;
    // Read user data from file.
    while (fscanf(file, "%49[^,],%49[^,],%9s\n",
                  users[*user_count].username,
                  users[*user_count].password,
                  users[*user_count].role) != EOF) {
        (*user_count)++;
    }
    fclose(file);
}

// Adds a new user record to the users file.
void save_user(User newUser) {
    FILE *file = fopen("../data/users.txt", "a");
    if (file == NULL) {
        printf("Could not open user file to save data.\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);
}

// Checks username and password against stored data.
int validate_credentials(char *username, char *password, char *role) {
    FILE *file = fopen("../data/users.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    char stored_username[50], stored_password[50], stored_role[10];
    // Look through file for matching username/password.
    while (fscanf(file, "%49[^,],%49[^,],%9s\n", stored_username, stored_password, stored_role) != EOF) {
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            strcpy(role, stored_role);
            fclose(file);
            return 1; // Found matching credentials.
        }
    }
    fclose(file);
    return 0; // No match found.
}
