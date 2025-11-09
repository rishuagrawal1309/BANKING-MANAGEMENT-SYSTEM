#include <stdio.h>
#include <stdlib.h>
#include "employee.h"
#include "loan.h"
#include "user.h"
#include "account.h"
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

void employee_menu(int connFD) {
    int choice;
    do {
        printf("\n%s=========================================%s\n", CYAN, RESET);
printf("%s           EMPLOYEE  MENU              %s\n", YELLOW, RESET);
printf("%s=========================================%s\n", CYAN, RESET);
printf("%s[1]%s View Assigned Loans\n", GREEN, RESET);
printf("%s[2]%s Update Customer Data\n", GREEN, RESET);
printf("%s[3]%s Logout\n", GREEN, RESET);
printf("%s[4]%s View All Loans\n", GREEN, RESET);
printf("%s[5]%s Mark Loan Processed\n", GREEN, RESET);
printf("%s[6]%s Add New Customer\n", GREEN, RESET);
printf("%s-----------------------------------------%s\n", CYAN, RESET);
printf("%sEnter choice:%s ", BLUE, RESET);


        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
           case 1: {
    FILE *fp = fopen("loans.txt", "r");
    if (!fp) {
        printf("ERROR: loans.txt not found!\n");
        break;
    }

    int loanID, accountID;
    double amount;
    char reason[100], status[50];

    printf("\n--- LIST OF ALL LOANS ---\n");

    while (fscanf(fp, "%d %d %lf %s %s",
                  &loanID, &accountID, &amount, reason, status) == 5) {

        printf("\nLoan ID: %d\n", loanID);
        printf("Account ID: %d\n", accountID);
        printf("Amount: ₹%.2f\n", amount);
        printf("Reason: %s\n", reason);
        printf("Status: %s\n", status);
        printf("-----------------------------------\n");
    }

    fclose(fp);
    break;
}

            case 2: printf("Customer data updated.\n"); break;
            case 3: printf("Employee logged out.\n"); break;
            case 4:
               list_loans();
                break;
            case 5: 
                int id;
                printf("Enter Loan ID to mark as Processed: ");
                scanf("%d", &id);
                update_loan_status(id, "Processed");
                break;
            case 6: 
    User new;
    printf("Enter new username: ");
    scanf("%s", new.username);
    printf("Enter password: ");
    scanf("%s", new.password);
    new.role = 4;
    add_user(new.username, new.password, new.role);
    Account acc;
    acc.accountID = get_next_account_id();
    acc.balance = 0.0;

    FILE *fp = fopen("accounts.txt", "a");
    fprintf(fp, "%d %s %.2f\n", acc.accountID, new.password, acc.balance);
    fclose(fp);

    printf("%s✔ Customer added successfully!%s\n", GREEN, RESET);

    printf("Login Username : %s\n", new.username);
    printf("Assigned Account ID : %d\n", acc.accountID);
    printf("Account Password : %s\n", new.password);
    printf("Initial Balance : ₹%.2f\n\n", acc.balance);
    break;
            default: printf("%s✖ Invalid choice! Please try again.%s\n", RED, RESET);

        }
    } while (choice != 3);
}
