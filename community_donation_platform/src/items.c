// This program is a simple donation management system using a file called "items.txt".
// We can add items to the file, show what's available, search them by category, and update their status.
// There are also helper functions to handle input and convert strings to lowercase.

#include "items.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This function just clears any leftover characters in stdin
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Makes all letters in a string lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Lets you add a new item to the items file by asking for info from the user
void add_item() {
    FILE *file = fopen(ITEM_FILE_PATH, "a+");  // Changed to a+
    if (!file) {
        printf("Error: Unable to open items.txt for writing.\n");
        return;
    }

    // If the file is empty, write a header first
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    
    if (fileSize == 0) {
        fprintf(file, "item_id,donor_username,category,description,condition,status\n");
    } else if (fileSize > 0) {
        // Check if the last character is a newline
        fseek(file, -1, SEEK_END);
        char lastChar = fgetc(file);
        if (lastChar != '\n') {
            fprintf(file, "\n");  // Add a newline if needed
        }
        fseek(file, 0, SEEK_END);  // Move back to end for appending
    }

    Item newItem;

    // Ask for username
    printf("Enter your username: ");
    if (scanf("%20s", newItem.donor_username) != 1) {
        printf("Invalid input for username.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    // Ask for category (we use fgets to include spaces)
    printf("Enter category (e.g., Clothes, Furniture, Electronics, Books, etc.): ");
    if (fgets(newItem.category, sizeof(newItem.category), stdin) == NULL) {
        printf("Error reading category.\n");
        fclose(file);
        return;
    }
    newItem.category[strcspn(newItem.category, "\n")] = '\0';

    // Ask for the description
    printf("Enter description: ");
    if (fgets(newItem.description, MAX_DESC, stdin) == NULL) {
        printf("Error reading description.\n");
        fclose(file);
        return;
    }
    newItem.description[strcspn(newItem.description, "\n")] = '\0';

    // Ask for the condition of the item
    printf("Enter condition (New/Good/Fair): ");
    if (scanf("%20s", newItem.condition) != 1) {
        printf("Invalid input for condition.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    // By default, new items are available
    strcpy(newItem.status, "available");

    // Figure out the next item_id by reading everything and finding the highest existing
    int last_id = 0;
    FILE *readFile = fopen(ITEM_FILE_PATH, "r");
    if (readFile) {
        char header[200];
        fgets(header, sizeof(header), readFile); // skip the header
        Item temp;
        while (fscanf(readFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category, temp.description,
                      temp.condition, temp.status) != EOF) {
            if (temp.item_id > last_id) {
                last_id = temp.item_id;
            }
        }
        fclose(readFile);
    }
    newItem.item_id = last_id + 1;

    // Write the new item record to the file
    fprintf(file, "%d,%s,%s,%s,%s,%s\n",
            newItem.item_id, newItem.donor_username, newItem.category,
            newItem.description, newItem.condition, newItem.status);
    fclose(file);

    printf("Item successfully added!\n");
}

// Displays all items that are currently available
void display_items() {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    // Skip the header line
    char header[200];
    if (!fgets(header, sizeof(header), file)) {
        // If the file is empty or unreadable
        fclose(file);
        printf("No items available.\n");
        return;
    }

    Item temp;
    int found = 0;
    printf("\nAvailable Items:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("ID | Donor        | Category     | Description                           | Condition | Status\n");
    printf("--------------------------------------------------------------------------------\n");

    // Print only items with status = "available"
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s| %-12s| %-36s| %-10s| %-10s\n",
                   temp.item_id, temp.donor_username, temp.category, temp.description,
                   temp.condition, temp.status);
            found = 1;
        }
    }
    fclose(file);
    if (!found) {
        printf("No items available.\n");
    }
}

// Shows a list of distinct categories for the user to choose from, then returns it
int get_category_selection(char selected_category[]) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return 0;
    }

    // Skip the header
    char header[200];
    if (!fgets(header, sizeof(header), file)) {
        fclose(file);
        printf("No items available.\n");
        return 0;
    }

    char categories[MAX_ITEMS][21];
    int count = 0;
    Item temp;

    // Collect unique categories from available items
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category,
                  temp.description, temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            int exists = 0;
            char lowerCat[21];
            strcpy(lowerCat, temp.category);
            to_lowercase(lowerCat);

            // Check if it's already in the list
            for (int i = 0; i < count; i++) {
                char lowerStored[21];
                strcpy(lowerStored, categories[i]);
                to_lowercase(lowerStored);
                if (strcmp(lowerStored, lowerCat) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists && count < MAX_ITEMS) {
                strcpy(categories[count], temp.category);
                count++;
            }
        }
    }
    fclose(file);
    
    if (count == 0) {
        printf("No available categories found.\n");
        return 0;
    }
    
    // Print out the categories and let user pick
    printf("\nAvailable Categories:\n");
    for (int i = 0; i < count; i++) {
        printf("  %d. %s\n", i + 1, categories[i]);
    }
    printf("Enter the number corresponding to the desired category: ");
    int selection;
    if (scanf("%d", &selection) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    if (selection < 1 || selection > count) {
        printf("Invalid selection.\n");
        return 0;
    }

    // Copy chosen category
    strcpy(selected_category, categories[selection - 1]);
    return 1;
}

// Lets the user look for items in a certain category (case-insensitive)
void search_items() {
    char search_category[21];
    if (!get_category_selection(search_category)) {
        return;
    }
    to_lowercase(search_category);

    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    // Skip the header
    char header[200];
    if (!fgets(header, sizeof(header), file)) {
        fclose(file);
        printf("No items available.\n");
        return;
    }

    Item temp;
    int found = 0;

    printf("\nSearch Results:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("ID | Donor        | Category     | Description                           | Condition | Status\n");
    printf("--------------------------------------------------------------------------------\n");

    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        char lowerCat[21];
        strcpy(lowerCat, temp.category);
        to_lowercase(lowerCat);

        // Must match category and be available
        if (strcmp(lowerCat, search_category) == 0 && strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s| %-12s| %-36s| %-10s| %-10s\n",
                   temp.item_id, temp.donor_username, temp.category, temp.description,
                   temp.condition, temp.status);
            found = 1;
        }
    }
    fclose(file);
    if (!found) {
        printf("No items found in this category.\n");
    }
}

// Changes an item's status if we find the matching item_id
void update_status(int item_id, char *new_status) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open items.txt for reading.\n");
        return;
    }

    // Skip the header
    char header[200];
    if (!fgets(header, sizeof(header), file)) {
        fclose(file);
        printf("Error: items file is empty or corrupted.\n");
        return;
    }

    // Create a temp file to rewrite data
    FILE *tempFile = fopen("../data/temp_items.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    // Write header to the temp file
    fprintf(tempFile, "item_id,donor_username,category,description,condition,status\n");

    Item temp;
    // Go through the items and update status for matching ID
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category,
                  temp.description, temp.condition, temp.status) != EOF) {
        if (temp.item_id == item_id) {
            strcpy(temp.status, new_status);
        }
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s\n",
                temp.item_id, temp.donor_username, temp.category,
                temp.description, temp.condition, temp.status);
    }
    fclose(file);
    fclose(tempFile);

    // Replace the old file with the new one
    remove(ITEM_FILE_PATH);
    if (rename("../data/temp_items.txt", ITEM_FILE_PATH) != 0) {
        printf("Error: Unable to update items file.\n");
    }
}
