#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number of items and maximum description length */
#define MAX_ITEMS 100
#define MAX_DESC 100

/* File path for the items file */
#define ITEM_FILE_PATH "../data/items.txt"

/* Structure representing a donation item */
typedef struct {
    int item_id;
    char donor_username[21];
    char category[21];
    char description[MAX_DESC];
    char condition[21];    // Expected values: "New", "Good", "Fair"
    char status[21];       // Expected values: "available" or "donated"
} Item;

/* Function prototypes for item operations */
void add_item();
void display_items();
void search_items();
/* New function for numeric category selection; returns 1 on success */
int get_category_selection(char selected_category[]);

/* Utility functions */
void to_lowercase(char *str);

/* Function to update an item's status; defined in items.c */
void update_status(int item_id, char *new_status);

#endif /* ITEMS_H */
