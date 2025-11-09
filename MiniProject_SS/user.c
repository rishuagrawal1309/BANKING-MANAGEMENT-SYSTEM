#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

#define USERS_FILE "users.txt"

// Structure for default users
User default_users[] = {
    {1, "admin", "admin", 1},
    {2, "manager", "manager", 2},
    {3, "employee", "employee", 3},
    {4, "customer", "customer", 4}
};
int default_user_count = 4;

// Check if user file exists and has data
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) { fclose(file); return 1; }
    return 0;
}

// Register default users if file not present
void register_default_users() {
    if (file_exists(USERS_FILE)) return;

    FILE *file = fopen(USERS_FILE, "w");
    if (!file) {
        perror("Error creating users.txt");
        return;
    }

    for (int i = 0; i < default_user_count; i++) {
        fprintf(file, "%d %s %s %d\n", 
                default_users[i].id,
                default_users[i].username,
                default_users[i].password,
                default_users[i].role);
    }

    fclose(file);
    printf("Default users registered successfully.\n");
}

// Add new user (used by employee/admin)
void add_user(const char *username, const char *password, int role) {
    FILE *file = fopen(USERS_FILE, "a");
    if (!file) {
        perror("Error opening users.txt");
        return;
    }

    // Generate new ID
    int id = rand() % 10000;
    fprintf(file, "%d %s %s %d\n", id, username, password, role);
    fclose(file);
    printf("New user '%s' added successfully.\n", username);
}

// Login validation
int login(User *u) {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        perror("Error opening users.txt");
        return 0;
    }

    char username[50], password[50];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    while (fscanf(file, "%d %s %s %d", &u->id, u->username, u->password, &u->role) == 4) {
        if (strcmp(username, u->username) == 0 && strcmp(password, u->password) == 0) {
            fclose(file);
            return 1; // success
        }
    }

    fclose(file);
    printf("Invalid username or password.\n");
    return 0;
}
