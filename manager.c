#include <stdio.h>
#include <stdlib.h>
#include "manager.h"
#include "loan.h"
#include "feedback.h"
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define WHITE   "\033[1;37m"
#define CLEAR   "\033[2J\033[1;1H"


void loan_approval_ui();
void manager_menu(int connFD) {
    int choice;

    do {
        printf(CLEAR);
        printf(CYAN "===============================================\n" RESET);
        printf(YELLOW "                 MANAGER PORTAL                \n" RESET);
        printf(CYAN "===============================================\n" RESET);
        printf(GREEN "[1]" RESET " View All Loans\n");
        printf(GREEN "[2]" RESET " Loan Approval Panel\n");
        printf(GREEN "[3]" RESET " View Feedback\n");
        printf(GREEN "[4]" RESET " Reply to Feedback\n");
        printf(GREEN "[5]" RESET " Logout\n");
        printf(CYAN "-----------------------------------------------\n" RESET);
        printf(BLUE "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                printf(CLEAR);
                printf(YELLOW "📌 ALL LOANS:\n\n" RESET);
                list_loans();
                printf("\n" CYAN "Press Enter to continue..." RESET);
                getchar(); getchar();
                break;

            case 2:
                loan_approval_ui();
                break;

            case 3:
                printf(CLEAR);
                printf(YELLOW "📌 FEEDBACK LIST:\n\n" RESET);
                list_feedbacks();
                printf("\n" CYAN "Press Enter to continue..." RESET);
                getchar(); getchar();
                break;

            case 4: {
                int id;
                char reply[100];
                printf("\nEnter Feedback ID: ");
                scanf("%d", &id);
                printf("Enter reply: ");
                scanf("%s", reply);
                add_reply(id, reply);
                printf(GREEN "✔ Reply saved!\n" RESET);
                sleep(1);
                break;
            }

            case 5:
                printf(YELLOW "⚠ Logging out...\n" RESET);
                sleep(1);
                return;

            default:
                printf(RED "✖ Invalid choice. Try again.\n" RESET);
                sleep(1);
        }
    } while(choice != 5);
}
