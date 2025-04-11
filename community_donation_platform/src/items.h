/*
 * items.h
 *
 * This header file defines everything related to donation items for our
 * Community Donation Platform. It includes constants, the Item structure,
 * and prototypes for all the functions we use to add, display, and search items.
 */

#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of items the system can handle
#define MAX_ITEMS 100

// Maximum length allowed for item descriptions
#define MAX_DESC 100

// Path to the file where we store item details
#define ITEM_FILE_PATH "../data/items.txt"

/*
 * Structure: Item
 *
 * Stores information about each item donated by users.
 *
 * item_id        - Unique ID number for the item.
 * donor_username - Username of the person donating the item.
 * category       - Type/category of the item (like "Books" or "Electronics").
 * description    - Short description about the item.
 * condition      - Condition of the item ("New", "Good", "Fair").
 * status         - Current availability status ("available" or "donated").
 */
typedef struct {
    int item_id;
    char donor_username[21];
    char category[21];
    char description[MAX_DESC];
    char condition[21];
    char status[21];
} Item;

/*
 * Function Prototypes:
 */

// Lets the user add a new donation item to the system
void add_item();

// Shows all donation items that are currently available
void display_items();

// Allows searching for items by category
void search_items();

// Displays categories as a numbered list and lets the user select one
int get_category_selection(char selected_category[]);

// Converts any text string to lowercase (for easy searches)
void to_lowercase(char *str);

// Changes the status of an item (for example, marking it as "donated")
void update_status(int item_id, char *new_status);

#endif /* ITEMS_H */
