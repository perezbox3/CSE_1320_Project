#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"  // Includes the definition for Item

/* File path for storing donation requests */
#define REQUEST_FILE_PATH "../data/requests.txt"

/* Structure representing a donation request */
typedef struct {
    int request_id;
    int item_id;
    char recipient_username[21];
    char status[21]; // Expected values: "pending", "approved", or "rejected"
} Request;

/* Function prototypes for handling donation requests */
void request_item(char *recipient_username);
void approve_request(char *donor_username);
/* Note: update_status is now defined in items.c and declared in items.h */

/* Functions for donor notifications */
void view_inbox(char *donor_username);
int count_pending_requests(char *donor_username);

/* Function for recipient inventory */
void view_inventory(char *recipient_username);

#endif /* REQUESTS_H */
