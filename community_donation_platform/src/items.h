#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * MAX_ITEMS defines the maximum number of items that can be stored.
 * MAX_DESC defines the maximum length of an item's description.
 */
#define MAX_ITEMS 100
#define MAX_DESC 100

/*
 * ITEM_FILE_PATH is the relative file path to the text file that stores the items.
 * The file is expected to have a header line followed by item records.
 */
#define ITEM_FILE_PATH "../data/items.txt"

/*
 * Structure: Item
 * ----------------
 * Represents an item available for donation.
 *
 * Fields:
 *   item_id        - A unique integer identifier for the item.
 *   donor_username - A string (max 20 characters plus the null terminator) representing
 *                    the username of the donor who is donating the item.
 *   category       - The category of the item (e.g., "Books", "Electronics").
 *   description    - A brief text description of the item.
 *   condition      - The condition of the item (expected values: "New", "Good", "Fair").
 *   status         - The current status of the item (e.g., "available", "donated").
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
 *
 * add_item(): 
 *   Prompts the user for details of the item and writes the new item to the items file.
 *
 * display_items():
 *   Reads the items file and prints a table of items whose status is "available".
 *
 * search_items():
 *   Lets the user search for items by category. This function calls get_category_selection()
 *   to display a numbered list of categories and then performs a case-insensitive search.
 *
 * get_category_selection(char selected_category[]):
 *   Reads the items file, compiles a list of distinct categories from the "available" items,
 *   displays them with numbers, and prompts the user to select one. The selected category is
 *   returned via the selected_category parameter.
 *
 * to_lowercase(char *str):
 *   Converts the provided string to lowercase; useful for case-insensitive comparisons.
 *
 * update_status(int item_id, char *new_status):
 *   Updates the status of an item specified by item_id in the items file.
 */
void add_item();
void display_items();
void search_items();
int get_category_selection(char selected_category[]);
void to_lowercase(char *str);
void update_status(int item_id, char *new_status);

#endif /* ITEMS_H */
