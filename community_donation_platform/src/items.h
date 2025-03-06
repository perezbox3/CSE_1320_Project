#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_ITEMS 100
#define MAX_DESC 100
#define ITEM_FILE_PATH "../data/items.txt"

// Item structure
typedef struct {
    int item_id;
    char donor_username[21];
    char category[21];
    char description[MAX_DESC];
    char condition[21]; // "New", "Good", "Fair"
    char status[21]; // "available" or "donated"
} Item;

// Function Prototypes
void add_item();
void display_items();
void search_items();

#endif
