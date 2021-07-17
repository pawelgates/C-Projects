#define USERS_PATH "files-txt/users.txt"
#define ITEMS_PATH "files-txt/items.txt"
#define LOG_PATH "files-txt/log.txt"

struct system_user
{
    char username[16];
    char password[16];
    char fullname[21];
    int lvl;
    int id;
    struct system_user *next;
    struct system_user *prev;
};

struct item
{
    int id;
    int day;
    int month;
    int year;
    int hours;
    int mins;
    char product_type[20];
    char product_name[20];
    char date[18];
    float price;
    char in_store[12];
    struct item *next;
    struct item *prev;
};