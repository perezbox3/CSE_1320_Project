#include "user.h"   // Include header file for user-related definitions and function prototypes.
#include <stdio.h>  // Standard I/O functions (printf, scanf, etc.)

//------------------------------------
// Helper function: clear_input_buffer
//------------------------------------
/*
 * clear_input_buffer()
 *
 * Description:
 *   This static helper function flushes (clears) any remaining characters
 *   from the standard input (stdin) until a newline ('\n') or the end of file (EOF)
 *   is encountered.
 *
 * Purpose:
 *   After using input functions like scanf(), there may be extra characters (e.g.,
 *   the newline from pressing Enter). These extraneous characters may interfere with
 *   subsequent input. Calling this function ensures that the input buffer is clean.
 *
 * Note:
 *   The function is declared as "static" to limit its scope to this file only.
 */
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//------------------------------------
// Function: signup
//------------------------------------
/*
 * signup()
 *
 * Description:
 *   This function facilitates user registration (signup) by prompting the user to
 *   enter a username, password, and role (Donor or Recipient). The role is selected
 *   by entering a single character ('D' or 'R', case-insensitive).
 *
 * Process:
 *   1. Prompts the user for a username and verifies the input.
 *   2. Prompts for a password and validates the input.
 *   3. Asks the user to choose their role by entering either 'D' or 'R'.
 *      - If the entered character is 'D' or 'd', the user is set as a "donor".
 *      - If the character is 'R' or 'r', the user is set as a "recipient".
 *   4. If the role is valid, the new user data is saved to the users file using save_user().
 *   5. If any input is invalid, appropriate error messages are displayed, and
 *      the process stops without saving.
 *
 * Important:
 *   clear_input_buffer() is called after each input to flush the input buffer and
 *   ensure clean input for subsequent prompts.
 */
void signup() {
    User newUser;  // Local variable to store new user information.

    // Prompt and read the username.
    printf("Enter username: ");
    if (scanf("%49s", newUser.username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer(); // Clean up leftover input.

    // Prompt and read the password.
    printf("Enter password: ");
    if (scanf("%49s", newUser.password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Prompt for role using a single character (D or R).
    char roleChar;
    printf("Enter role (D for Donor, R for Recipient): ");
    if (scanf(" %c", &roleChar) != 1) {  // The leading space ensures whitespace is ignored.
        printf("Invalid input for role.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Determine role based on user input.
    if (roleChar == 'D' || roleChar == 'd') {
        strcpy(newUser.role, "donor");
    } else if (roleChar == 'R' || roleChar == 'r') {
        strcpy(newUser.role, "recipient");
    } else {
        printf("Invalid role selection.\n");
        return;
    }

    // Save the new user's information to the file.
    save_user(newUser);
    printf("Signup successful!\n");
}

//------------------------------------
// Function: login
//------------------------------------
/*
 * login(char *logged_in_user, char *logged_in_role)
 *
 * Description:
 *   This function logs a user in by prompting for the username and password.
 *
 * Process:
 *   1. Prompts the user to enter their username.
 *   2. Prompts the user to enter their password.
 *   3. Calls validate_credentials() to check the provided credentials against
 *      those stored in the users file.
 *   4. If the credentials match, the logged_in_user is set to the username,
 *      and logged_in_role is set to the corresponding role (donor or recipient).
 *   5. Returns 1 (true) for a successful login; otherwise, prints an error
 *      and returns 0 (false).
 *
 * Note:
 *   The function uses clear_input_buffer() to ensure a clean input stream between prompts.
 */
int login(char *logged_in_user, char *logged_in_role) {
    char username[50], password[50];

    // Prompt for username.
    printf("Enter username: ");
    if (scanf("%49s", username) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Prompt for password.
    printf("Enter password: ");
    if (scanf("%49s", password) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    // Validate the provided credentials.
    if (validate_credentials(username, password, logged_in_role)) {
        // On successful validation, copy username into the provided logged_in_user variable.
        strcpy(logged_in_user, username);
        return 1;
    } else {
        // If credentials are not valid, inform the user.
        printf("Invalid credentials!\n");
        return 0;
    }
}

//------------------------------------
// Function: load_users
//------------------------------------
/*
 * load_users(User users[], int *user_count)
 *
 * Description:
 *   This function loads all user records from the users file into an array of User structures.
 *
 * Process:
 *   1. Opens the users file (at the relative path "../data/users.txt") for reading.
 *   2. If no file is found, prints an error message and returns.
 *   3. Reads each user record using fscanf with appropriate field widths to avoid overflow.
 *   4. Increments the count of users (user_count) accordingly.
 *   5. Closes the file after reading.
 *
 * Parameters:
 *   users      - An array of User structures to hold the loaded user data.
 *   user_count - A pointer to an integer that will be updated with the total number of users read.
 */
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

//------------------------------------
// Function: save_user
//------------------------------------
/*
 * save_user(User newUser)
 *
 * Description:
 *   Saves a new user's information to the users file. The user information is appended
 *   to the file in CSV format.
 *
 * Process:
 *   1. Opens the users file ("../data/users.txt") in append mode.
 *   2. If the file cannot be opened, prints an error message and returns.
 *   3. Writes the username, password, and role (separated by commas) followed by a newline.
 *   4. Closes the file.
 */
void save_user(User newUser) {
    FILE *file = fopen("../data/users.txt", "a");
    if (file == NULL) {
        printf("Error opening user file!\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);
}

//------------------------------------
// Function: validate_credentials
//------------------------------------
/*
 * validate_credentials(char *username, char *password, char *role)
 *
 * Description:
 *   Validates a user's credentials by comparing the input username and password
 *   with those stored in the users file.
 *
 * Process:
 *   1. Opens the users file for reading.
 *   2. Reads each user record, comparing the provided username and password with
 *      the stored ones.
 *   3. If a match is found, copies the corresponding role to the provided role variable,
 *      closes the file, and returns 1 (true).
 *   4. If no match is found or an error occurs, returns 0 (false).
 *
 * Returns:
 *   1 if the credentials match a stored user; 0 otherwise.
 */
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
