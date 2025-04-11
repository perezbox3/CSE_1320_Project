#include "requests.h"
#include "items.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------
// Helper function to clear input buffer.
//------------------------------------
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//------------------------------------
// Converts a string to lowercase.
//------------------------------------
static void local_to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//------------------------------------
// Requests an item by first listing available items.
//------------------------------------
void request_item(char *recipient_username) {
    // List available items.
    display_items();
    FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
    int available = 0;
    if (itemFile) {
        char itemHeader[100];
        fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
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
    int item_id;
    printf("Enter the ID of the item you want to request: ");
    if (scanf("%d", &item_id) != 1) {
        printf("Invalid input for item ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Verify the item exists and is available.
    itemFile = fopen(ITEM_FILE_PATH, "r");
    if (!itemFile) {
        printf("Error: No items available.\n");
        return;
    }
    char itemHeader[100];
    fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
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
    int last_id = 0;
    FILE *readFile = fopen(REQUEST_FILE_PATH, "r");
    if (readFile) {
        char reqHeader[100];
        fgets(reqHeader, sizeof(reqHeader), readFile); // Skip header.
        Request tempReq;
        while (fscanf(readFile, "%d,%d,%20[^,],%20[^\n]\n",
                      &tempReq.request_id, &tempReq.item_id, tempReq.recipient_username, tempReq.status) != EOF) {
            last_id = tempReq.request_id;
        }
        fclose(readFile);
    }
    int request_id = last_id + 1;
    fprintf(file, "%d,%d,%s,pending\n", request_id, item_id, recipient_username);
    fclose(file);
    printf("Request successfully submitted!\n");
}

//------------------------------------
// Approves or rejects a request.
//------------------------------------
void approve_request(char *donor_username) {
    if (count_pending_requests(donor_username) == 0) {
        printf("No pending requests for approval.\n");
        return;
    }
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
    fgets(reqHeader, sizeof(reqHeader), file); // Skip header.
    FILE *tempFile = fopen("../data/temp_requests.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    // Write header line.
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
                printf("Invalid decision. Request not updated.\n");
                fprintf(tempFile, "%d,%d,%s,%s\n", temp.request_id, temp.item_id,
                        temp.recipient_username, temp.status);
                continue;
            }
        }
        fprintf(tempFile, "%d,%d,%s,%s\n", temp.request_id, temp.item_id,
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

//------------------------------------
// update_status is defined in items.c; do not redefine here.
//------------------------------------

//------------------------------------
// Views the donor's inbox (pending requests).
//------------------------------------
void view_inbox(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No request notifications available.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header.
    int found = 0;
    Request req;
    printf("\nInbox - Pending Requests:\n");
    printf("-------------------------------------------------\n");
    printf("ReqID | ItemID | Recipient\n");
    printf("-------------------------------------------------\n");
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                printf("Error: Unable to open items.txt.\n");
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
            Item item;
            int donorMatch = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username, item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id && strcmp(item.donor_username, donor_username) == 0) {
                    donorMatch = 1;
                    break;
                }
            }
            fclose(itemFile);
            if (donorMatch) {
                printf("%-6d| %-7d| %-20s\n", req.request_id, req.item_id, req.recipient_username);
                found = 1;
            }
        }
    }
    fclose(reqFile);
    if (!found) {
        printf("No pending notifications.\n");
    }
}

//------------------------------------
// Counts pending requests for items owned by the donor.
//------------------------------------
int count_pending_requests(char *donor_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        return 0;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header.
    int count = 0;
    Request req;
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "pending") == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
            Item item;
            int donorMatch = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username, item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id && strcmp(item.donor_username, donor_username) == 0) {
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

//------------------------------------
// Views approved items (inventory) for the recipient.
//------------------------------------
void view_inventory(char *recipient_username) {
    FILE *reqFile = fopen(REQUEST_FILE_PATH, "r");
    if (!reqFile) {
        printf("No inventory records available.\n");
        return;
    }
    char header[100];
    fgets(header, sizeof(header), reqFile); // Skip header.
    int found = 0;
    Request req;
    printf("\nYour Inventory (Approved Items):\n");
    printf("---------------------------------------------------------------\n");
    printf("ReqID | ItemID | Category         | Description\n");
    printf("---------------------------------------------------------------\n");
    while (fscanf(reqFile, "%d,%d,%20[^,],%20[^\n]\n",
                  &req.request_id, &req.item_id, req.recipient_username, req.status) != EOF) {
        if (strcmp(req.status, "approved") == 0 && strcmp(req.recipient_username, recipient_username) == 0) {
            FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
            if (!itemFile) {
                continue;
            }
            char itemHeader[100];
            fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
            Item item;
            int itemFound = 0;
            while (fscanf(itemFile, "%d,%20[^,],%20[^,],%99[^,],%20[^,],%20[^\n]\n",
                          &item.item_id, item.donor_username, item.category, item.description,
                          item.condition, item.status) != EOF) {
                if (item.item_id == req.item_id) {
                    itemFound = 1;
                    break;
                }
            }
            fclose(itemFile);
            if (itemFound) {
                printf("%-6d| %-7d| %-17s| %s\n", req.request_id, req.item_id,
                       item.category, item.description);
                found = 1;
            }
        }
    }
    fclose(reqFile);
    if (!found) {
        printf("Your inventory is empty.\n");
    }
}
