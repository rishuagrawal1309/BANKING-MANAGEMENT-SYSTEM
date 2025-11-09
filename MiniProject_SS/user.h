#ifndef USER_H
#define USER_H

typedef struct {
    int id;
    char username[50];
    char password[50];
    int role;  // 1=admin, 2=manager, 3=employee, 4=customer
} User;

void register_default_users();
void add_user(const char *username, const char *password, int role);
int login(User *u);

#endif
