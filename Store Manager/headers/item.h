
void create_items(char *path);
struct item *readItems(char *path, struct item *head, struct system_user *main_user, int print);
struct item *add_new_item(char *path, struct item *head, struct system_user *main_user);
struct item *delete_item(char *path, struct item *head, struct system_user *main_user);
struct item *update_item(char *path, struct item *head, struct system_user *main_user);
struct item *sort_by_product_type(char *path, struct item *head, struct system_user *main_user);
struct item *sort_by_in_store(char *path, struct item *head, struct system_user *main_user);
void free_items(struct item *head);
