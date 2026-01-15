#include "structdef.h"

void update_label(GtkLabel *label, char *text);
void process_index(int new_index, struct wallpaper_callback *callback);
char *calculate_nthofnth(struct wallpaper_callback *callback);
void apply_wallpaper(struct wallpaper_callback *callback);
struct collection_tuple get_wallpapers(const char *wallpaper_path);
struct collection_tuple get_subdirs(const char *wallpaper_path);
void change_wallpaper_path(GtkWidget *widget, GdkEvent *event, gpointer data);
void favorite_wallpaper(struct wallpaper_callback *callback);
bool check_file_exists(char *path);
int check_folder_exists(char *wallpaper_path);
void update_button(struct wallpaper_callback *callback);
void send_notification(char *title, char *message);
int exec_command(char *cmd);
void _apply_wallpaper(char *wallpaper_path);
void write_save_file(char *wallpaper_path);
char *get_save_file_path();
char *read_save_file();

