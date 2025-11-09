#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FEEDBACK_FILE "feedback.txt"

typedef struct {
    int feedbackID;
    int accountID;
    char message[200];
    char reply[200];
} Feedback;

void add_feedback(Feedback fb);
void list_feedbacks();
void add_reply(int feedbackID, const char *reply);

#endif
