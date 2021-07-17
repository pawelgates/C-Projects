
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "headers/structures.h"
#include "headers/date.h"
#include "headers/item.h"
#include "headers/user.h"

int main()
{
    int day, month, year, hours, mins;
    int access, lvl;
    int i = 100;
    struct system_user *main_user = (struct system_user *)malloc(sizeof(struct system_user));
    struct item *head = NULL;
    struct system_user *head_of_users = NULL;
    //User entry to the system
    access = system_enter(USERS_PATH, &main_user);
    if (access == 0)
    {
        printf("You are blocked forever!\n");
        exit(1);
    }
    printf("Welcome back, %s! Choose your actions:\n", main_user->fullname);
    lvl = main_user->lvl;
    while (i != 0)
    {
        if (lvl == 1)
        {
            printf("1. List of items // 2. Add item // ");
            printf("3. Sort by product type or name // 4. Sort by availability in store // 0. EXIT \n");
            scanf("%d", &i);
            switch (i)
            {
            case 1:
                head = readItems(ITEMS_PATH, head, main_user, 1);
                break;

            case 2:
                head = readItems(ITEMS_PATH, head, main_user, 0);
                head = add_new_item(ITEMS_PATH, head, main_user);
                break;
            case 3:
                head = sort_by_product_type(ITEMS_PATH, head, main_user);
                break;
            case 4:
                head = sort_by_in_store(ITEMS_PATH, head, main_user);
                break;
            }
        }
        if (lvl == 2)
        {
            printf("1. List of items // 2. Add item // 3. Update item // 4. Delete item \n");
            printf("5. Sort by product type or name // 6. Sort by availability in store // 0. EXIT\n");
            scanf("%d", &i);
            switch (i)
            {
            case 1:
                head = readItems(ITEMS_PATH, head, main_user, 1);
                break;

            case 2:
                head = readItems(ITEMS_PATH, head, main_user, 0);
                head = add_new_item(ITEMS_PATH, head, main_user);
                break;
            case 3:
                head = update_item(ITEMS_PATH, head, main_user);
                break;
            case 4:
                head = delete_item(ITEMS_PATH, head, main_user);
                break;
            case 5:
                head = sort_by_product_type(ITEMS_PATH, head, main_user);
                break;
            case 6:
                head = sort_by_in_store(ITEMS_PATH, head, main_user);
                break;
            }
        }
        if (lvl == 3)
        {
            printf("1. List of items // 2. Add item // 3. Update item // 4. Delete item \n");
            printf("5. List of users // 6. Add user // 7. Update user // 8. Delete user \n");
            printf("9. Sort by product type or name // 10. Sort by availability in store // 0. EXIT\n");
            scanf("%d", &i);
            switch (i)
            {
            case 1:
                head = readItems(ITEMS_PATH, head, main_user, 1);
                break;

            case 2:
                head = readItems(ITEMS_PATH, head, main_user, 0);
                head = add_new_item(ITEMS_PATH, head, main_user);
                break;
            case 3:
                head = update_item(ITEMS_PATH, head, main_user);
                break;
            case 4:
                head = delete_item(ITEMS_PATH, head, main_user);
                break;
            case 5:
                head_of_users = read_users(USERS_PATH, head_of_users, main_user);
                break;
            case 6:
                head_of_users = read_users(USERS_PATH, head_of_users, main_user);
                head_of_users = add_new_user(USERS_PATH, head_of_users, main_user);
                break;
            case 7:
                head_of_users = update_user(USERS_PATH, head_of_users, main_user);
                break;
            case 8:
                head_of_users = delete_user(USERS_PATH, head_of_users, main_user);
                break;
            case 9:
                head = sort_by_product_type(ITEMS_PATH, head, main_user);
                break;
            case 10:
                head = sort_by_in_store(ITEMS_PATH, head, main_user);
                break;
            }
        }
        printf("\n");
    }
    //LOG INFO
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log, "%02d/%02d/%d, %02d:%02d : User %s loged out from the system.\n\n", day, month, year, hours, mins, main_user->fullname);
    fclose(log);

    //FREE MEMORY
    free(main_user);
    free_items(head);
    free_system_user(head_of_users);
    return 0;
}
