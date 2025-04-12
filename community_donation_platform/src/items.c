#include "items.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * clear_input_buffer()
 * --------------------
 * A helper function that flushes the standard input buffer.
 * It loops until it encounters a newline character or end of file.
 * This prevents leftover characters (like newline from scanf) from affecting subsequent input.
 */
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/*
 * to_lowercase(char *str)
 * -------------------------
 * Converts all characters in the input string to lowercase.
 *
 * Parameters:
 *   str - A pointer to the string to be converted.
 *
 * This function iterates over the string and uses the C library function tolower()
 * to change each character to its lowercase equivalent.
 */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/*
 * add_item()
 * -----------
 * Prompts the user to input details for a new donation item and writes this item to the items file.
 *
 * Process:
 *   1. Opens the items file (with "a+" mode to allow appending and reading).
 *   2. Prompts and reads the donor's username, item category, description, and condition.
 *   3. Sets the default status of the item to "available".
 *   4. Determines a new unique item_id by reading through the file (skipping the header).
 *   5. Appends the new item to the file.
 *   6. Closes the file and confirms the addition.
 *
 * Error Handling:
 *   If any input is invalid or if a file cannot be opened, the function prints an error message,
 *   flushes the input buffer when necessary, and safely returns.
 */
void add_item() {
    FILE *file = fopen(ITEM_FILE_PATH, "a+");
    if (!file) {
        printf("Error: Unable to open items.txt for writing.\n");
        return;
    }

    Item newItem;

    // Prompt and read the donor's username.
    printf("Enter your username: ");
    if (scanf("%20s", newItem.donor_username) != 1) {
        printf("Invalid input for username.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();  // Clear newline from the input

    // Prompt and read the category. Using fgets to allow spaces.
    printf("Enter category (e.g., Clothes, Furniture, Electronics, Books, etc.): ");
    if (fgets(newItem.category, sizeof(newItem.category), stdin) == NULL) {
        printf("Error reading category.\n");
        fclose(file);
        return;
    }
    // Remove trailing newline character, if any.
    newItem.category[strcspn(newItem.category, "\n")] = '\0';

    // Prompt and read the item's description.
    printf("Enter description: ");
    if (fgets(newItem.description, MAX_DESC, stdin) == NULL) {
        printf("Error reading description.\n");
        fclose(file);
        return;
    }
    newItem.description[strcspn(newItem.description, "\n")] = '\0';

    // Prompt and read the condition of the item.
    printf("Enter condition (New/Good/Fair): ");
    if (scanf("%20s", newItem.condition) != 1) {
        printf("Invalid input for condition.\n");
        clear_input_buffer();
        fclose(file);
        return;
    }
    clear_input_buffer();

    // Set the default status for a new item.
    strcpy(newItem.status, "available");

    // Determine a new unique item_id:
    // Open the items file for reading and skip the header.
    int last_id = 0;
    FILE *readFile = fopen(ITEM_FILE_PATH, "r");
    if (readFile) {
        char header[100];
        fgets(header, sizeof(header), readFile); // Skip header line
        Item temp;
        // Read through each record; update last_id with the highest found item_id.
        while (fscanf(readFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category, temp.description,
                      temp.condition, temp.status) != EOF) {
            last_id = temp.item_id;
        }
        fclose(readFile);
    }
    newItem.item_id = last_id + 1;

    // Append the new item record to the file.
    fprintf(file, "%d,%s,%s,%s,%s,%s\n",
            newItem.item_id, newItem.donor_username, newItem.category,
            newItem.description, newItem.condition, newItem.status);
    fclose(file);

    printf("Item successfully added!\n");
}

/*
 * display_items()
 * ----------------
 * Reads the items file and displays a nicely formatted table of all items that are "available" for donation.
 *
 * Process:
 *   1. Opens the items file for reading.
 *   2. Skips the header line.
 *   3. Iterates through each item record and prints only the records where status equals "available".
 *   4. If no available items are found, prints a message indicating the list is empty.
 */
void display_items() {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return;
    }

    char header[100];
    fgets(header, sizeof(header), file); // Skip the header line.
    Item temp;
    int found = 0;
    printf("\nAvailable Items:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("ID | Donor        | Category     | Description                           | Condition | Status\n");
    printf("--------------------------------------------------------------------------------\n");
    // Iterate through all item records.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        // Only display items marked as "available".
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

/*
 * get_category_selection(char selected_category[])
 * --------------------------------------------------
 * Reads all available items, compiles a list of distinct categories,
 * displays them with assigned numbers, and prompts the user to select one.
 *
 * Parameters:
 *   selected_category - a character array where the selected category name will be stored.
 *
 * Returns:
 *   1 if a valid category was selected; 0 otherwise.
 */
int get_category_selection(char selected_category[]) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("No items available.\n");
        return 0;
    }
    char header[100];
    fgets(header, sizeof(header), file); // Skip header.
    
    char categories[MAX_ITEMS][21]; // Array to hold distinct category names.
    int count = 0;
    Item temp;
    // Iterate through items to collect distinct categories.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        if (strcmp(temp.status, "available") == 0) {
            int exists = 0;
            char lowerCat[21];
            strcpy(lowerCat, temp.category);
            to_lowercase(lowerCat);
            // Check if category is already in the list.
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
    
    // Check if any categories were found.
    if (count == 0) {
        printf("No available categories found.\n");
        return 0;
    }
    
    // Display the distinct categories with assigned numbers.
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
    // Return the selected category.
    strcpy(selected_category, categories[selection - 1]);
    return 1;
}

/*
 * search_items()
 * ---------------
 * Allows the user to search for available items by category.
 * It calls get_category_selection() to display available categories with numbers,
 * then uses the selected category for a case-insensitive search in the items file.
 *
 * Process:
 *   1. Obtains the selected category using get_category_selection().
 *   2. Converts the category to lowercase.
 *   3. Opens the items file and skips the header.
 *   4. Iterates over the records and prints items whose category (in lowercase)
 *      matches the selected category and whose status is "available".
 */
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
    // Iterate through all items.
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        char lowerCat[21];
        strcpy(lowerCat, temp.category);
        to_lowercase(lowerCat);
        // Check for match in category and available status.
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

/*
 * update_status(int item_id, char *new_status)
 * --------------------------------------------
 * Updates the status of an item (e.g., changing it from "available" to "donated").
 *
 * Process:
 *   1. Opens the items file for reading and skips the header.
 *   2. Reads each record, and if the record's item_id matches the provided item_id,
 *      updates the status field to the new_status.
 *   3. Writes all records to a temporary file with the updated status.
 *   4. Replaces the original items file with the temporary file.
 */
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
    // Write the header to the temporary file.
    fprintf(tempFile, "item_id,donor_username,category,description,condition,status\n");
    Item temp;
    while (fscanf(file, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp.item_id, temp.donor_username, temp.category, temp.description,
                  temp.condition, temp.status) != EOF) {
        // If we find the record with the matching item_id, update its status.
        if (temp.item_id == item_id) {
            strcpy(temp.status, new_status);
        }
        // Write the (possibly updated) record to the temporary file.
        fprintf(tempFile, "%d,%s,%s,%s,%s,%s\n",
                temp.item_id, temp.donor_username, temp.category, temp.description,
                temp.condition, temp.status);
    }
    fclose(file);
    fclose(tempFile);
    // Replace the original file with the new file.
    remove(ITEM_FILE_PATH);
    if (rename("../data/temp_items.txt", ITEM_FILE_PATH) != 0) {
        printf("Error: Unable to update items file.\n");
    }
}
