// requests.h
// This file defines how we handle "Request" records in the donation system. 
// We have a struct for requests and a few functions that let recipients request items,
// let donors approve or reject them, and also let us see what's been requested.

#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"  // For the Item struct and update_status function

// Where we store the requests
#define REQUEST_FILE_PATH "../data/requests.txt"

// Each request has a unique request_id, an item_id, a recipient username, and a status
typedef struct {
    int request_id;
    int item_id;
    char recipient_username[21]; 
    char status[21]; // "pending", "approved", or "rejected"
} Request;

// Lets a recipient ask for an available item
void request_item(char *recipient_username);

// Lets a donor approve or reject an item request
void approve_request(char *donor_username);

// Shows the donor all the pending requests for their items
void view_inbox(char *donor_username);

// Counts how many pending requests belong to a specific donor
int count_pending_requests(char *donor_username);

// Shows a recipient all items that have been approved for them
void view_inventory(char *recipient_username);

#endif /* REQUESTS_H */
