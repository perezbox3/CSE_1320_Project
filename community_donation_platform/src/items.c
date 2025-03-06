#include "items.h"
#include <ctype.h> // For case-insensitive search

// Function to convert a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to add an item for donation
void add_item() {
    // Ensure items.txt exists before writing
    FILE *file = fopen(ITEM_FILE_PATH, "a+");
    if (!file) {
        printf("Error: Unable to open items.txt for writing.\n");
        return;
    }

    Item newItem;
    printf("Enter your username: ");
    scanf("%s", newItem.donor_username);

    getchar(); // Clear leftover newline

    printf("Enter category (e.g., Clothes, Furniture): ");
    fgets(newItem.category, 21, stdin);
    newItem.category[strcspn(newItem.category, "\n")] = '\0'; // Remove newline

    printf("Enter description: ");
    fgets(newItem.description, MAX_DESC, stdin);
    newItem.description[strcspn(newItem.description, "\n")] = '\0'; // Remove newline

    printf("Enter condition (New/Good/Fair): ");
    scanf("%s", newItem.condition);

    strcpy(newItem.status, "available"); // Default status

    // Determine item ID (increment based on file content)
    int last_id = 0;
    FILE *readFile = fopen(ITEM_FILE_PATH, "r");
    if (readFile) {
        Item temp;
        while (fscanf(readFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category, temp.description,
                      temp.condition, temp.status) != EOF) {
            last_id = temp.item_id;
        }
        fclose(readFile);
    }
    newItem.item_id = last_id + 1;

    // Write to file
    fprintf(file, "%d,%s,%s,%s,%s,%s\n",
            newItem.item_id, newItem.donor_username, newItem.category,
            newItem.description, newItem.condition, newItem.status);

    fclose(file);
    printf("Item successfully added!\n");
}

// Function to display all available items
void display_items() {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    Item temp;
    printf("\nAvailable Items:\n");
    printf("------------------------------------------------------------\n");
    printf("ID | Donor        | Category  | Condition | Status    \n");
    printf("------------------------------------------------------------\n");

    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s | %-9s | %-9s | %-9s\n",
                   temp.item_id, temp.donor_username, temp.category, temp.condition, temp.status);
        }
    }

    fclose(file);
}

// Function to search for items based on category
void search_items() {
    char search_category[21];
    printf("Enter category to search: ");
    scanf("%s", search_category);
    to_lowercase(search_category); // Convert input to lowercase

    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    Item temp;
    int found = 0;
    printf("\nSearch Results:\n");
    printf("------------------------------------------------------------\n");
    printf("ID | Donor        | Category  | Condition | Status    \n");
    printf("------------------------------------------------------------\n");

    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        char temp_category[21];
        strcpy(temp_category, temp.category);
        to_lowercase(temp_category);

        if (strcmp(temp_category, search_category) == 0 && strcmp(temp.status, "available") == 0) {
            printf("%-3d| %-12s | %-9s | %-9s | %-9s\n",
                   temp.item_id, temp.donor_username, temp.category, temp.condition, temp.status);
            found = 1;
        }
    }

    if (!found) {
        printf("No items found in this category.\n");
    }

    fclose(file);
}
