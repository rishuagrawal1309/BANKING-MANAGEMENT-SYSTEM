#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "account.h"
#include <string.h>
#include "loan.h"
#include "feedback.h"
// read one account by ID
Account read_account(int id) {
    Account acc = {id, 0};
    FILE *fp = fopen(ACC_FILE, "r");
    if (!fp) return acc;

    while (fscanf(fp, "%d %lf", &acc.accountID, &acc.balance) == 2) {
        if (acc.accountID == id) {
            fclose(fp);
            return acc;
        }
    }
    fclose(fp);
    return (Account){id, 0};
}

// write/update one account
void write_account(Account acc) {
    FILE *fp = fopen(ACC_FILE, "r+");
    if (!fp) fp = fopen(ACC_FILE, "w+");
    FILE *temp = fopen("temp.txt", "w");

    int id; double bal;
    int found = 0;
    while (fscanf(fp, "%d %lf", &id, &bal) == 2) {
        if (id == acc.accountID) {
            fprintf(temp, "%d %.2f\n", acc.accountID, acc.balance);
            found = 1;
        } else {
            fprintf(temp, "%d %.2f\n", id, bal);
        }
    }
    if (!found)
        fprintf(temp, "%d %.2f\n", acc.accountID, acc.balance);

    fclose(fp);
    fclose(temp);
    rename("temp.txt", ACC_FILE);
}

// append to transaction log
void log_transaction(int id, const char *type, double amount) {
    FILE *fp = fopen(TXN_FILE, "a");
    if (!fp) return;
    fprintf(fp, "Account %d: %s ₹%.2f\n", id, type, amount);
    fclose(fp);
}
int verify_account(int accountID, const char *password);

void customer_menu(int connFD) {
    int choice;
    double amt;
    int accountID;
    char enteredPassword[50];
    char buf[256];

    // Step 1: Ask for account ID
    write(connFD, "ENTER_ACCOUNT_ID\n", strlen("ENTER_ACCOUNT_ID\n"));
    memset(buf, 0, sizeof(buf));
    read(connFD, buf, sizeof(buf));
    accountID = atoi(buf);

    // Step 2: Ask for password
    write(connFD, "ENTER_ACCOUNT_PASSWORD\n", strlen("ENTER_ACCOUNT_PASSWORD\n"));
    memset(enteredPassword, 0, sizeof(enteredPassword));
    read(connFD, enteredPassword, sizeof(enteredPassword));

    // Step 3: Verify account
    int verified = verify_account(accountID, enteredPassword);
    if (!verified) {
        write(connFD, "ERR_NO_ACCOUNT\n", strlen("ERR_NO_ACCOUNT\n"));
        return;
    }

    write(connFD, "ACCOUNT_LOGIN_OK\n", strlen("ACCOUNT_LOGIN_OK\n"));

    // Step 4: Menu loop
    while (1) {
        const char *menu =
            "\n--- CUSTOMER MENU ---\n"
            "1. View Balance\n"
            "2. Deposit\n"
            "3. Withdraw\n"
            "4. Logout\n"
            "5. Apply for Loan\n"
            "6. Give Feedback\n"
            "Enter choice:\n";
        write(connFD, menu, strlen(menu));

        memset(buf, 0, sizeof(buf));
        if (read(connFD, buf, sizeof(buf)) <= 0)
            break;
        choice = atoi(buf);

        Account acc = read_account(accountID);
        char msg[256];

        switch (choice) {
            case 1:
                snprintf(msg, sizeof(msg), "Current balance: ₹%.2f\n", acc.balance);
                write(connFD, msg, strlen(msg));
                break;

            case 2:
                write(connFD, "Enter amount to deposit:\n", strlen("Enter amount to deposit:\n"));
                memset(buf, 0, sizeof(buf));
                read(connFD, buf, sizeof(buf));
                amt = atof(buf);

                acc.balance += amt;
                write_account(acc);
                log_transaction(accountID, "Deposit", amt);

                snprintf(msg, sizeof(msg), "Deposited ₹%.2f. New balance ₹%.2f\n", amt, acc.balance);
                write(connFD, msg, strlen(msg));
                break;

            case 3:
                write(connFD, "Enter amount to withdraw:\n", strlen("Enter amount to withdraw:\n"));
                memset(buf, 0, sizeof(buf));
                read(connFD, buf, sizeof(buf));
                amt = atof(buf);

                if (amt > acc.balance)
                    write(connFD, "Insufficient funds!\n", strlen("Insufficient funds!\n"));
                else {
                    acc.balance -= amt;
                    write_account(acc);
                    log_transaction(accountID, "Withdraw", amt);
                    snprintf(msg, sizeof(msg), "Withdrawn ₹%.2f. New balance ₹%.2f\n", amt, acc.balance);
                    write(connFD, msg, strlen(msg));
                }
                break;

            case 5: {
                Loan ln;
                write(connFD, "Enter loan amount:\n", strlen("Enter loan amount:\n"));
                memset(buf, 0, sizeof(buf));
                read(connFD, buf, sizeof(buf));
                ln.amount = atof(buf);

                write(connFD, "Enter loan reason (one word):\n", strlen("Enter loan reason (one word):\n"));
                memset(ln.reason, 0, sizeof(ln.reason));
                read(connFD, ln.reason, sizeof(ln.reason));

                ln.accountID = accountID;
                ln.loanID = rand() % 10000;
                strcpy(ln.status, "Pending");
                add_loan(ln);

                snprintf(msg, sizeof(msg), "Loan applied successfully (ID=%d)\n", ln.loanID);
                write(connFD, msg, strlen(msg));
                break;
            }

            case 6: {
                Feedback fb;
                fb.feedbackID = rand() % 10000;
                fb.accountID = accountID;
                write(connFD, "Enter your feedback (one word):\n", strlen("Enter your feedback (one word):\n"));
                memset(fb.message, 0, sizeof(fb.message));
                read(connFD, fb.message, sizeof(fb.message));
                strcpy(fb.reply, "Pending");
                add_feedback(fb);

                snprintf(msg, sizeof(msg), "Feedback submitted (ID=%d)\n", fb.feedbackID);
                write(connFD, msg, strlen(msg));
                break;
            }

            case 4:
                write(connFD, "SESSION_END\n", strlen("SESSION_END\n"));
                return; // exit the function (and thread)
                break;

            default:
                write(connFD, "Invalid choice.\n", strlen("Invalid choice.\n"));
        }
    }
}


int verify_account(int accountID, const char *password) {
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) {
        perror("Error opening accounts.txt");
        return 0;
    }

    int id;
    char pass[50];
    double balance;

    // Expected format: accountID password balance
    while (fscanf(fp, "%d %s %lf", &id, pass, &balance) == 3) {
        if (id == accountID && strcmp(pass, password) == 0) {
            fclose(fp);
            return 1;  // ✅ Account and password match
        }
    }

    fclose(fp);
    return 0;  // ❌ Account not found or wrong password
}
