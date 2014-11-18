int drop_privileges(int permanent); 
int restore_privileges(void);

char *get_user_name(void);
const char *get_effective_user_name(void);
