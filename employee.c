#include <stdio.h>
#include <stdlib.h>
#include "employee.h"
#include "loan.h"
#include "user.h"
void employee_menu(int connFD) {
    int choice;
    do {
        printf("\n--- EMPLOYEE MENU ---\n");
        printf("1. View Assigned Loans\n");
        printf("2. Update Customer Data\n");
        printf("3. Logout\n");
        printf("4. View All Loans\n");
        printf("5. Mark Loan Processed\n");
        printf("6. Add New Customer\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("Assigned loans displayed.\n"); break;
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
    printf("Customer %s added!\n", new.username);
    break;

            default: printf("Invalid choice.\n");
        }
    } while (choice != 3);
}
