
void create_users(char *path);
int system_enter(char *path, struct system_user **s_user);
struct system_user *read_users(char *path, struct system_user *head_of_users, struct system_user *main_user);
struct system_user *add_new_user(char *path, struct system_user *head_of_users, struct system_user *main_user);
struct system_user *delete_user(char *path, struct system_user *head_of_users, struct system_user *main_user);
struct system_user *update_user(char *path, struct system_user *head_of_users, struct system_user *main_user);
void free_system_user(struct system_user *head);
