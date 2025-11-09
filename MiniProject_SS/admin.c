#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"

void admin_menu(int connFD) {
    int choice;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Manager\n");
        printf("2. Add Employee\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: admin_add_manager(); break;
            case 2: admin_add_employee(); break;
            case 3: admin_logout(connFD); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

void admin_add_manager() {
    printf("[Admin] Manager added successfully!\n");
}

void admin_add_employee() {
    printf("[Admin] Employee added successfully!\n");
}

void admin_logout(int connFD) {
    printf("Admin logged out. (connFD=%d)\n", connFD);
}
