#ifndef USER_H
#define USER_H

/*
 * user.h
 * -------
 * This header file defines the User structure and declares the functions used for 
 * user management and authentication in the Community Donation Platform.
 *
 * The file is organized as follows:
 *   1. Inclusion of standard libraries needed for I/O and string manipulation.
 *   2. Definition of the User structure, which holds user account information.
 *   3. Declaration of functions for:
 *         - Registering a new user (signup)
 *         - Authenticating a user (login)
 *         - Loading user data from persistent storage (load_users)
 *         - Saving new user data (save_user)
 *         - Validating credentials for login (validate_credentials)
 *
 * Each function is designed to support proper input handling, error checking, and
 * data storage for a simple file-based user management system.
 */

#include <stdio.h>      // Provides standard input/output functions.
#include <stdlib.h>     // Provides general utilities like memory allocation and program control.
#include <string.h>     // Provides string handling functions.

/*
 * Structure: User
 * ----------------
 * Represents a user account in the system.
 *
 * Fields:
 *   username:  A character array that stores the user's name (up to 49 characters plus the null terminator).
 *   password:  A character array that stores the user's password (up to 49 characters plus the null terminator).
 *   role:      A character array that specifies the user's role in the system.
 *              Expected values are "donor" or "recipient".
 */
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // Expected values: "donor" or "recipient"
} User;

/*
 * Function Declarations:
 *
 * signup()
 * --------
 * Purpose:
 *    Registers a new user by prompting for the username, password, and role.
 *    The role is selected by inputting a character (e.g., 'D' for donor or 'R' for recipient).
 * Behavior:
 *    - Validates the input for each field.
 *    - Calls save_user() to store the user details in the persistent storage.
 *    - Provides appropriate success or error messages.
 */
void signup();

/*
 * login(char *logged_in_user, char *logged_in_role)
 * --------------------------------------------------
 * Purpose:
 *    Authenticates a user by verifying the provided username and password against stored credentials.
 * Behavior:
 *    - Prompts the user to enter their username and password.
 *    - Uses the validate_credentials() function to check if they match an existing user record.
 *    - On success, it copies the username and role into the provided parameters and returns 1.
 *    - Returns 0 if the credentials are invalid.
 *
 * Parameters:
 *    logged_in_user: A string in which the authenticated username will be stored.
 *    logged_in_role: A string in which the role of the authenticated user ("donor" or "recipient") will be stored.
 */
int login(char *logged_in_user, char *logged_in_role);

/*
 * load_users(User users[], int *user_count)
 * -----------------------------------------
 * Purpose:
 *    Loads all user records from the persistent storage file into an array of User structures.
 * Behavior:
 *    - Opens the users file and reads each record.
 *    - Stores the records in the provided array and updates the user_count to reflect the number of users loaded.
 *    - If the file does not exist or is empty, an appropriate message is displayed.
 *
 * Parameters:
 *    users:      An array of User structures that will be populated with the user data.
 *    user_count: A pointer to an integer that will hold the number of users loaded from the file.
 */
void load_users(User users[], int *user_count);

/*
 * save_user(User newUser)
 * -----------------------
 * Purpose:
 *    Saves a new user's details to the persistent storage (users file) in CSV format.
 * Behavior:
 *    - Opens the file in append mode.
 *    - Writes the new user's username, password, and role.
 *    - Closes the file.
 *
 * Parameters:
 *    newUser: A User structure containing the new user's information.
 */
void save_user(User newUser);

/*
 * validate_credentials(char *username, char *password, char *role)
 * -----------------------------------------------------------------
 * Purpose:
 *    Checks if the provided username and password match any of the stored user records.
 * Behavior:
 *    - Opens the users file and reads each record.
 *    - Compares the provided credentials with each stored user.
 *    - On a successful match, the corresponding role is copied into the provided role string, 
 *      and the function returns 1.
 *    - Returns 0 if no match is found.
 *
 * Parameters:
 *    username: The username entered by the user attempting to log in.
 *    password: The password entered by the user.
 *    role:     A string that will be filled with the user's role if the credentials are valid.
 *
 * Returns:
 *    1 if the credentials are valid (login successful), 0 otherwise.
 */
int validate_credentials(char *username, char *password, char *role);

#endif /* USER_H */
