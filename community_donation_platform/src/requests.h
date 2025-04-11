#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"  // Add this include

#define REQUEST_FILE_PATH "../data/requests.txt"

// Request structure
typedef struct {
    int request_id;
    int item_id;
    char recipient_username[21];
    char status[21]; // "pending", "approved", or "rejected"
} Request;

// Function prototypes
void request_item(char *recipient_username);
void approve_request(char *donor_username);
void update_status(int item_id, char *new_status);

#endif
