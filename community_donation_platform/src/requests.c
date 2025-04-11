#include "requests.h"   // Include definitions for request functions and the Request structure.
#include "items.h"      // Include definitions for items (such as the Item structure and update_status).
#include <ctype.h>      // For using the tolower() function.
#include <string.h>     // For string manipulation functions like strcpy() and strcmp().
#include <stdio.h>      // For standard I/O functions.
#include <stdlib.h>     // For standard library functions like exit().

//------------------------------------
// Helper function to clear input buffer.
//------------------------------------
/*
 * clear_input_buffer()
 *
 * This function discards any leftover characters in the standard input
 * buffer. It is useful after using scanf() to prevent stray newline characters
 * or any unexpected input from affecting subsequent input operations.
 */
static void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

//------------------------------------
// Converts a string to lowercase.
//------------------------------------
/*
 * local_to_lowercase(char *str)
 *
 * This helper function converts every character in the provided string to its
 * lowercase equivalent using the tolower() function. It is used to perform
 * case-insensitive comparisons.
 */
static void local_to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//------------------------------------
// Requests an item by first listing available items.
//------------------------------------
/*
 * request_item(char *recipient_username)
 *
 * This function implements the process for a recipient to request an item.
 * It first displays all available items so the recipient can view valid item IDs.
 *
 * Steps:
 *   1. Calls display_items() to list available items.
 *   2. Opens the items file and skips the header.
 *   3. Checks for at least one available item; if none, informs the user and returns.
 *   4. Prompts the user to input an item ID.
 *   5. Verifies that the item exists and its status is "available".
 *   6. Opens the requests file to determine a new, unique request_id.
 *   7. Appends the new request (with status set to "pending") to the requests file.
 *   8. Closes the file and confirms successful submission.
 */
void request_item(char *recipient_username) {
    // List available items so the recipient can see valid selections.
    display_items();

    // Open the items file for reading to check if any items are available.
    FILE *itemFile = fopen(ITEM_FILE_PATH, "r");
    int available = 0;
    if (itemFile) {
        char itemHeader[100];
        // Skip the header line.
        fgets(itemHeader, sizeof(itemHeader), itemFile);
        Item temp;
        // Check if there is at least one item with status "available".
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
    
    // Prompt the recipient to input the ID of the item they want to request.
    int item_id;
    printf("Enter the ID of the item you want to request: ");
    if (scanf("%d", &item_id) != 1) {
        printf("Invalid input for item ID.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Verify that the item exists and is available.
    itemFile = fopen(ITEM_FILE_PATH, "r");
    if (!itemFile) {
        printf("Error: No items available.\n");
        return;
    }
    char itemHeader[100];
    fgets(itemHeader, sizeof(itemHeader), itemFile); // Skip header.
    Item temp_item;
    int item_found = 0;
    // Iterate through the items file to check if the given item ID exists and is available.
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
    
    // Open the requests file in append mode to add the new request.
    FILE *file = fopen(REQUEST_FILE_PATH, "a");
    if (!file) {
        printf("Error: Unable to open requests.txt for writing.\n");
        return;
    }
    int last_id = 0;
    FILE *readFile = fopen(REQUEST_FILE_PATH, "r");
    if (readFile) {
        char reqHeader[100];
        // Skip the header line.
        fgets(reqHeader, sizeof(reqHeader), readFile);
        Request tempReq;
        // Read existing requests to determine the last used request ID.
        while (fscanf(readFile, "%d,%d,%20[^,],%20[^\n]\n",
                      &tempReq.request_id, &tempReq.item_id, tempReq.recipient_username, tempReq.status) != EOF) {
            last_id = tempReq.request_id;
        }
        fclose(readFile);
    }
    int request_id = last_id + 1;
    // Append the new request (with status "pending") to the requests file.
    fprintf(file, "%d,%d,%s,pending\n", request_id, item_id, recipient_username);
    fclose(file);
    printf("Request successfully submitted!\n");
}

//------------------------------------
// Approves or rejects a request.
//------------------------------------
/*
 * approve_request(char *donor_username)
 *
 * This function allows a donor to approve or reject pending requests for items they own.
 * Steps:
 *   1. Check if there are any pending requests using count_pending_requests().
 *   2. If none exist, inform the donor and return.
 *   3. Otherwise, display pending requests by calling view_inbox().
 *   4. Prompt the donor to input the request ID they want to update.
 *   5. Prompt for the decision ("approve" or "reject").
 *   6. Open the requests file, skip the header, and iterate through each record.
 *   7. If the record matches the request ID, update its status accordingly:
 *        - If approved, set status to "approved" and call update_status() on the corresponding item.
 *        - If rejected, set status to "rejected".
 *   8. Write all records to a temporary file.
 *   9. Replace the original requests file with the temporary file.
 *   10. Confirm the update.
 */
void approve_request(char *donor_username) {
    // If there are no pending requests for this donor, exit the function.
    if (count_pending_requests(donor_username) == 0) {
        printf("No pending requests for approval.\n");
        return;
    }
    // Display the donor's inbox (pending requests) so they know which request to update.
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
    local_to_lowercase(decision);  // Convert decision to lowercase for uniform comparison.

    FILE *file = fopen(REQUEST_FILE_PATH, "r");
    if (!file) {
        printf("Error: Unable to open requests.txt for reading.\n");
        return;
    }
    char reqHeader[100];
    fgets(reqHeader, sizeof(reqHeader), file); // Skip header line.
    FILE *tempFile = fopen("../data/temp_requests.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    // Write the header line to the temporary file.
    fprintf(tempFile, "request_id,item_id,recipient_username,status\n");
    Request temp;
    int found = 0;
    // Process each request record.
    while (fscanf(file, "%d,%d,%20[^,],%20[^\n]\n",
                  &temp.request_id, &temp.item_id, temp.recipient_username, temp.status) != EOF) {
        if (temp.request_id == request_id) {
            found = 1;
            // Update the status based on the donor's decision.
            if (strcmp(decision, "approve") == 0) {
                strcpy(temp.status, "approved");
                // Also update the corresponding item status to "donated".
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
        // Write the (possibly updated) request record to the temporary file.
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

// Note: update_status() is defined in items.c, so we do not define it here.

//------------------------------------
// Views the donor's inbox (pending requests).
//------------------------------------
/*
 * view_inbox(char *donor_username)
 *
 * This function displays a list of pending requests made for items that are owned by the donor.
 * It reads the requests file (skipping the header) and, for each request with status "pending",
 * opens the items file to verify the item belongs to the donor.
 * If it matches, the request is printed in a formatted table.
 */
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
            // Open the items file and check if the request corresponds to an item owned by the donor.
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
/*
 * count_pending_requests(char *donor_username)
 *
 * This function counts the number of pending requests for items owned by the donor.
 * It opens the requests file (skipping the header) and iterates through each record.
 * For every "pending" request, it confirms the item belongs to the donor by checking in the items file.
 * It then returns the total count.
 */
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
/*
 * view_inventory(char *recipient_username)
 *
 * This function displays a list of approved requests (i.e., items that have been "donated")
 * for the currently logged-in recipient. It reads the requests file (skipping the header) and,
 * for each record with status "approved" that belongs to the recipient, it retrieves the associated
 * item details (category and description) from the items file.
 */
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
