#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"  // Includes the definition for Item structure used in the donation system.

/*
 * REQUEST_FILE_PATH
 * -----------------
 * Defines the relative path to the text file that stores donation requests.
 * This file holds all request records (each record contains request ID, item ID, 
 * the username of the recipient who made the request, and the current status of the request).
 */
#define REQUEST_FILE_PATH "../data/requests.txt"

/*
 * Structure: Request
 * --------------------
 * Represents a donation request made by a recipient for a specific donation item.
 *
 * Fields:
 *  - request_id         : A unique identifier assigned to each donation request.
 *  - item_id            : The unique identifier of the donation item being requested.
 *  - recipient_username : The username of the recipient making the request. The field
 *                         can store up to 20 characters plus the null terminator.
 *  - status             : The current status of the request. Expected values are:
 *                           "pending"  - Request has been submitted and awaits donor review.
 *                           "approved" - Donor has approved the request.
 *                           "rejected" - Donor has rejected the request.
 */
typedef struct {
    int request_id;
    int item_id;
    char recipient_username[21];
    char status[21]; // Expected values: "pending", "approved", or "rejected"
} Request;

/*
 * Function Prototypes:
 *
 * request_item(char *recipient_username)
 * ----------------------------------------
 * Allows a recipient to submit a donation request for a specific item. This function:
 *  - Lists available items so the recipient can choose a valid item ID.
 *  - Verifies that the chosen item exists and is available.
 *  - Generates a new unique request_id by scanning the existing requests.
 *  - Appends the request with a "pending" status to the requests file.
 *
 * approve_request(char *donor_username)
 * ---------------------------------------
 * Enables a donor to review and either approve or reject pending requests for items they own.
 * It:
 *  - Displays pending requests (using view_inbox()).
 *  - Prompts for the request ID and decision ("approve" or "reject").
 *  - Updates the request status accordingly.
 *  - If approved, it calls update_status() (declared in items.h and defined in items.c)
 *    to update the associated donation item's status.
 *
 * Note:
 *  The update_status() function is not defined here; it is declared in items.h and implemented
 *  within items.c, ensuring a single, centralized update mechanism.
 *
 * view_inbox(char *donor_username)
 * ----------------------------------
 * Displays a list (or "inbox") of pending requests for donation items owned by the donor.
 * This function reads the requests file (skipping the header) and shows only those requests
 * with the status "pending" that correspond to items whose donor username matches the logged-in donor.
 *
 * count_pending_requests(char *donor_username)
 * ----------------------------------------------
 * Iterates through the requests file to count the number of pending requests for items belonging
 * to the given donor. This count is used in the donor's main menu to display a notification badge.
 *
 * view_inventory(char *recipient_username)
 * ------------------------------------------
 * For recipients, this function displays their "inventory" of items that have been approved (i.e., donated).
 * It reads through the requests file and, for each request with an "approved" status corresponding to the
 * logged-in recipient, retrieves item details (like category and description) from the items file.
 */
void request_item(char *recipient_username);
void approve_request(char *donor_username);
/* Note: update_status is now defined in items.c and declared in items.h */

void view_inbox(char *donor_username);
int count_pending_requests(char *donor_username);
void view_inventory(char *recipient_username);

#endif /* REQUESTS_H */
