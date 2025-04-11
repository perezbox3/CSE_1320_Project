#include "user.h"
#include <stdio.h>

// Helper function to clear input buffer
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Function to register a new user
void signup() {
    User newUser;
    printf("Enter username: ");
    if (scanf("%49s", newUser.username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Enter password: ");
    if (scanf("%49s", newUser.password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Select role by letter instead of full word.
    char roleChar;
    printf("Enter role (D for Donor, R for Recipient): ");
    if (scanf(" %c", &roleChar) != 1) {
        printf("Invalid input for role.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (roleChar == 'D' || roleChar == 'd') {
        strcpy(newUser.role, "donor");
    } else if (roleChar == 'R' || roleChar == 'r') {
        strcpy(newUser.role, "recipient");
    } else {
        printf("Invalid role selection.\n");
        return;
    }

    // Save user to file
    save_user(newUser);
    printf("Signup successful!\n");
}

// Function to log in a user
int login(char *logged_in_user, char *logged_in_role) {
    char username[50], password[50];
    printf("Enter username: ");
    if (scanf("%49s", username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    printf("Enter password: ");
    if (scanf("%49s", password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    if (validate_credentials(username, password, logged_in_role)) {
        strcpy(logged_in_user, username);
        return 1;
    } else {
        printf("Invalid credentials!\n");
        return 0;
    }
}

// Function to load users from file
void load_users(User users[], int *user_count) {
    FILE *file = fopen("../data/users.txt", "r");
    if (file == NULL) {
        printf("No user data found.\n");
        return;
    }
    *user_count = 0;
    while (fscanf(file, "%49[^,],%49[^,],%9s\n",
                  users[*user_count].username,
                  users[*user_count].password,
                  users[*user_count].role) != EOF) {
        (*user_count)++;
    }
    fclose(file);
}

// Function to save a user to file
void save_user(User newUser) {
    FILE *file = fopen("../data/users.txt", "a");
    if (file == NULL) {
        printf("Error opening user file!\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);
}

// Function to validate user credentials
int validate_credentials(char *username, char *password, char *role) {
    FILE *file = fopen("../data/users.txt", "r");
    if (file == NULL) {
        return 0;
    }
    char stored_username[50], stored_password[50], stored_role[10];
    while (fscanf(file, "%49[^,],%49[^,],%9s\n", stored_username, stored_password, stored_role) != EOF) {
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            strcpy(role, stored_role);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
