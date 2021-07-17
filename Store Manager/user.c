#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/structures.h"
#include "headers/date.h"
#include "headers/item.h"

void create_users(char *path)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Unable to create file!\n");
        exit(2);
    }
    fputs("Username        Password        L Fullname             ", fp);
    fputs("\n", fp);
    fputs("admin           admin           3 System_Manager       ", fp);
    fputs("\n", fp);
    fclose(fp);
}

int system_enter(char *path, struct system_user **s_user)
{
    int day, month, year, hours, mins;
    int counter = 0, level;
    char username_enter[16], password_enter[16];
    char temp[100], username[16], password[16], fullname[21];
    struct system_user *tmp_user = (struct system_user *)malloc(sizeof(struct system_user));
    while (counter < 3)
    {
        printf("Enter your Username: \n");
        scanf("%[^\n]%*c", username_enter);
        printf("Enter your Password: \n");
        scanf("%[^\n]%*c", password_enter);

        FILE *fp = fopen(path, "r");
        if (!fp)
        {
            create_users(USERS_PATH);
            printf("File 'users.txt' was created. Enter once again please!\n");
            //LOG INFO
            FILE *log = fopen(LOG_PATH, "a");
            getDateTime(&day, &month, &year, &hours, &mins);
            fprintf(log, "%02d/%02d/%d, %02d:%02d : File 'users.txt' was created.\n\n", day, month, year, hours, mins);
            fclose(log);
        }
        else
        {
            fgets(temp, 100, fp);
            while (fscanf(fp, "%15s %15s %15d %20s", username, password, &level, fullname) != EOF)
            {
                if (strcmp(username_enter, username) == 0 && strcmp(password_enter, password) == 0)
                {
                    counter = 3;
                    strcpy(tmp_user->username, username);
                    strcpy(tmp_user->password, password);
                    strcpy(tmp_user->fullname, fullname);
                    tmp_user->lvl = level;
                    *s_user = tmp_user;
                    //LOG INFO
                    FILE *log = fopen(LOG_PATH, "a");
                    getDateTime(&day, &month, &year, &hours, &mins);
                    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s entered the system.\n", day, month, year, hours, mins, tmp_user->fullname);
                    fclose(log);
                    return 1;
                }
            }
            printf("Username is incorrect!\n");
            counter++;
        }
        fclose(fp);
    }
    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : Login fail.\n", day, month, year, hours, mins);
    fclose(log);
    return 0;
}

struct system_user *read_users(char *path, struct system_user *head_of_users, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    int level, id = 1;
    char temp[55], username[16], password[16], fullname[21];
    struct system_user *tmp_head = NULL;

    FILE *fp = fopen(path, "r");
    while (!fp)
    {
        printf("File not found!\n");
    }

    fgets(temp, 55, fp);
    printf("\n");
    puts(temp);
    printf("\n");
    while (fscanf(fp, "%15s %15s %d %20s", username, password, &level, fullname) != EOF)
    {
        struct system_user *tmp = (struct system_user *)malloc(sizeof(struct system_user));
        struct system_user *tmp_head2 = head_of_users;

        printf("%-15s %-15s %-1d %-19s \n", username, password, level, fullname);

        tmp->id = id;
        strcpy(tmp->username, username);
        strcpy(tmp->password, password);
        tmp->lvl = level;
        strcpy(tmp->fullname, fullname);
        tmp->next = NULL;
        id++;
        if (tmp_head == NULL)
        {
            tmp_head = tmp;
            tmp_head->prev = NULL;
        }
        else
        {
            while (tmp_head->next != NULL)
            {
                tmp_head = tmp_head->next;
            }
            tmp_head2 = tmp_head;
            tmp_head->next = tmp;
            tmp_head->next->prev = tmp_head2;
            while (tmp_head->prev != NULL)
            {
                tmp_head = tmp_head->prev;
            }
        }
    }
    fclose(fp);
    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s printed the list of users.\n", day, month, year, hours, mins, main_user->fullname);
    fclose(log);
    return tmp_head;
}

struct system_user *add_new_user(char *path, struct system_user *head_of_users, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    int level;
    char username[16], password[16], fullname[21];
    struct system_user *tmp = (struct system_user *)malloc(sizeof(struct system_user));
    struct system_user *tmp_head = (struct system_user *)malloc(sizeof(struct system_user));
    tmp_head = head_of_users;
    printf("Enter user name: ");
    scanf(" %[^\n]%*c", username);
    printf("Enter password: ");
    scanf(" %[^\n]%*c", password);
    printf("Enter full name: ");
    scanf(" %[^\n]%*c", fullname);
    printf("Enter access level:");
    scanf(" %d", &level);
    FILE *fp = fopen(path, "a");

    while (tmp_head->next != NULL)
    {
        tmp_head = tmp_head->next;
    }
    tmp->id = tmp_head->id + 1;
    strcpy(tmp->username, username);
    strcpy(tmp->password, password);
    strcpy(tmp->fullname, fullname);
    tmp->lvl = level;

    tmp->next = NULL;
    tmp->prev = tmp_head;
    tmp_head->next = tmp;
    fprintf(fp, "%-15s %-15s %-1d %-19s \n", username, password, level, fullname);
    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s added new user - %s with access level %d.\n", day, month, year, hours, mins, main_user->fullname, fullname, level);
    fclose(log);
    while (tmp_head->prev != NULL)
    {
        tmp_head = tmp_head->prev;
    }
    fclose(fp);
    return tmp_head;
}

struct system_user *delete_user(char *path, struct system_user *head_of_users, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    char username[16] = "username", password[16] = "password", level[2] = "L", fullname[21] = "fullname";
    head_of_users = read_users(path, head_of_users, main_user);
    struct system_user *tmp = (struct system_user *)malloc(sizeof(struct system_user));
    struct system_user *tmp_head = (struct system_user *)malloc(sizeof(struct system_user));
    tmp = head_of_users;
    tmp_head = head_of_users;

    while (tmp != NULL)
    {
        printf("User %d: %s, level of access is %d \n", tmp->id, tmp->fullname, tmp->lvl);
        tmp = tmp->next;
    }
    int user_num;
    printf("Enter user number: ");
    scanf(" %d", &user_num);
    while (user_num == 1)
    {
        printf("You cant delete 'admin' user \n");
        printf("Choose another user: ");
        scanf(" %d", &user_num);
    }
    tmp = head_of_users;
    while (tmp->id != user_num)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            printf("There is no such ID");
            return tmp_head;
        }
    }
    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s deleted user %s.\n", day, month, year, hours, mins, main_user->fullname, tmp->fullname);
    fclose(log);

    if (tmp->prev == NULL)
    {
        tmp_head = tmp->next;
        tmp_head->prev = NULL;
        return tmp_head;
    }
    tmp_head = tmp->prev;
    tmp_head->next = tmp->next;
    tmp_head->prev = tmp->prev->prev;
    while (tmp_head->prev != NULL)
    {
        tmp_head = tmp_head->prev;
    }
    //Sorting IDS
    tmp = tmp_head;
    int k = 1;
    while (tmp != NULL)
    {
        tmp->id = k;
        k++;
        tmp = tmp->next;
    }
    //Second part of correcting the file
    tmp = tmp_head;
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Unable to create file!\n");
        exit(2);
    }
    fputs(username, fp);
    fputs("        ", fp);
    fputs(password, fp);
    fputs("        ", fp);
    fputs(level, fp);
    fputs(" ", fp);
    fputs(fullname, fp);
    fputs("             ", fp);
    fputs("\n", fp);
    while (tmp != NULL)
    {
        fprintf(fp, "%-15s %-15s %-1d %-19s \n", tmp->username, tmp->password, tmp->lvl, tmp->fullname);
        tmp = tmp->next;
    }
    fclose(fp);
    free(tmp);

    return tmp_head;
}

struct system_user *update_user(char *path, struct system_user *head_of_users, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    head_of_users = read_users(path, head_of_users, main_user);
    char username[16] = "username", password[16] = "password", level[2] = "L", fullname[21] = "fullname";
    struct system_user *tmp = (struct system_user *)malloc(sizeof(struct system_user));
    struct system_user *tmp_head = (struct system_user *)malloc(sizeof(struct system_user));
    int i;

    tmp = head_of_users;
    tmp_head = head_of_users;
    while (tmp != NULL)
    {
        printf("User %d: %s, level of access is %d \n", tmp->id, tmp->fullname, tmp->lvl);
        tmp = tmp->next;
    }
    printf("What user you wish to update? Choose ID: \n");
    scanf(" %d", &i);

    tmp = head_of_users;
    while (tmp->id != i)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            printf("There is no such ID");
            return tmp_head;
        }
    }
    printf("Enter new username: ");
    scanf(" %[^\n]%*c", tmp->username);
    printf("Enter new password: ");
    scanf(" %[^\n]%*c", tmp->password);
    printf("Enter new full name: ");
    scanf(" %[^\n]%*c", tmp->fullname);
    printf("Enter the level of access:");
    scanf("%d", &tmp->lvl);
    //LOG INFO---
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s updated user %d - username: %s, password: %s, level: %d, fullname: %s .\n", day, month, year, hours, mins, main_user->fullname, tmp->id, tmp->username, tmp->password, tmp->lvl, tmp->fullname);
    fclose(log);
    //---
    while (tmp->prev != NULL)
    {
        tmp = tmp->prev;
    }
    //Second part of correcting the file
    tmp = tmp_head;
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Unable to create file!\n");
        exit(2);
    }
    fputs(username, fp);
    fputs("        ", fp);
    fputs(password, fp);
    fputs("        ", fp);
    fputs(level, fp);
    fputs(" ", fp);
    fputs(fullname, fp);
    fputs("             ", fp);
    fputs("\n", fp);
    while (tmp != NULL)
    {
        fprintf(fp, "%-15s %-15s %-1d %-19s \n", tmp->username, tmp->password, tmp->lvl, tmp->fullname);
        tmp = tmp->next;
    }
    fclose(fp);
    free(tmp);

    return tmp_head;
}

void free_system_user(struct system_user *head)
{
    struct system_user *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}