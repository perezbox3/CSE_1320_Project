// This file handles item requests in our donation system. Recipients can request items, 
// and donors can approve or reject those requests. We also have helper 
// functions to view pending requests, count them, and look at a recipient's approved items.

#include "requests.h"   // For request functions and Request structure
#include "items.h"      // For Item structure and update_status function
#include <ctype.h>      // For tolower()
#include <string.h>     // For string operations
#include <stdio.h>      // For standard input/output
#include <stdlib.h>     // For general utilities
#include <sys/stat.h>   // For directory checking and creation

// Clears leftover chars in stdin
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Makes a string lowercase
static void local_to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Ensure the data directory exists
static void ensure_data_directory() {
    struct stat st = {0};
    if (stat("../data", &st) == -1) {
        #ifdef _WIN32
            mkdir("../data");
        #else
            mkdir("../data", 0700);
        #endif
    }
}

// Recipients can request an available item by ID
void request_item(char *recipient_username) {
    // Ensure data directory exists
    ensure_data_directory();
    
    // Show available items first
    display_items();

    // Check if at least one available item exists
    FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
    int available = 0;
    if (itemFile) {
        char itemHeader[100];
        fgets(itemHeader, sizeof(itemHeader), itemFile); // skip item file header
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
        printf("No available items to request.\n");
        return;
    }

    // Ask for the item ID to request
    int item_id;
    printf("Enter the ID of the item you want to request: ");
    if (scanf("%d", &item_id) != 1) {
        printf("Invalid input for item ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Ensure this item is actually available
    itemFile = fopen(ITEM_FILE_PATH, "r");
    if (!itemFile) {
        printf("Error: No items available.\n");
        return;
    }
    char itemHeader[100];
    fgets(itemHeader, sizeof(itemHeader), itemFile); // skip item file header
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

    // Open requests file in append mode and ensure it has a header
    FILE *file = fopen(REQUEST_FILE_PATH, "a+");  // Changed from "a" to "a+"
    if (!file) {
        printf("Error: Unable to open requests.txt for writing.\n");
        return;
    }
    
    // If the file is empty, write a header so we can skip it later
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    
    if (fileSize == 0) {
        fprintf(file, "request_id,item_id,recipient_username,status\n");
    } else if (fileSize > 0) {
        // Check if the last character is a newline
        fseek(file, -1, SEEK_END);
        char lastChar = fgetc(file);
        if (lastChar != '\n') {
            fprintf(file, "\n");  // Add a newline if needed
        }
        fseek(file, 0, SEEK_END);  // Move back to end for appending
    }

    // Figure out the last used request ID, so we know what the next one is
    int last_id = 0;
    FILE *readFile = fopen(REQUEST_FILE_PATH, "r");
    if (readFile) {
        char reqHeader[100];
        fgets(reqHeader, sizeof(reqHeader), readFile); // skip request file header
        Request tempReq;
        while (fscanf(readFile, "%d,%d,%20[^,],%20[^\n]\n",
                      &tempReq.request_id, &tempReq.item_id,
                      tempReq.recipient_username, tempReq.status) != EOF) {
            if (tempReq.request_id > last_id) {
                last_id = tempReq.request_id;
            }
        }
        fclose(readFile);
    }
    int request_id = last_id + 1;

    // Write the new request (with "pending" status) to the file
    fprintf(file, "%d,%d,%s,pending\n", request_id, item_id, recipient_username);
    fclose(file);
    printf("Request successfully submitted!\n");
}

// Donors can approve or reject a request
void approve_request(char *donor_username) {
    // Ensure data directory exists
    ensure_data_directory();
    
    if (count_pending_requests(donor_username) == 0) {
        printf("No pending requests for approval.\n");
        return;
    }
    // Display the donor's inbox
    printf("\nPending Requests for Approval:\n");
    view_inbox(donor_username);

    int request_id;
    char decision[10];
    printf("Enter the ID of the request to approve/reject: ");
    if (scanf("%d", &request_id) != 1) {
        printf("Invalid input for request ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Enter decision (approve/reject): ");
    if (scanf("%9s", decision) != 1) {
        printf("Invalid input for decision.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    local_to_lowercase(decision);

    FILE *file = fopen(REQUEST_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open requests.txt for reading.\n");
        return;
    }
    char reqHeader[100];
    fgets(reqHeader, sizeof(reqHeader), file); // skip request file header
    FILE *tempFile = fopen("../data/temp_requests.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    // Write the CSV header into the temp file
    fprintf(tempFile, "request_id,item_id,recipient_username,status\n");

    Request temp;
    int found = 0;
    // Check each request
    while (fscanf(file, "%d,%d,%20[^,],%20[^\n]\n",
                  &temp.request_id, &temp.item_id,
                  temp.recipient_username, temp.status) != EOF) {
        if (temp.request_id == request_id) {
            found = 1;
            if (strcmp(decision, "approve") == 0) {
                strcpy(temp.status, "approved");
                // Also mark the item as donated
                update_status(temp.item_id, "donated");
            } else if (strcmp(decision, "reject") == 0) {
                strcpy(temp.status, "rejected");
            } else {
                printf("Invalid decision. Request not updated.\n");
                fprintf(tempFile, "%d,%d,%s,%s\n",
                        temp.request_id, temp.item_id,
                        temp.recipient_username, temp.status);
                continue;
            }
        }
        fprintf(tempFile, "%d,%d,%s,%s\n",
                temp.request_id, temp.item_id,
                temp.recipient_username, temp.status);
    }
    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Request ID not found.\n");
        remove("../data/temp_requests.txt");
    } else {
        remove(REQUEST_FILE_PATH);
        if (rename("../data/temp_requests.txt", REQUEST_FILE_PATH) != 0) {
            printf("Error: Unable to update requests file.\n");
        } else {
            printf("Request successfully updated.\n");
        }
    }
}

// Shows all pending requests for this donor
void view_inbox(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No request notifications available.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // skip request file header
    int found = 0;
    Request req;

    printf("\nInbox - Pending Requests:\n");
    printf("-------------------------------------------------\n");
    printf("ReqID | ItemID | Recipient\n");
    printf("-------------------------------------------------\n");

    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id,
                  req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                printf("Error: Unable to open items.txt.\n");
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // skip item file header
            Item item;
            int donorMatch = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username, item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id &&
                    strcmp(item.donor_username, donor_username) == 0) {
                    donorMatch = 1;
                    break;
                }
            }
            fclose(itemFile);

            if (donorMatch) {
                printf("%-6d| %-7d| %-20s\n",
                       req.request_id, req.item_id, req.recipient_username);
                found = 1;
            }
        }
    }
    fclose(reqFile);

    if (!found) {
        printf("No pending notifications.\n");
    }
}

// Counts how many pending requests belong to this donor
int count_pending_requests(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        return 0;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // skip request file header
    int count = 0;
    Request req;

    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id,
                  req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // skip item file header
            Item item;
            int donorMatch = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username,
                          item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id &&
                    strcmp(item.donor_username, donor_username) == 0) {
                    donorMatch = 1;
                    break;
                }
            }
            fclose(itemFile);
            if (donorMatch) {
                count++;
            }
        }
    }
    fclose(reqFile);
    return count;
}

// Shows items that have been approved for a given recipient
void view_inventory(char *recipient_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No inventory records available.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // skip request file header
    int found = 0;
    Request req;

    printf("\nYour Inventory (Approved Items):\n");
    printf("---------------------------------------------------------------\n");
    printf("ReqID | ItemID | Category         | Description\n");
    printf("---------------------------------------------------------------\n");

    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id,
                  req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "approved") == 0 &&
            strcmp(req.recipient_username, recipient_username) == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // skip item file header
            Item item;
            int itemFound = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username,
                          item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id) {
                    itemFound = 1;
                    break;
                }
            }
            fclose(itemFile);

            if (itemFound) {
                printf("%-6d| %-7d| %-17s| %s\n",
                       req.request_id, req.item_id, item.category, item.description);
                found = 1;
            }
        }
    }
    fclose(reqFile);

    if (!found) {
        printf("Your inventory is empty.\n");
    }
}