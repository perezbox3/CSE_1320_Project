#include "items.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------
// Helper function to clear input buffer
//------------------------------------
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//------------------------------------
// Converts a string to lowercase.
//------------------------------------
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//------------------------------------
// Adds an item for donation.
//------------------------------------
void add_item() {
    FILE *file = fopen(ITEM_FILE_PATH, "a+");
    if (!file) {
        printf("Error: Unable to open items.txt for writing.\n");
        return;
    }

    Item newItem;
    printf("Enter your username: ");
    if (scanf("%20s", newItem.donor_username) != 1) {
        printf("Invalid input for username.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    printf("Enter category (e.g., Clothes, Furniture, Electronics, Books, etc.): ");
    if (fgets(newItem.category, sizeof(newItem.category), stdin) == NULL) {
        printf("Error reading category.\n");
        fclose(file);
        return;
    }
    newItem.category[strcspn(newItem.category, "\n")] = '\0';

    printf("Enter description: ");
    if (fgets(newItem.description, MAX_DESC, stdin) == NULL) {
        printf("Error reading description.\n");
        fclose(file);
        return;
    }
    newItem.description[strcspn(newItem.description, "\n")] = '\0';

    printf("Enter condition (New/Good/Fair): ");
    if (scanf("%20s", newItem.condition) != 1) {
        printf("Invalid input for condition.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    strcpy(newItem.status, "available");

    // Determine next item_id by scanning the file (skip header).
    int last_id = 0;
    FILE *readFile = fopen(ITEM_FILE_PATH, "r");
    if (readFile) {
        char header[100];
        fgets(header, sizeof(header), readFile); // Skip header line.
        Item temp;
        while (fscanf(readFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category, temp.description,
                      temp.condition, temp.status) != EOF) {
            last_id = temp.item_id;
        }
        fclose(readFile);
    }
    newItem.item_id = last_id + 1;

    // Write the new item.
    fprintf(file, "%d,%s,%s,%s,%s,%s\n",
            newItem.item_id, newItem.donor_username, newItem.category,
            newItem.description, newItem.condition, newItem.status);
    fclose(file);

    printf("Item successfully added!\n");
}

//------------------------------------
// Displays all available items.
//------------------------------------
void display_items() {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), file); // Skip header.
    Item temp;
    int found = 0;
    printf("\nAvailable Items:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("ID | Donor        | Category     | Description                           | Condition | Status\n");
    printf("--------------------------------------------------------------------------------\n");
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

//------------------------------------
// Presents a numbered list of available categories and returns the selected category.
//------------------------------------
int get_category_selection(char selected_category[]) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return 0;
    }
    char header[100];
    fgets(header, sizeof(header), file); // Skip header.
    char categories[MAX_ITEMS][21];
    int count = 0;
    Item temp;
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            int exists = 0;
            char lowerCat[21];
            strcpy(lowerCat, temp.category);
            to_lowercase(lowerCat);
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
    strcpy(selected_category, categories[selection - 1]);
    return 1;
}

//------------------------------------
// Searches for items based on the selected category.
//------------------------------------
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
    char header[100];
    fgets(header, sizeof(header), file); // Skip header.
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

//------------------------------------
// Updates the status of an item (e.g., marking it as "donated").
// This function is defined only here (in items.c).
//------------------------------------
void update_status(int item_id, char *new_status) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open items.txt for reading.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), file); // Skip header.
    FILE *tempFile = fopen("../data/temp_items.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    // Write header line to temp file.
    fprintf(tempFile, "item_id,donor_username,category,description,condition,status\n");
    Item temp;
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (temp.item_id == item_id) {
            strcpy(temp.status, new_status);
        }
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s\n",
                temp.item_id, temp.donor_username, temp.category, temp.description,
                temp.condition, temp.status);
    }
    fclose(file);
    fclose(tempFile);
    remove(ITEM_FILE_PATH);
    if (rename("../data/temp_items.txt", ITEM_FILE_PATH) != 0) {
        printf("Error: Unable to update items file.\n");
    }
}
