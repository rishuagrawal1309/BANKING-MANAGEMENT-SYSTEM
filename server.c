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

void *connection_handler(void *socket_desc);

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
    printf("Server started on port 8080...\n");

    while ((client_fd = accept(server_fd, (struct sockaddr *)&client, &c))) {
        printf("Connection accepted\n");
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

    printf("\nNew client connected.\n");

    // Register defaults if not present
    register_default_users();

    // Login
    if (!login(&loggedInUser)) {
        printf("Login failed.\n");
        close(sock);
        return 0;
    }

    printf("Login success: %s (Role %d)\n", loggedInUser.username, loggedInUser.role);

    switch (loggedInUser.role) {
        case 1: admin_menu(sock); break;
        case 2: manager_menu(sock); break;
        case 3: employee_menu(sock); break;
        case 4: 
         printf("Customer login detected. Handing over to customer menu...\n");
    customer_menu(sock);
    break;

        default: printf("Unknown role.\n");
    }

    close(sock);
    return 0;
}

