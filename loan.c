#include "loan.h"

void add_loan(Loan ln) {
    FILE *fp = fopen(LOAN_FILE, "a");
    if (!fp) { perror("loan file"); return; }
    fprintf(fp, "%d %d %.2f %s %s\n", ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    fclose(fp);
}

void list_loans() {
    FILE *fp = fopen(LOAN_FILE, "r");
    if (!fp) { printf("No loans yet.\n"); return; }
    Loan ln;
    printf("\n--- Loan List ---\n");
    while (fscanf(fp, "%d %d %lf %s %s", &ln.loanID, &ln.accountID, &ln.amount, ln.reason, ln.status) == 5)
        printf("LoanID: %d | Acc: %d | Amt: %.2f | Reason: %s | Status: %s\n",
               ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    fclose(fp);
}

void update_loan_status(int loanID, const char *newStatus) {
    FILE *fp = fopen(LOAN_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    Loan ln;
    int found = 0;
    while (fscanf(fp, "%d %d %lf %s %s", &ln.loanID, &ln.accountID, &ln.amount, ln.reason, ln.status) == 5) {
        if (ln.loanID == loanID) {
            strcpy(ln.status, newStatus);
            found = 1;
        }
        fprintf(temp, "%d %d %.2f %s %s\n", ln.loanID, ln.accountID, ln.amount, ln.reason, ln.status);
    }
    fclose(fp);
    fclose(temp);
    rename("temp.txt", LOAN_FILE);
    if (!found) printf("Loan %d not found.\n", loanID);
    else printf("Loan %d updated to %s.\n", loanID, newStatus);
}
