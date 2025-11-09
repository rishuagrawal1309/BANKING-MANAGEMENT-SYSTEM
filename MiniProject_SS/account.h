#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ACC_FILE "accounts.txt"
#define TXN_FILE "transactions.txt"

typedef struct {
    int accountID;
    double balance;
  
} Account;

// Function prototypes
Account read_account(int id);
void write_account(Account acc);
void log_transaction(int id, const char *type, double amount);

#endif
