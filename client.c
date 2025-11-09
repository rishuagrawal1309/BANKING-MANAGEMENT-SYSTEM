#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define CLEAR   "\033[2J\033[1;1H"


int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[2000];

    printf(CLEAR);
    printf(CYAN "============================================\n" RESET);
    printf(YELLOW "        WELCOME TO BANKING SYSTEM CLIENT     \n" RESET);
    printf(CYAN "============================================\n\n" RESET);
    printf(BLUE "Connecting to server...\n" RESET);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf(RED "✖ Failed to create socket.\n" RESET);
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf(RED "✖ Connection to server failed.\n" RESET);
        exit(1);
    }

    printf(GREEN "✔ Connected to Banking Server!\n\n" RESET);

    while (1) {
    memset(buffer, 0, sizeof(buffer));
    int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (read_size <= 0) {
            printf(RED "\n✖ Server disconnected.\n" RESET);
            break;
        }

        buffer[read_size] = '\0';

        // Display server output with UI
        if (strstr(buffer, "LOGIN"))
            printf(YELLOW "%s" RESET, buffer);
        else
            printf("%s", buffer);

        fflush(stdout);

    // ✅ Detect if same account is already logged in
if (strstr(buffer, "ACCOUNT_ALREADY_ACTIVE")) {
    printf("⚠️  This account is already logged in from another device.\n");
    printf("Please logout from the other session before logging in.\n");
    close(sock);
    return 0;  // Exit client
}


    // Detect input prompts
    if ( strstr(buffer, "ACCOUNT_ID") ||
     strstr(buffer, "PASSWORD") ||
     strstr(buffer, "choice") ||
     strstr(buffer, "Target Account ID") ||
     strstr(buffer, "amount") ||
     strstr(buffer, "loan") ||
     strstr(buffer, "feedback") ) {

         printf(CYAN "> " RESET);
            char input[100];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            strcat(input, "\n");
            send(sock, input, strlen(input), 0);
    }

    // Proper exit only on true logout confirmation
    if (strstr(buffer, "SESSION_END")) {
            printf(GREEN "\n✔ Logged out successfully.\n" RESET);
            break;
}
    }
    close(sock);
    return 0;
}
