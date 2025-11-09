#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "account.h"
#include <string.h>
#include "loan.h"
#include "feedback.h"
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define CLEAR "\033[2J\033[H"

// read one account by ID
Account read_account(int id) {
    Account acc = {id, 0};
    FILE *fp = fopen(ACC_FILE, "r");
    if (!fp) return acc;

    int file_id;
    char pass[50];
    double balance;
    while (fscanf(fp, "%d %s %lf", &file_id, pass, &balance) == 3) {
        if (file_id == id) {
            acc.balance = balance;
            fclose(fp);
            return acc;
        }
    }
    fclose(fp);
    return acc;
}

// write/update one account
void write_account(Account acc) {
    FILE *fp = fopen(ACC_FILE, "r");
    if (!fp) fp = fopen(ACC_FILE, "w");

    FILE *temp = fopen("temp.txt", "w");
    int id;
    char pass[50];
    double bal;
    int found = 0;

    while (fscanf(fp, "%d %s %lf", &id, pass, &bal) == 3) {
        if (id == acc.accountID) {
            fprintf(temp, "%d %s %.2f\n", id, pass, acc.balance);
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", id, pass, bal);
        }
    }

    fclose(fp);
    if (!found) {
        // Default password for new accounts
        fprintf(temp, "%d %s %.2f\n", acc.accountID, "default", acc.balance);
    }

    fclose(temp);
    rename("temp.txt", ACC_FILE);
}


int get_next_account_id() {
    FILE *fp = fopen(ACC_FILE, "r");
    if (!fp)
        return 1001; // First account if file is empty

    int id;
    char pass[50];
    double bal;
    int lastID = 1000;

    while (fscanf(fp, "%d %s %lf", &id, pass, &bal) == 3) {
        if (id > lastID)
            lastID = id;
    }

    fclose(fp);
    return lastID + 1;
}

// ================= SESSION LOCK FUNCTIONS =================

int is_account_active(int accountID) {
    FILE *fp = fopen("active_sessions.txt", "r");
    if (!fp) return 0; // file does not exist yet → no one is active

    int id;
    while (fscanf(fp, "%d", &id) == 1) {
        if (id == accountID) {
            fclose(fp);
            return 1; // this account is already logged in somewhere
        }
    }

    fclose(fp);
    return 0;
}

void add_active_account(int accountID) {
    FILE *fp = fopen("active_sessions.txt", "a");
    fprintf(fp, "%d\n", accountID);
    fclose(fp);
}

void remove_active_account(int accountID) {
    FILE *fp = fopen("active_sessions.txt", "r");
    FILE *tmp = fopen("temp_active.txt", "w");

    int id;
    while (fscanf(fp, "%d", &id) == 1) {
        if (id != accountID) {
            fprintf(tmp, "%d\n", id);
        }
    }

    fclose(fp);
    fclose(tmp);
    rename("temp_active.txt", "active_sessions.txt");
}


// append to transaction log
void log_transaction(int id, const char *type, double amount) {
    FILE *fp = fopen(TXN_FILE, "a");
    if (!fp) return;
    fprintf(fp, "Account %d: %s ₹%.2f\n", id, type, amount);
    fclose(fp);
}
int verify_account(int accountID, const char *password); 
void show_transactions(int accountID, int connFD) {
    FILE *fp = fopen(TXN_FILE, "r");
    if (!fp) {
        write(connFD, RED "No transactions found.\n" RESET, strlen(RED "No transactions found.\n" RESET));
        return;
    }

    char line[256];
    int found = 0;

    write(connFD,
        CLEAR
        CYAN "========== TRANSACTION HISTORY ==========\n" RESET,
        strlen(CYAN "========== TRANSACTION HISTORY ==========\n" RESET));

    while (fgets(line, sizeof(line), fp)) {
        // Only show transactions matching this account ID
        char prefix[32];
        snprintf(prefix, sizeof(prefix), "Account %d:", accountID);

        if (strncmp(line, prefix, strlen(prefix)) == 0) {
            write(connFD, line, strlen(line));
            found = 1;
        }
    }

    if (!found) {
        write(connFD, YELLOW "No transactions recorded yet.\n" RESET, strlen(YELLOW "No transactions recorded yet.\n" RESET));
    }

    write(connFD,
        CYAN "=========================================\n" RESET,
        strlen(CYAN "=========================================\n" RESET));

    fclose(fp);
}
void clean_input(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
}

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
    clean_input(enteredPassword);   

    // Step 3: Verify account
    int verified = verify_account(accountID, enteredPassword);
if (!verified) {
   write(connFD, RED "✖ Invalid Account or Password!\n" RESET, strlen(RED "✖ Invalid Account or Password!\n" RESET));

    return;
}

// ✅ NEW: prevent same account from logging into another terminal
if (is_account_active(accountID)) {
    write(connFD, "ACCOUNT_ALREADY_ACTIVE\n", strlen("ACCOUNT_ALREADY_ACTIVE\n"));
    return;
}

// ✅ Mark account as active now
add_active_account(accountID);

   write(connFD, CLEAR, strlen(CLEAR));
write(connFD,
    CYAN "=========================================\n"
    YELLOW "         WELCOME TO YOUR ACCOUNT         \n"
    CYAN "=========================================\n"
    GREEN "✔ Login Successful!\n"
    RESET,
strlen(
    CYAN "=========================================\n"
    YELLOW "         WELCOME TO YOUR ACCOUNT         \n"
    CYAN "=========================================\n"
    GREEN "✔ Login Successful!\n"
    RESET
));


    // Step 4: Menu loop
    while (1) {
        const char *menu =
    "\n"
    CYAN "=========================================\n" RESET
    YELLOW "             CUSTOMER MENU               \n" RESET
    CYAN "=========================================\n" RESET
    GREEN "[1]" RESET " View Balance\n"
    GREEN "[2]" RESET " Deposit\n"
    GREEN "[3]" RESET " Withdraw\n"
    GREEN "[4]" RESET " Logout\n"
    GREEN "[5]" RESET " Apply for Loan\n"
    GREEN "[6]" RESET " Give Feedback\n"
    GREEN "[7]" RESET " Transfer Funds\n"
    GREEN "[8]" RESET " View Transaction History\n"
    CYAN "-----------------------------------------\n" RESET
    BLUE "Enter choice: " RESET;

        write(connFD, menu, strlen(menu));

        memset(buf, 0, sizeof(buf));
        if (read(connFD, buf, sizeof(buf)) <= 0)
            break;
        choice = atoi(buf);

        Account acc = read_account(accountID);
        char msg[256];

        switch (choice) {
            case 1:
                snprintf(msg, sizeof(msg),
    CLEAR
    CYAN "=========================================\n" RESET
    YELLOW "              ACCOUNT BALANCE            \n" RESET
    CYAN "=========================================\n" RESET
    WHITE "Account ID: %d\n" RESET
    GREEN "Available Balance: ₹%.2f\n" RESET
    CYAN "-----------------------------------------\n" RESET,
    accountID, acc.balance);

                write(connFD, msg, strlen(msg));
                break;

            case 2:
                write(connFD,
    CLEAR
    CYAN "======== DEPOSIT MONEY ========\n" RESET
    BLUE "Enter amount to deposit: " RESET,
strlen(
    CYAN "======== DEPOSIT MONEY ========\n" RESET
    BLUE "Enter amount to deposit: " RESET
));

                memset(buf, 0, sizeof(buf));
                read(connFD, buf, sizeof(buf));
                amt = atof(buf);

                acc.balance += amt;
                write_account(acc);
                log_transaction(accountID, "Deposit", amt);

                snprintf(msg, sizeof(msg), GREEN "✔ Deposited ₹%.2f\nNew Balance: ₹%.2f\n" RESET, amt, acc.balance);
write(connFD, msg, strlen(msg));

                break;

            case 3:
                write(connFD,
    CLEAR
    CYAN "======== WITHDRAW MONEY ========\n" RESET
    BLUE "Enter amount to withdraw: " RESET,
strlen(
    CYAN "======== WITHDRAW MONEY ========\n" RESET
    BLUE "Enter amount to withdraw: " RESET
));

                memset(buf, 0, sizeof(buf));
                read(connFD, buf, sizeof(buf));
                amt = atof(buf);

                if (amt > acc.balance)
                    write(connFD, RED "✖ Insufficient funds!\n" RESET, strlen(RED "✖ Insufficient funds!\n" RESET));

                else {
                    acc.balance -= amt;
                    write_account(acc);
                    log_transaction(accountID, "Withdraw", amt);
                    snprintf(msg, sizeof(msg), GREEN "✔ Withdrawn ₹%.2f\nNew Balance: ₹%.2f\n" RESET, amt, acc.balance);
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

                snprintf(msg, sizeof(msg), GREEN "✔ Loan request submitted successfully!\nLoan ID: %d\n" RESET, ln.loanID);
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
    remove_active_account(accountID); // ✅ free the lock
    write(connFD, YELLOW "⚠ Logging out...\n" RESET, strlen(YELLOW "⚠ Logging out...\n" RESET));
write(connFD, "SESSION_END\n", strlen("SESSION_END\n"));

    return;
   break;

   case 7: {
    int targetID;
    double amount;

    const char *transferPrompt =
        CLEAR
        CYAN "======== FUND TRANSFER ========\n" RESET
        BLUE "Enter Target Account ID:\n> " RESET;
    write(connFD, transferPrompt, strlen(transferPrompt));

    memset(buf, 0, sizeof(buf));
    read(connFD, buf, sizeof(buf));
    clean_input(buf);
    targetID = atoi(buf);

    // Prevent self-transfer
    if (targetID == accountID) {
        const char *err = RED "✖ Cannot transfer to the same account.\n" RESET;
        write(connFD, err, strlen(err));
        break;
    }

    Account targetAcc = read_account(targetID);

    if (targetAcc.accountID != targetID) {
        const char *err = RED "✖ Target account not found.\n" RESET;
        write(connFD, err, strlen(err));
        break;
    }

    const char *amountPrompt =
        BLUE "Enter amount to transfer:\n> " RESET;
    write(connFD, amountPrompt, strlen(amountPrompt));

    memset(buf, 0, sizeof(buf));
    read(connFD, buf, sizeof(buf));
    clean_input(buf);
    amount = atof(buf);

    // Check balance
    if (amount <= 0) {
        const char *err = RED "✖ Invalid amount!\n" RESET;
        write(connFD, err, strlen(err));
        break;
    }

    if (amount > acc.balance) {
        const char *err = RED "✖ Insufficient balance!\n" RESET;
        write(connFD, err, strlen(err));
        break;
    }

    // Perform Transfer
    acc.balance -= amount;
    targetAcc.balance += amount;

    write_account(acc);
    write_account(targetAcc);

    log_transaction(accountID, "Transfer Out", amount);
    log_transaction(targetID, "Transfer In", amount);

    snprintf(msg, sizeof(msg),
             GREEN "✔ Transferred ₹%.2f to Account %d\n"
             "New Balance: ₹%.2f\n" RESET,
             amount, targetID, acc.balance);
    write(connFD, msg, strlen(msg));

    break;
}

case 8:
    show_transactions(accountID, connFD);
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

    /*
       Try id password balance
    */
    while (fscanf(fp, "%d %s %lf", &id, pass, &balance) == 3) {
        if (id == accountID && strcmp(pass, password) == 0 ) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}