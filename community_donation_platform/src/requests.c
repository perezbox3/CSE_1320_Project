#include "requests.h"
#include "items.h"

// Function to request an item
void request_item(char *recipient_username) {
    // First check if the item exists and is available
    int item_id;
    printf("Enter the ID of the item you want to request: ");
    scanf("%d", &item_id);

    // Verify item exists and is available
    FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
    if (!itemFile) {
        printf("Error: No items available.\n");
        return;
    }

    Item temp_item;
    int item_found = 0;
    while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                  &temp_item.item_id, temp_item.donor_username, temp_item.category,
                  temp_item.description, temp_item.condition, temp_item.status) != EOF) {
        if (temp_item.item_id == item_id && strcmp(temp_item.status, "available") == 0) {
            item_found = 1;
            break;
        }
    }
    fclose(itemFile);

    if (!item_found) {
        printf("Error: Item not found or not available.\n");
        return;
    }

    FILE *file = fopen(REQUEST_FILE_PATH, "a");
    if (!file) {
        printf("Error: Unable to open requests.txt for writing.\n");
        return;
    }

    // Determine request ID (increment based on file content)
    int last_id = 0;
    FILE *readFile = fopen(REQUEST_FILE_PATH, "r");
    if (readFile) {
        Request temp;
        while (fscanf(readFile, "%d,%d,%20[^,],%20[^\n]\n",
                      &temp.request_id, &temp.item_id, temp.recipient_username, temp.status) != EOF) {
            last_id = temp.request_id;
        }
        fclose(readFile);
    }

    int request_id = last_id + 1;
    fprintf(file, "%d,%d,%s,pending\n", request_id, item_id, recipient_username);
    fclose(file);

    printf("Request successfully submitted!\n");
}

// Function to approve or reject a request
void approve_request(char *donor_username) {
    int request_id;
    char decision[10];
    printf("Enter the ID of the request to approve/reject: ");
    scanf("%d", &request_id);
    printf("Enter decision (approve/reject): ");
    scanf("%s", decision);

    FILE *file = fopen(REQUEST_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open requests.txt for reading.\n");
        return;
    }

    FILE *tempFile = fopen("../data/temp_requests.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    Request temp;
    int found = 0;
    while (fscanf(file, "%d,%d,%20[^,],%20[^\n]\n",
                  &temp.request_id, &temp.item_id, temp.recipient_username, temp.status) != EOF) {
        if (temp.request_id == request_id) {
            found = 1;
            if (strcmp(decision, "approve") == 0) {
                strcpy(temp.status, "approved");
                update_status(temp.item_id, "donated");
            } else if (strcmp(decision, "reject") == 0) {
                strcpy(temp.status, "rejected");
            } else {
                printf("Invalid decision. Request not updated.\n");
                fprintf(tempFile, "%d,%d,%s,%s\n", temp.request_id, temp.item_id, temp.recipient_username, temp.status);
                continue;
            }
        }
        fprintf(tempFile, "%d,%d,%s,%s\n", temp.request_id, temp.item_id, temp.recipient_username, temp.status);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Request ID not found.\n");
        remove("../data/temp_requests.txt");
    } else {
        remove(REQUEST_FILE_PATH);
        rename("../data/temp_requests.txt", REQUEST_FILE_PATH);
        printf("Request successfully updated.\n");
    }
}

// Function to update the status of an item
void update_status(int item_id, char *new_status) {
    FILE *file = fopen(ITEM_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open items.txt for reading.\n");
        return;
    }

    FILE *tempFile = fopen("../data/temp_items.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

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
    rename("../data/temp_items.txt", ITEM_FILE_PATH);
}
