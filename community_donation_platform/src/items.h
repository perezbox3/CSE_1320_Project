// This file sets up our "Item" structure and function prototypes for a donation system.
// We define the maximum sizes for data, and we have a path to the "items.txt" file.
// There's also a struct that describes an item and some function declarations to use it.

#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// We can store up to 100 items and have descriptions up to 100 characters
#define MAX_ITEMS 100
#define MAX_DESC 100

// This is where our items get saved and read
#define ITEM_FILE_PATH "../data/items.txt"

// Structure for donation items
typedef struct {
    int item_id;               // Unique ID for the item
    char donor_username[21];   // Username of the donor
    char category[21];         // Item category like "Books", "Electronics", etc.
    char description[MAX_DESC]; // A short text describing the item
    char condition[21];        // Like "New", "Good", "Fair"
    char status[21];           // "available", "donated", etc.
} Item;

// Below are the functions we use in our program:

// Lets user add a new item (asks for info, then saves it to items file)
void add_item();

// Shows all items that are marked as "available"
void display_items();

// Lets user pick a category and shows items matching that category
void search_items();

// Asks user to pick from a list of categories (returns selected one)
int get_category_selection(char selected_category[]);

// Makes a string lowercase for case-insensitive matching
void to_lowercase(char *str);

// Changes the status of an item (like from "available" to "donated")
void update_status(int item_id, char *new_status);

#endif /* ITEMS_H */
