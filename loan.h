#ifndef LOAN_H
#define LOAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOAN_FILE "loans.txt"

typedef struct {
    int loanID;
    int accountID;
    double amount;
    char reason[100];
    char status[20];   // Pending / Approved / Rejected / Processed
} Loan;

void add_loan(Loan ln);
void list_loans();
void update_loan_status(int loanID, const char *newStatus);

#endif
