#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/structures.h"
#include "headers/date.h"

void create_items(char *path)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Unable to create file!\n");
        exit(2);
    }
    fputs("ID      Product type        Product name       Entry Date        In store    Price   ", fp);
    fputs("\n", fp);
    fclose(fp);
}

struct item *readItems(char *path, struct item *head, struct system_user *main_user, int print)
{
    int day, month, year, hours, mins;
    int id;
    float price;
    char product_type[20], product_name[20], in_store[12], temp[82];
    struct item *tmp_head = NULL;

    FILE *fp = fopen(path, "r");
    while (!fp)
    {
        fclose(fp);
        create_items(ITEMS_PATH);
        printf("File 'items.txt' was created. \n");
        //LOG INFO
        FILE *log = fopen(LOG_PATH, "a");
        getDateTime(&day, &month, &year, &hours, &mins);
        fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s created 'items.txt'.\n\n", day, month, year, hours, mins, main_user->fullname);
        fclose(log);
        return tmp_head;
    }
    fgets(temp, 85, fp);
    if (print == 1)
    {
        puts(temp);
    }
    while (fscanf(fp, "%d %20s %20s %2d/%2d/%d, %02d:%02d %12s %f\n", &id, product_type, product_name, &day, &month, &year, &hours, &mins, in_store, &price) != EOF)
    {
        struct item *tmp = (struct item *)malloc(sizeof(struct item));

        struct item *tmp_head2 = head;
        if (print == 1)
        {
            printf("%-7d %-19s %-18s %-2d/%-d/%-4d,%-2d:%-2d   %-11s %-10.2f\n", id, product_type, product_name, day, month, year, hours, mins, in_store, price);
        }
        tmp->id = id;
        strcpy(tmp->product_type, product_type);
        strcpy(tmp->product_name, product_name);
        strcpy(tmp->date, "date");
        strcpy(tmp->in_store, in_store);
        tmp->day = day;
        tmp->month = month;
        tmp->year = year;
        tmp->hours = hours;
        tmp->mins = mins;
        tmp->price = price;
        tmp->next = NULL;
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
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s printed the list of items.\n", day, month, year, hours, mins, main_user->fullname);
    fclose(log);
    return tmp_head;
}

struct item *add_new_item(char *path, struct item *head, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    float price;
    char product_type[20], product_name[20], date[18] = "date", in_store[12];
    struct item *tmp = (struct item *)malloc(sizeof(struct item));
    struct item *tmp_head = (struct item *)malloc(sizeof(struct item));
    tmp_head = head;
    printf("Enter product type: ");
    scanf(" %[^\n]%*c", product_type);
    printf("Enter product name: ");
    scanf(" %[^\n]%*c", product_name);
    printf("Product in store? ");
    scanf(" %[^\n]%*c", in_store);
    printf("Enter product price:");
    scanf(" %f", &price);

    FILE *fp = fopen(path, "a");

    if (tmp_head == NULL)
    {
        tmp->id = 1;
        strcpy(tmp->product_type, product_type);
        strcpy(tmp->product_name, product_name);
        strcpy(tmp->date, date);
        strcpy(tmp->in_store, in_store);
        tmp->price = price;
        tmp->next = NULL;
        tmp->prev = NULL;
        tmp_head = tmp;
        getDateTime(&day, &month, &year, &hours, &mins);
        tmp->day = day;
        tmp->month = month;
        tmp->year = year;
        tmp->hours = hours;
        tmp->mins = mins;
        fprintf(fp, "%-7d %-19s %-18s %02d/%02d/%d,%02d:%02d  %-11s %-7.2f \n", tmp->id, product_type, product_name, day, month, year, hours, mins, in_store, price);
        //LOG INFO
        FILE *log = fopen(LOG_PATH, "a");
        getDateTime(&day, &month, &year, &hours, &mins);
        fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s added new item - %s / %s / %.2f.\n", day, month, year, hours, mins, main_user->fullname, product_type, product_name, price);
        fclose(log);
    }
    else
    {
        while (tmp_head->next != NULL)
        {
            tmp_head = tmp_head->next;
        }
        tmp->id = tmp_head->id + 1;
        strcpy(tmp->product_type, product_type);
        strcpy(tmp->product_name, product_name);
        strcpy(tmp->date, date);
        strcpy(tmp->in_store, in_store);
        tmp->price = price;
        tmp->next = NULL;
        tmp->prev = tmp_head;
        tmp_head->next = tmp;
        getDateTime(&day, &month, &year, &hours, &mins);
        tmp->day = day;
        tmp->month = month;
        tmp->year = year;
        tmp->hours = hours;
        tmp->mins = mins;
        fprintf(fp, "%-7d %-19s %-18s %02d/%02d/%d,%02d:%02d  %-11s %-7.2f \n", tmp->id, product_type, product_name, day, month, year, hours, mins, in_store, price);
        //LOG INFO
        FILE *log = fopen(LOG_PATH, "a");
        getDateTime(&day, &month, &year, &hours, &mins);
        fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s added new item - %s / %s / %.2f.\n", day, month, year, hours, mins, main_user->fullname, product_type, product_name, price);
        fclose(log);
        while (tmp_head->prev != NULL)
        {
            tmp_head = tmp_head->prev;
        }
    }
    fclose(fp);
    return tmp_head;
}

struct item *delete_item(char *path, struct item *head, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    char product_type[20] = "Product type", product_name[20] = "Product name", date[18] = "Entry Date", in_store[12] = "In store", id[8] = "ID", price[7] = "Price";
    int i;
    head = readItems(path, head, main_user, 0);
    printf("What item to delete? Choose ID: \n");
    scanf("%d", &i);
    //Deleting for Linked List
    struct item *tmp = (struct item *)malloc(sizeof(struct item));
    struct item *tmp_head = (struct item *)malloc(sizeof(struct item));
    tmp = head;
    tmp_head = head;

    while (tmp->id != i)
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
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s deleted item - %s / %s / %.2f.\n", day, month, year, hours, mins, main_user->fullname, tmp->product_type, tmp->product_name, tmp->price);
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
    fputs(id, fp);
    fputs("      ", fp);
    fputs(product_type, fp);
    fputs("        ", fp);
    fputs(product_name, fp);
    fputs("       ", fp);
    fputs(date, fp);
    fputs("        ", fp);
    fputs(in_store, fp);
    fputs("    ", fp);
    fputs(price, fp);
    fputs("\n", fp);
    getDateTime(&day, &month, &year, &hours, &mins);
    while (tmp != NULL)
    {
        fprintf(fp, "%-7d %-19s %-18s %-2d/%-d/%-4d,%-2d:%-2d  %-11s %-10.2f\n", tmp->id, tmp->product_type, tmp->product_name, tmp->day, tmp->month, tmp->year, tmp->hours, tmp->mins, tmp->in_store, tmp->price);
        tmp = tmp->next;
    }
    fclose(fp);
    free(tmp);
    return tmp_head;
}

struct item *update_item(char *path, struct item *head, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    char product_type[20] = "Product type", product_name[20] = "Product name", date[18] = "Entry Date", in_store[12] = "In store", id[8] = "ID", price[7] = "Price";
    int i;
    head = readItems(path, head, main_user, 0);
    printf("What item you wish to update? Choose ID: \n");
    scanf(" %d", &i);

    struct item *tmp = (struct item *)malloc(sizeof(struct item));
    struct item *tmp_head = (struct item *)malloc(sizeof(struct item));
    tmp = head;
    tmp_head = head;

    while (tmp->id != i)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            printf("There is no such ID");
            return tmp_head;
        }
    }
    printf("Enter product type: ");
    scanf(" %[^\n]%*c", tmp->product_type);
    printf("Enter product name: ");
    scanf(" %[^\n]%*c", tmp->product_name);
    printf("Product in store? ");
    scanf(" %[^\n]%*c", tmp->in_store);
    printf("Enter product price:");
    scanf("%f", &tmp->price);

    getDateTime(&day, &month, &year, &hours, &mins);

    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s updated item %d to %s / %s / %.2f.\n", tmp->day, tmp->month, tmp->year, tmp->hours, tmp->mins, main_user->fullname, tmp->id, tmp->product_type, tmp->product_name, tmp->price);
    fclose(log);

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
    fputs(id, fp);
    fputs("      ", fp);
    fputs(product_type, fp);
    fputs("        ", fp);
    fputs(product_name, fp);
    fputs("       ", fp);
    fputs(date, fp);
    fputs("        ", fp);
    fputs(in_store, fp);
    fputs("    ", fp);
    fputs(price, fp);
    fputs("\n", fp);
    getDateTime(&day, &month, &year, &hours, &mins);
    while (tmp != NULL)
    {
        fprintf(fp, "%-7d %-19s %-18s %-2d/%-d/%-4d,%-2d:%-2d   %-11s %-10.2f\n", tmp->id, tmp->product_type, tmp->product_name, tmp->day, tmp->month, tmp->year, tmp->hours, tmp->mins, tmp->in_store, tmp->price);
        tmp = tmp->next;
    }
    fclose(fp);
    free(tmp);
    return tmp_head;
}

struct item *sort_by_product_type(char *path, struct item *head, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    int id;
    float price;
    char product_type[20], product_name[20], in_store[12], temp[82], search[20];
    struct item *tmp_head = NULL;

    printf("Enter product type or name: ");
    scanf(" %[^\n]%*c", search);

    FILE *fp = fopen(path, "r");
    while (!fp)
    {
        printf("File not found!\n");
    }
    fgets(temp, 85, fp);
    puts(temp);

    while (fscanf(fp, "%d %20s %20s %2d/%2d/%d, %02d:%02d %12s %f\n", &id, product_type, product_name, &day, &month, &year, &hours, &mins, in_store, &price) != EOF)
    {
        struct item *tmp = (struct item *)malloc(sizeof(struct item));

        struct item *tmp_head2 = head;
        if (strcmp(search, product_type) == 0 || strcmp(search, product_name) == 0)
        {
            printf("%-7d %-19s %-18s %-2d/%-d/%-4d,%-2d:%-2d   %-11s %-10.2f\n", id, product_type, product_name, day, month, year, hours, mins, in_store, price);
        }
        tmp->id = id;
        strcpy(tmp->product_type, product_type);
        strcpy(tmp->product_name, product_name);
        strcpy(tmp->date, "date");
        strcpy(tmp->in_store, in_store);
        tmp->day = day;
        tmp->month = month;
        tmp->year = year;
        tmp->hours = hours;
        tmp->mins = mins;
        tmp->price = price;
        tmp->next = NULL;
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
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s printed sorted list of items .\n", day, month, year, hours, mins, main_user->fullname);
    fclose(log);
    return tmp_head;
}

struct item *sort_by_in_store(char *path, struct item *head, struct system_user *main_user)
{
    int day, month, year, hours, mins;
    int id;
    float price;
    char product_type[20], product_name[20], in_store[12], temp[82], search[12];
    struct item *tmp_head = NULL;

    printf("Enter 'yes' or 'no': ");
    scanf(" %[^\n]%*c", search);

    FILE *fp = fopen(path, "r");
    while (!fp)
    {
        printf("File not found!\n");
    }
    fgets(temp, 85, fp);
    puts(temp);

    while (fscanf(fp, "%d %20s %20s %2d/%2d/%d, %02d:%02d %12s %f\n", &id, product_type, product_name, &day, &month, &year, &hours, &mins, in_store, &price) != EOF)
    {
        struct item *tmp = (struct item *)malloc(sizeof(struct item));

        struct item *tmp_head2 = head;
        if (strcmp(search, in_store) == 0)
        {
            printf("%-7d %-19s %-18s %-2d/%-d/%-4d,%-2d:%-2d   %-11s %-10.2f\n", id, product_type, product_name, day, month, year, hours, mins, in_store, price);
        }
        tmp->id = id;
        strcpy(tmp->product_type, product_type);
        strcpy(tmp->product_name, product_name);
        strcpy(tmp->date, "date");
        strcpy(tmp->in_store, in_store);
        tmp->day = day;
        tmp->month = month;
        tmp->year = year;
        tmp->hours = hours;
        tmp->mins = mins;
        tmp->price = price;
        tmp->next = NULL;
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
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s printed sorted list of items .\n", day, month, year, hours, mins, main_user->fullname);
    fclose(log);
    return tmp_head;
}

void free_items(struct item *head)
{
    struct item *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}