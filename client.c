#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[2000];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to the Banking Server!\n");

    while (1) {
    memset(buffer, 0, sizeof(buffer));
    int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (read_size <= 0) {
        if (read_size == 0)
            printf("\nServer closed the connection.\n");
        else
            perror("recv");
        break;
    }

    buffer[read_size] = '\0';
    printf("%s", buffer);
    fflush(stdout);

    // Detect input prompts
    if (strstr(buffer, "Username") ||
        strstr(buffer, "Password") ||
        strstr(buffer, "Enter role number") ||
        strstr(buffer, "ENTER_ACCOUNT_ID") ||
        strstr(buffer, "ENTER_ACCOUNT_PASSWORD") ||
        strstr(buffer, "Enter choice") ||
        strstr(buffer, "Enter amount") ||
        strstr(buffer, "Enter loan") ||
        strstr(buffer, "Enter your feedback")) {

        char input[100];
        printf("> ");
        fflush(stdout);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        send(sock, input, strlen(input), 0);
    }

    // Proper exit only on true logout confirmation
    if (strstr(buffer, "SESSION_END\n")) {
        printf("Logged out successfully.\n");
        break;
    }
}

    close(sock);
    return 0;
}
