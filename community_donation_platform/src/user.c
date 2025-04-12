// user.c
// This file handles user operations like signing up, logging in, and checking credentials.
// It uses a "users.txt" file to store and load usernames, passwords, and roles.

#include "user.h"   // For the User structure and function prototypes
#include <stdio.h>  // For input/output functions

// Clears leftover characters in stdin so they don't affect future inputs
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Lets a new user sign up by providing username, password, and role
void signup() {
    User newUser;

    // Ask for a username
    printf("Enter username: ");
    if (scanf("%49s", newUser.username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Ask for a password
    printf("Enter password: ");
    if (scanf("%49s", newUser.password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Ask for the user role (D or R)
    char roleChar;
    printf("Enter role (D for Donor, R for Recipient): ");
    if (scanf(" %c", &roleChar) != 1) {
        printf("Invalid input for role.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Convert that character to the right role string
    if (roleChar == 'D' || roleChar == 'd') {
        strcpy(newUser.role, "donor");
    } else if (roleChar == 'R' || roleChar == 'r') {
        strcpy(newUser.role, "recipient");
    } else {
        printf("Invalid role selection.\n");
        return;
    }

    // Save the new user into the file
    save_user(newUser);
    printf("Signup successful!\n");
}

// Lets a user log in by checking username and password against the file
int login(char *logged_in_user, char *logged_in_role) {
    char username[50], password[50];

    // Ask for username
    printf("Enter username: ");
    if (scanf("%49s", username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Ask for password
    printf("Enter password: ");
    if (scanf("%49s", password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Validate credentials and return 1 if good, 0 if not
    if (validate_credentials(username, password, logged_in_role)) {
        strcpy(logged_in_user, username);
        return 1;
    } else {
        printf("Invalid credentials!\n");
        return 0;
    }
}

// Loads all users from the users.txt file into an array
void load_users(User users[], int *user_count) {
    FILE *file = fopen("../data/users.txt", "r");
    if (!file) {
        printf("No user data found.\n");
        return;
    }

    // Skip the header line
    // (We assume the first line is always: "username,password,role")
    char header[200];
    if (fgets(header, sizeof(header), file) == NULL) {
        // The file was empty or had an unexpected format
        fclose(file);
        printf("No user data found.\n");
        return;
    }

    *user_count = 0;
    // Now read each user record line
    while (fscanf(file, "%49[^,],%49[^,],%9s\n",
                  users[*user_count].username,
                  users[*user_count].password,
                  users[*user_count].role) != EOF) {
        (*user_count)++;
    }
    fclose(file);
}

// Saves one new user to the end of the users file
void save_user(User newUser) {
    FILE *file = fopen("../data/users.txt", "a+");  // Changed 'a' to 'a+'
    if (!file) {
        printf("Error opening user file!\n");
        return;
    }

    // If the file is empty, write a header first
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    
    if (fileSize == 0) {
        fprintf(file, "username,password,role\n");
    } else if (fileSize > 0) {
        // Check if the last character is a newline
        fseek(file, -1, SEEK_END);
        char lastChar = fgetc(file);
        if (lastChar != '\n') {
            fprintf(file, "\n");  // Add a newline if needed
        }
        fseek(file, 0, SEEK_END);  // Move back to end for appending
    }

    // Now append this user's record
    fprintf(file, "%s,%s,%s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);
}

// Checks if the username and password match something in the users file
int validate_credentials(char *username, char *password, char *role) {
    FILE *file = fopen("../data/users.txt", "r");
    if (!file) {
        return 0;
    }

    // Skip the header line if it exists
    char header[200];
    if (fgets(header, sizeof(header), file) == NULL) {
        // empty file or read error
        fclose(file);
        return 0;
    }

    char stored_username[50], stored_password[50], stored_role[10];
    while (fscanf(file, "%49[^,],%49[^,],%9s\n",
                  stored_username,
                  stored_password,
                  stored_role) != EOF) {
        if (strcmp(username, stored_username) == 0 &&
            strcmp(password, stored_password) == 0) {
            strcpy(role, stored_role);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
