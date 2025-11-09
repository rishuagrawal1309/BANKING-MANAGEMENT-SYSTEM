#include "loan.h"
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define CLEAR   "\033[2J\033[H"

void add_loan(Loan ln) {
    FILE *fp = fopen(LOAN_FILE, "a");
    if (!fp) { perror(RED "✖ Error opening loan file" RESET); return; }

    fprintf(fp, "%d %d %.2f %s %s\n", ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    fclose(fp);

    printf(GREEN "✔ Loan application recorded successfully!\n" RESET);
}


void list_loans() {
    FILE *fp = fopen(LOAN_FILE, "r");
    if (!fp) {
        printf(RED "✖ No loans found in the system.\n" RESET);
        return;
    }

    printf(CLEAR);
    printf(CYAN "===========================================================\n" RESET);
    printf(YELLOW "                          LOAN LIST                        \n" RESET);
    printf(CYAN "===========================================================\n" RESET);
    printf(WHITE "LoanID   Account   Amount       Reason            Status\n" RESET);
    printf(CYAN "-----------------------------------------------------------\n" RESET);

    Loan ln;
    while (fscanf(fp, "%d %d %lf %s %s",
                  &ln.loanID, &ln.accountID, &ln.amount, ln.reason, ln.status) == 5) {
        printf(GREEN "%-8d" RESET " %-8d %-12.2f %-15s %-10s\n",
               ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    }

    printf(CYAN "-----------------------------------------------------------\n\n" RESET);
    fclose(fp);
}


void update_loan_status(int loanID, const char *newStatus) {
    FILE *fp = fopen(LOAN_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        perror(RED "✖ Error updating loan file" RESET);
        return;
    }

    Loan ln;
    int found = 0;

    while (fscanf(fp, "%d %d %lf %s %s",
                  &ln.loanID, &ln.accountID, &ln.amount, ln.reason, ln.status) == 5) {

        if (ln.loanID == loanID) {
            strcpy(ln.status, newStatus);
            found = 1;
        }

        fprintf(temp, "%d %d %.2f %s %s\n",
                ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    }

    fclose(fp);
    fclose(temp);
    rename("temp.txt", LOAN_FILE);

    if (found)
        printf(GREEN "✔ Loan %d updated to '%s'.\n" RESET, loanID, newStatus);
    else
        printf(RED "✖ Loan ID %d not found.\n" RESET, loanID);
}

void loan_approval_ui() {
    FILE *fp = fopen(LOAN_FILE, "r");
    if (!fp) {
        printf(RED "✖ No loans found.\n" RESET);
        return;
    }

    Loan ln;
    int pendingExists = 0;

    while (fscanf(fp, "%d %d %lf %s %s",
                  &ln.loanID, &ln.accountID, &ln.amount, ln.reason, ln.status) == 5) {

        if (strcmp(ln.status, "Pending") == 0) {
            pendingExists = 1;

            printf(CLEAR);
            printf(CYAN "===============================================\n" RESET);
            printf(YELLOW "                LOAN APPROVAL PANEL            \n" RESET);
            printf(CYAN "===============================================\n" RESET);

            printf(WHITE  "Loan ID: " RESET "%d\n", ln.loanID);
            printf(WHITE  "Account ID: " RESET "%d\n", ln.accountID);
            printf(WHITE  "Loan Amount: " RESET "₹%.2f\n", ln.amount);
            printf(WHITE  "Purpose: " RESET "%s\n", ln.reason);
            printf(WHITE  "Current Status: " RESET "%s\n", ln.status);

            printf(CYAN "-----------------------------------------------\n" RESET);
            printf(GREEN "[A]" RESET " Approve   ");
            printf(RED "[R]" RESET " Reject   ");
            printf(YELLOW "[S]" RESET " Skip   ");
            printf(BLUE "[E]" RESET " Exit\n");
            printf(CYAN "-----------------------------------------------\n" RESET);
            printf(BLUE "Enter choice: " RESET);

            char ch;
            scanf(" %c", &ch);

            if (ch == 'A' || ch == 'a') {
                update_loan_status(ln.loanID, "Approved");
                printf(GREEN "✔ Loan Approved.\n" RESET);
                sleep(1);
            }
            else if (ch == 'R' || ch == 'r') {
                update_loan_status(ln.loanID, "Rejected");
                printf(RED "✖ Loan Rejected.\n" RESET);
                sleep(1);
            }
            else if (ch == 'E' || ch == 'e') {
                fclose(fp);
                return;
            }
        }
    }

    fclose(fp);

    if (!pendingExists) {
        printf(YELLOW "⚠ No pending loans at the moment.\n" RESET);
        sleep(1);
    }
}
