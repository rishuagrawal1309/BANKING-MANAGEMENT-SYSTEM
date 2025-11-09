#include "feedback.h"

void add_feedback(Feedback fb) {
    FILE *fp = fopen(FEEDBACK_FILE, "a");
    if (!fp) { perror("feedback.txt"); return; }
    fprintf(fp, "%d %d %s %s\n",
            fb.feedbackID, fb.accountID, fb.message, fb.reply);
    fclose(fp);
}

void list_feedbacks() {
    FILE *fp = fopen(FEEDBACK_FILE, "r");
    if (!fp) { printf("No feedback found.\n"); return; }
    Feedback fb;
    printf("\n--- FEEDBACK LIST ---\n");
    while (fscanf(fp, "%d %d %s %s",
                  &fb.feedbackID, &fb.accountID, fb.message, fb.reply) == 4) {
        printf("ID:%d | Acc:%d | Msg:%s | Reply:%s\n",
               fb.feedbackID, fb.accountID, fb.message, fb.reply);
    }
    fclose(fp);
}

void add_reply(int feedbackID, const char *reply) {
    FILE *fp = fopen(FEEDBACK_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { perror("feedback file"); return; }

    Feedback fb;
    int found = 0;
    while (fscanf(fp, "%d %d %s %s",
                  &fb.feedbackID, &fb.accountID, fb.message, fb.reply) == 4) {
        if (fb.feedbackID == feedbackID) {
            strcpy(fb.reply, reply);
            found = 1;
        }
        fprintf(temp, "%d %d %s %s\n",
                fb.feedbackID, fb.accountID, fb.message, fb.reply);
    }
    fclose(fp);
    fclose(temp);
    rename("temp.txt", FEEDBACK_FILE);

    if (found)
        printf("Reply added to feedback %d.\n", feedbackID);
    else
        printf("Feedback %d not found.\n", feedbackID);
}
