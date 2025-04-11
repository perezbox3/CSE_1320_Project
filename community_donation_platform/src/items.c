/*
 * items.c
 *
 * Handles everything related to donation items in our system.
 * Donors can add new items, everyone can view what's available,
 * and users can search items by categories. It stores everything
 * in a simple text file called "items.txt".
 */

#include "items.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Clears any leftover characters from the input buffer.
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Converts a string to lowercase for easy comparisons later.
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Adds a new donation item to the system.
void add_item() {
    FILE *file = fopen(ITEM_FILE_PATH, "a+");
    if (!file) {
        printf("Error: Could not open items file.\n");
        return;
    }

    Item newItem;

    // Get the donor's username.
    printf("Enter your username: ");
    if (scanf("%20s", newItem.donor_username) != 1) {
        printf("Invalid username input.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    // Get the category of the item.
    printf("Enter category (e.g., Clothes, Furniture, Electronics): ");
    if (!fgets(newItem.category, sizeof(newItem.category), stdin)) {
        printf("Invalid category input.\n");
        fclose(file);
        return;
    }
    newItem.category[strcspn(newItem.category, "\n")] = '\0';

    // Get a description of the item.
    printf("Enter description: ");
    if (!fgets(newItem.description, MAX_DESC, stdin)) {
        printf("Invalid description input.\n");
        fclose(file);
        return;
    }
    newItem.description[strcspn(newItem.description, "\n")] = '\0';

    // Get the condition of the item (New/Good/Fair).
    printf("Enter condition (New/Good/Fair): ");
    if (scanf("%20s", newItem.condition) != 1) {
        printf("Invalid condition input.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    // Set status as "available" by default.
    strcpy(newItem.status, "available");

    // Find the next item ID by checking the file.
    int last_id = 0;
    FILE *readFile = fopen(ITEM_FILE_PATH, "r");
    if (readFile) {
        char header[100];
        fgets(header, sizeof(header), readFile); // Skip header
        Item temp;
        while (fscanf(readFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category, temp.description,
                      temp.condition, temp.status) != EOF) {
            last_id = temp.item_id;
        }
        fclose(readFile);
    }
    newItem.item_id = last_id + 1;

    // Save the new item to the file.
    fprintf(file, "%d,%s,%s,%s,%s,%s\n",
            newItem.item_id, newItem.donor_username, newItem.category,
            newItem.description, newItem.condition, newItem.status);
    fclose(file);

    printf("Item successfully added!\n");
}

// Shows all available items.
void display_items() {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    char header[100];
    fgets(header, sizeof(header), file); // Skip header
    Item temp;
    int found = 0;

    printf("\nAvailable Items:\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("ID | Donor       | Category     | Description                   | Condition\n");
    printf("---------------------------------------------------------------------------------\n");

    // Display each available item from the file.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s| %-13s| %-30s| %-9s\n",
                   temp.item_id, temp.donor_username, temp.category,
                   temp.description, temp.condition);
            found = 1;
        }
    }

    if (!found) {
        printf("No items available.\n");
    }

    fclose(file);
}

// Lists categories with numbers so user can easily pick one.
int get_category_selection(char selected_category[]) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return 0;
    }

    char header[100];
    fgets(header, sizeof(header), file); // Skip header
    char categories[MAX_ITEMS][21];
    int count = 0;
    Item temp;

    // Collect unique categories from items marked as available.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            int exists = 0;
            for (int i = 0; i < count; i++) {
                if (strcasecmp(categories[i], temp.category) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists && count < MAX_ITEMS) {
                strcpy(categories[count++], temp.category);
            }
        }
    }
    fclose(file);

    if (count == 0) {
        printf("No categories found.\n");
        return 0;
    }

    // Display the categories and let the user pick one.
    printf("\nCategories:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    printf("Choose a category number: ");
    int selection;
    if (scanf("%d", &selection) != 1 || selection < 1 || selection > count) {
        printf("Invalid selection.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    strcpy(selected_category, categories[selection - 1]);
    return 1;
}

// Lets the user search for available items by choosing a category.
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
    fgets(header, sizeof(header), file); // Skip header
    Item temp;
    int found = 0;

    printf("\nSearch Results:\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("ID | Donor       | Category     | Description                   | Condition\n");
    printf("---------------------------------------------------------------------------------\n");

    // Show items matching the chosen category.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        char lowerCat[21];
        strcpy(lowerCat, temp.category);
        to_lowercase(lowerCat);
        if (strcmp(lowerCat, search_category) == 0 && strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s| %-13s| %-30s| %-9s\n",
                   temp.item_id, temp.donor_username, temp.category,
                   temp.description, temp.condition);
            found = 1;
        }
    }

    if (!found) {
        printf("No items found in this category.\n");
    }

    fclose(file);
}

// Updates the status of an item, like marking it as "donated".
void update_status(int item_id, char *new_status) {
    // (Implementation unchanged; see earlier code provided)
}
