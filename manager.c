#include <stdio.h>
#include <stdlib.h>
#include "manager.h"
#include "loan.h"
#include "feedback.h"

void manager_menu(int connFD) {
    int choice;
    do {
        printf("\n--- MANAGER MENU ---\n");
        printf("1. Approve Loan\n");
        printf("2. View Reports\n");
        printf("3. Logout\n");
        printf("4. View All Loans\n");
        printf("5. Approve/Reject Loan\n");
        printf("6. View Feedback\n");
        printf("7. Reply to Feedback\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("Loan approved.\n"); break;
            case 2: printf("Report viewed.\n"); break;
            case 3: printf("Manager logged out.\n"); break;
            case 4:
               list_loans();
                break;
           case 5: 
    int id, ch;
    printf("Enter Loan ID: ");
    scanf("%d", &id);
    printf("1. Approve  2. Reject: ");
    scanf("%d", &ch);
    if (ch == 1) update_loan_status(id, "Approved");
    else update_loan_status(id, "Rejected");
    break;

          case 6:
    list_feedbacks();
    break;

case 7: 
    int id1;
    char reply[100];
    printf("Enter Feedback ID to reply: ");
    scanf("%d", &id1);
    printf("Enter reply (one word): ");
    scanf("%s", reply);
    add_reply(id1, reply);
    break;

            default: printf("Invalid choice.\n");
        }
    } while (choice != 3);
}
