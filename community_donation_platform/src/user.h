#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define user structure
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "donor" or "recipient"
} User;

// Function declarations
void signup();
int login(char *logged_in_user, char *logged_in_role);
void load_users(User users[], int *user_count);
void save_user(User newUser);
int validate_credentials(char *username, char *password, char *role);

#endif
