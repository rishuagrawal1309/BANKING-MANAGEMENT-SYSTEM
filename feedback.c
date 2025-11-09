#include "feedback.h"
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define CLEAR   "\033[2J\033[H"

void add_feedback(Feedback fb) {
    FILE *fp = fopen(FEEDBACK_FILE, "a");
    if (!fp) { perror("feedback.txt"); return; }
    fprintf(fp, "%d %d %s %s\n",
            fb.feedbackID, fb.accountID, fb.message, fb.reply);
    fclose(fp);
printf(GREEN "✔ Feedback recorded successfully!\n" RESET);

}

void list_feedbacks() {
    FILE *fp = fopen(FEEDBACK_FILE, "r");
    if (!fp) {
        printf(RED "✖ No feedback found.\n" RESET);
        return;
    }

    printf(CLEAR);
    printf(CYAN "=====================================================\n" RESET);
    printf(YELLOW "                   FEEDBACK DASHBOARD                \n" RESET);
    printf(CYAN "=====================================================\n" RESET);
    printf(WHITE "ID     Account     Message                Reply\n" RESET);
    printf(CYAN "-----------------------------------------------------\n" RESET);

    Feedback fb;
    while (fscanf(fp, "%d %d %s %s",
                  &fb.feedbackID, &fb.accountID, fb.message, fb.reply) == 4) {
        printf(GREEN "%-6d" RESET " %-10d %-20s %-20s\n",
               fb.feedbackID, fb.accountID, fb.message, fb.reply);
    }

    printf(CYAN "-----------------------------------------------------\n\n" RESET);
    fclose(fp);
}


void add_reply(int feedbackID, const char *reply) {
    FILE *fp = fopen(FEEDBACK_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror(RED "✖ Error opening feedback file" RESET);
        return;
    }

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
        printf(GREEN "✔ Reply added to Feedback ID %d.\n" RESET, feedbackID);
    else
        printf(RED "✖ Feedback ID %d not found.\n" RESET, feedbackID);
}
