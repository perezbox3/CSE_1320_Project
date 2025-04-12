#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"  // Uses the Item structure defined in items.h

// This file contains the definitions and function prototypes related to handling requests.
// Recipients can request items, and donors can approve or reject those requests.

// The location of the file that holds all request information.
#define REQUEST_FILE_PATH "../data/requests.txt"

// Structure representing a request for a donation item.
typedef struct {
    int request_id;                   // Unique ID for each request
    int item_id;                      // ID of the item being requested
    char recipient_username[21];      // Username of the person requesting the item
    char status[21];                  // Status: "pending", "approved", or "rejected"
} Request;

// Function prototypes:

// Allows a recipient to request an available item.
// Shows available items, takes the user's choice, and saves the request.
void request_item(char *recipient_username);

// Allows donors to approve or reject requests made for their items.
// Shows all pending requests, lets the donor choose one, and updates its status.
void approve_request(char *donor_username);

// Shows a list of pending requests (inbox) for items belonging to a specific donor.
void view_inbox(char *donor_username);

// Counts how many pending requests a donor has (used to show notifications).
int count_pending_requests(char *donor_username);

// Shows recipients a list of items that have been approved (their inventory).
void view_inventory(char *recipient_username);

// Note: update_status() is declared in items.h and defined in items.c.
// It's used to update the item's status when a request is approved.

#endif /* REQUESTS_H */
