// user.h
// This file describes how we handle user accounts. We have a structure called "User"
// to store each user's info (username, password, and role), and some functions to
// sign up, log in, and manage user data in a file.

#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Holds a single user's data
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "donor" or "recipient"
} User;

// Lets a new user register by giving a username, password, and role
void signup();

// Logs a user in by verifying their username and password
// Returns 1 if valid, 0 otherwise
int login(char *logged_in_user, char *logged_in_role);

// Loads all users from the users.txt file into an array
void load_users(User users[], int *user_count);

// Saves a new user to the users.txt file
void save_user(User newUser);

// Checks if username and password match a record in the file
// Copies the role into the provided role variable if it matches
int validate_credentials(char *username, char *password, char *role);

#endif /* USER_H */
