#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "admin.h"
#include "manager.h"
#include "employee.h"
#include "customer.h"
#include "user.h"
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define MAGENTA "\033[1;35m"

void *connection_handler(void *socket_desc);
void show_banner() {
    printf(CYAN "\n==============================================\n" RESET);
    printf(YELLOW "        BANKING MANAGEMENT SYSTEM SERVER       \n" RESET);
    printf(CYAN   "==============================================\n" RESET);
    printf(GREEN  "       Multi-Client Concurrent Server Running   \n" RESET);
    printf(CYAN   "----------------------------------------------\n\n" RESET);
}

int main() 
{
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Could not create socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 3);
    show_banner();
printf(GREEN "✔ Server started on port %d...\n\n" RESET, 8080);


    while ((client_fd = accept(server_fd, (struct sockaddr *)&client, &c))) {
        printf(YELLOW "↳ New Client Connected\n" RESET);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, connection_handler, (void *)&client_fd);
    }

    if (client_fd < 0) perror("Accept failed");
    return 0;
}

void *connection_handler(void *socket_desc) 
{
    int sock = *(int *)socket_desc;
    User loggedInUser;

    printf(CYAN "\n[+] Client session started.\n" RESET);

    register_default_users();

    if (!login(&loggedInUser)) {
        printf(RED "✖ Login failed. Closing connection...\n" RESET);
        close(sock);
        return 0;
    }

    printf(GREEN "✔ Login Successful!\n" RESET);
    printf(WHITE "User: %s\nRole: %d\n\n" RESET, loggedInUser.username, loggedInUser.role);

    switch (loggedInUser.role) {
        case 1:
            printf(BLUE "→ Switching to ADMIN interface...\n" RESET);
            admin_menu(sock);
            break;
        case 2:
            printf(BLUE "→ Switching to MANAGER interface...\n" RESET);
            manager_menu(sock);
            break;
        case 3:
            printf(BLUE "→ Switching to EMPLOYEE interface...\n" RESET);
            employee_menu(sock);
            break;
        case 4:
            printf(BLUE "→ Switching to CUSTOMER interface...\n" RESET);
            customer_menu(sock);
            break;
        default:
            printf(RED "✖ Unknown role. Cannot proceed.\n" RESET);
    }

    printf(YELLOW "↳ Client session closed.\n" RESET);
    close(sock);
    return 0;
}


