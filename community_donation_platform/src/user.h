#ifndef USER_H
#define USER_H

// user.h
// ------------
// This header file has all the definitions and declarations needed for user management
// on the Community Donation Platform.
//
// It includes:
// - The User structure (which holds usernames, passwords, and roles).
// - Functions for signing up, logging in, and handling user data.

#include <stdio.h>    // For standard input and output (printf, scanf)
#include <stdlib.h>   // For standard utilities (like file handling and memory management)
#include <string.h>   // For string operations (like strcpy, strcmp)

// Structure for storing user information
typedef struct {
    char username[50];  // Username (up to 49 characters)
    char password[50];  // Password (up to 49 characters)
    char role[10];      // User role ("donor" or "recipient")
} User;

// Lets a new user sign up by entering their username, password, and choosing their role.
void signup();

// Checks if the entered username and password are correct. If correct, stores the user's name and role.
int login(char *logged_in_user, char *logged_in_role);

// Loads user data from the file into an array. Also sets the total number of users.
void load_users(User users[], int *user_count);

// Saves a newly registered user's information into the file.
void save_user(User newUser);

// Checks user credentials against stored data and returns the role if they match.
int validate_credentials(char *username, char *password, char *role);

#endif /* USER_H */
