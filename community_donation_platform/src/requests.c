// requests.c
// This file manages donation requests between donors and recipients.
// Recipients can request items, donors can approve or reject requests,
// and both users can check their notifications and inventory.

#include "requests.h"   // Include the request-related structures and functions
#include "items.h"      // Include item-related structures and functions
#include <ctype.h>      // For converting strings to lowercase
#include <string.h>     // For string operations like strcmp and strcpy
#include <stdio.h>      // For standard input/output (printf, scanf)
#include <stdlib.h>     // General purpose functions like exit()

// Clear the input buffer (to avoid leftover input from scanf causing problems)
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Convert a string to lowercase for easy comparison
static void local_to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Allow a recipient to request an available item
void request_item(char *recipient_username) {
    display_items(); // Show available items first

    // Check if there are available items at all
    FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
    int available = 0;
    if (itemFile) {
        char header[100];
        fgets(header, sizeof(header), itemFile); // Skip header
        Item temp;
        while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                      &temp.item_id, temp.donor_username, temp.category,
                      temp.description, temp.condition, temp.status) != EOF) {
            if (strcmp(temp.status, "available") == 0) {
                available = 1;
                break;
            }
        }
        fclose(itemFile);
    }
    if (!available) {
        printf("No items are available to request right now.\n");
        return;
    }

    // Ask recipient for item ID to request
    int item_id;
    printf("Enter the ID of the item you want: ");
    if (scanf("%d", &item_id) != 1) {
        printf("Invalid item ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Confirm the item is available
    itemFile = fopen(ITEM_FILE_PATH, "r");
    if (!itemFile) {
        printf("Couldn't open items file.\n");
        return;
    }
    char itemHeader[100];
    fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header
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
        printf("This item isn't available.\n");
        return;
    }

    // Assign a new request ID and add the request to the file
    FILE *file = fopen(REQUEST_FILE_PATH, "a");
    if (!file) {
        printf("Couldn't open requests file.\n");
        return;
    }
    int last_id = 0;
    FILE *readFile = fopen(REQUEST_FILE_PATH, "r");
    if (readFile) {
        char reqHeader[100];
        fgets(reqHeader, sizeof(reqHeader), readFile); // Skip header
        Request tempReq;
        while (fscanf(readFile, "%d,%d,%20[^,],%20[^\n]\n",
                      &tempReq.request_id, &tempReq.item_id, tempReq.recipient_username, tempReq.status) != EOF) {
            last_id = tempReq.request_id;
        }
        fclose(readFile);
    }
    fprintf(file, "%d,%d,%s,pending\n", last_id + 1, item_id, recipient_username);
    fclose(file);
    printf("Your request has been submitted!\n");
}

// Donors use this to approve or reject item requests
void approve_request(char *donor_username) {
    // Check if there are requests to handle
    if (count_pending_requests(donor_username) == 0) {
        printf("No pending requests.\n");
        return;
    }

    view_inbox(donor_username); // Show requests waiting for approval

    // Ask for request ID and decision
    int request_id;
    char decision[10];
    printf("Enter request ID: ");
    if (scanf("%d", &request_id) != 1) {
        printf("Invalid ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    printf("Approve or reject? (type 'approve' or 'reject'): ");
    if (scanf("%9s", decision) != 1) {
        printf("Invalid choice.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    local_to_lowercase(decision);

    // Update request status in file
    FILE *file = fopen(REQUEST_FILE_PATH, "r");
    FILE *tempFile = fopen("../data/temp_requests.txt", "w");
    if (!file || !tempFile) {
        printf("Couldn't update requests.\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }
    char reqHeader[100];
    fgets(reqHeader, sizeof(reqHeader), file); // Skip header
    fprintf(tempFile, "request_id,item_id,recipient_username,status\n");
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
                printf("Invalid decision entered.\n");
            }
        }
        fprintf(tempFile, "%d,%d,%s,%s\n", temp.request_id, temp.item_id,
                temp.recipient_username, temp.status);
    }
    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Request not found.\n");
        remove("../data/temp_requests.txt");
    } else {
        remove(REQUEST_FILE_PATH);
        rename("../data/temp_requests.txt", REQUEST_FILE_PATH);
        printf("Request updated successfully.\n");
    }
}

// Shows pending requests to donors
void view_inbox(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No pending requests.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header
    Request req;
    int found = 0;
    printf("\nPending Requests:\n");
    printf("----------------------------------\n");
    printf("ReqID | ItemID | Recipient\n");
    printf("----------------------------------\n");
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0 && item_belongs_to_donor(req.item_id, donor_username)) {
            printf("%-6d| %-7d| %-20s\n", req.request_id, req.item_id, req.recipient_username);
            found = 1;
        }
    }
    fclose(reqFile);
    if (!found) {
        printf("No pending requests.\n");
    }
}

// Counts the number of pending requests for a donor
int count_pending_requests(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        return 0;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header
    Request req;
    int count = 0;
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0 && item_belongs_to_donor(req.item_id, donor_username)) {
            count++;
        }
    }
    fclose(reqFile);
    return count;
}

// Shows the approved items (inventory) for recipients
void view_inventory(char *recipient_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No inventory yet.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header
    Request req;
    int found = 0;
    printf("\nYour Inventory:\n");
    printf("-----------------------------------------------\n");
    printf("ReqID | ItemID | Category       | Description\n");
    printf("-----------------------------------------------\n");
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "approved") == 0 && strcmp(req.recipient_username, recipient_username) == 0) {
            Item item;
            if (get_item_by_id(req.item_id, &item)) {
                printf("%-6d| %-7d| %-15s| %s\n", req.request_id, item.item_id, item.category, item.description);
                found = 1;
            }
        }
    }
    fclose(reqFile);
    if (!found) {
        printf("Your inventory is empty.\n");
    }
}
