#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#endif

#ifdef linux
#include <libnotify/notify.h>
#include <unistd.h>
#endif

#include "structdef.h"
#include "helpers.h"
#include "config.h"

void process_index(int new_index, struct wallpaper_callback *callback) {
    callback->wallpapers_index = new_index;
    char *temp_buffer; 
    temp_buffer = malloc(strlen(callback->wallpapers[callback->wallpapers_index]) + strlen(callback->wallpapers_dir) + 2);
    strcpy(temp_buffer, callback->wallpapers_dir);
    strcat(temp_buffer, callback->wallpapers[callback->wallpapers_index]);

    gtk_picture_set_filename(GTK_PICTURE(callback->image), temp_buffer);
    gtk_widget_queue_draw(callback->image);

    char *temp_text = calculate_nthofnth(callback);
    update_label(GTK_LABEL(callback->label), temp_text);
    free(temp_text);

    update_label(GTK_LABEL(callback->current_wp_label), callback->wallpapers[callback->wallpapers_index]);

    free(temp_buffer);
    
    if (gtk_check_button_get_active(GTK_CHECK_BUTTON(callback->check_button))) {
        apply_wallpaper(callback);
    }
    
}

void update_label(GtkLabel *label, char *text) {
    char *markup_prelude = "<span><b><span size='large'>";
    char *markup_postlude = "</span></b></span>";
    char *temp_markup_buf = malloc(strlen(text) + strlen(markup_postlude) + strlen(markup_prelude) + 2);
    strcpy(temp_markup_buf, markup_prelude);
    strcat(temp_markup_buf, text);
    strcat(temp_markup_buf, markup_postlude);

    gtk_label_set_markup(GTK_LABEL(label), temp_markup_buf);
    free(temp_markup_buf);
}

char *calculate_nthofnth(struct wallpaper_callback *callback) {
    char *index = malloc(sizeof(int) + 1);
    char *size = malloc(sizeof(int) + 1);
    sprintf(index, "%d", callback->wallpapers_index + 1);
    sprintf(size, "%d", callback->wallpapers_size);
    char *temp_label_buf = malloc(strlen(size) + strlen(index) + 1);
    strcpy(temp_label_buf, index);
    strcat(temp_label_buf, "/");
    strcat(temp_label_buf, size);

    return temp_label_buf;
}

int exec_command(char *cmd) {
    return system(cmd);
}

char *get_save_file_path() {
    char *home = getenv("HOME");
    char *file = ".cache/last_wallpaper";
    char *save_file_path = malloc(strlen(home) + strlen(file) + 2);
    sprintf(save_file_path, "%s/%s", home, file);
    return save_file_path;
}

char *read_save_file() {
    char *save_file_path = get_save_file_path();
    FILE *save_file = fopen(save_file_path, "r");

    free(save_file_path);
    if (save_file == NULL) {
        send_notification("Error opening last wallpaper", "You have not previously applied a wallpaper.");
        return NULL;
    }

    static char wallpaper_path[256];
    fgets(wallpaper_path, sizeof(wallpaper_path), save_file);

    fclose(save_file);
    return wallpaper_path;
}

void write_save_file(char *wallpaper_path) {
    char *save_file_path = get_save_file_path();
    FILE *save_file = fopen(save_file_path, "w");
    fputs(wallpaper_path, save_file);
    free(save_file_path);
    fclose(save_file);
}

void _apply_wallpaper(char *wallpaper_path) {
    char *first = "nohup ";
    char *second = WALLPAPER_CMD;
    char *third = " >/dev/null 2>&1 &";
    char *temp_command = malloc(strlen(wallpaper_path) + strlen(first) + strlen(second) + strlen(third) + 1);
    strcpy(temp_command, first);
    strcat(temp_command, second);
    strcat(temp_command, wallpaper_path);
    strcat(temp_command, third);
    
    if (KILL_COMMAND_FLAG == 0) {
        exec_command(KILL_COMMAND);
    }

    exec_command(temp_command);
    free(temp_command);

    write_save_file(wallpaper_path);
    char *notify_title = "Successfully applied wallpaper";
    send_notification(notify_title, wallpaper_path);
}

void apply_wallpaper(struct wallpaper_callback *callback) {
    #ifdef linux
        char *wallpaper_path = malloc(strlen(callback->wallpapers_dir) + strlen(callback->wallpapers[callback->wallpapers_index]) + 1);
        strcpy(wallpaper_path, callback->wallpapers_dir);
        strcat(wallpaper_path, callback->wallpapers[callback->wallpapers_index]);
        _apply_wallpaper(wallpaper_path);
        free(wallpaper_path);
    #endif
    #ifdef _WIN32
        char *wallpaper_path = malloc(strlen(callback->wallpapers_dir) + strlen(callback->wallpapers[callback->wallpapers_index]) + 1);
        strcpy(wallpaper_path, callback->wallpapers_dir);
        strcat(wallpaper_path, callback->wallpapers[callback->wallpapers_index]);
        printf("%s\n", wallpaper_path);
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, wallpaper_path, SPIF_SENDCHANGE);

        free(wallpaper_path);
    #endif

}

struct collection_tuple get_subdirs(const char *wallpaper_path) {
    int count = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(wallpaper_path);
    
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0 && dir->d_type == DT_DIR) {
            count += 1;
        }
    }

    if (count == 0) {
        struct collection_tuple returnme = {
            NULL,
            count
        };
        return returnme;
    }

    char **subdirs = malloc(count * sizeof(char *));
    count = 0;
    d = opendir(wallpaper_path);
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0 && dir->d_type == DT_DIR) {
            subdirs[count] =  malloc((strlen(dir->d_name) + 1) * sizeof(char));
            strcpy(subdirs[count], dir->d_name);
            count += 1;
        }
    }

    struct collection_tuple returnme = {
        subdirs, 
        count
    };

    return returnme;
}

struct collection_tuple get_wallpapers(const char *wallpaper_path) {
    //read the directory once, Just for the amount of items.
    int count = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(wallpaper_path);

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0 && dir->d_type != DT_DIR) {
            count += 1;
        }
    }
    
    //read it again but actually save the contents
    if (count == 0) {
        struct collection_tuple returnme = {
            NULL,
            count
        };
        return returnme;
    }

    char **wallpapers = malloc(count * sizeof(char *));
    count = 0;
    d = opendir(wallpaper_path);
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0 && dir->d_type != DT_DIR) {
            wallpapers[count] =  malloc((strlen(dir->d_name) + 1) * sizeof(char));
            strcpy(wallpapers[count], dir->d_name);
            count += 1;
        }
    }

    struct collection_tuple returnme = {
        wallpapers, 
        count
    };

    return returnme;
}

void change_wallpaper_path(GtkWidget *widget, GdkEvent *event, gpointer data) {
    struct wallpaper_callback *callback = (struct wallpaper_callback *) data;
    
    int drop_down_selected = gtk_drop_down_get_selected(callback->drop_down);
    char *temp_fav = malloc(strlen(FAVORITES_NAME) - 1);
    strncpy(temp_fav, FAVORITES_NAME, strlen(FAVORITES_NAME) - 1);

    if (drop_down_selected == 0) {
        callback->in_favorites = false; 
        update_button(callback);
    }
    else if (strcmp(callback->subdirs[drop_down_selected - 1], temp_fav) == 0) {
        callback->in_favorites = true;
        update_button(callback);
    }
    else {
        callback->in_favorites = false;
        update_button(callback);
    }
    free(temp_fav);

    char *temp_buffer;
    if (drop_down_selected == 0) {
        temp_buffer = malloc(strlen(WALLPAPERS_PATH) + 1);
        strcpy(temp_buffer, WALLPAPERS_PATH);
    }
    else {
        temp_buffer = malloc(strlen(WALLPAPERS_PATH) + strlen(callback->subdirs[drop_down_selected - 1]) + 2);
        strcpy(temp_buffer, WALLPAPERS_PATH);
        strcat(temp_buffer, callback->subdirs[drop_down_selected - 1]);
        strcat(temp_buffer, "/");
    }
    
    callback->wallpapers_index = 0;

    struct collection_tuple tuple = get_wallpapers(temp_buffer); 
    if(tuple.collection_array == NULL) {
        char *notify_title = "Selected folder is empty";
        send_notification(notify_title, temp_buffer);
        gtk_drop_down_set_selected(callback->drop_down, 0);
        return;
    }

    strcpy(callback->wallpapers_dir, temp_buffer);
    free(temp_buffer);

    free(callback->wallpapers);
    callback->wallpapers = tuple.collection_array;
    callback->wallpapers_size = tuple.collection_size;
    process_index(callback->wallpapers_index, callback);
}

void favorite_wallpaper(struct wallpaper_callback *callback) {
    char *current_wallpaper_path = malloc(strlen(callback->wallpapers_dir) + strlen(callback->wallpapers[callback->wallpapers_index]) + 1);
    strcpy(current_wallpaper_path, callback->wallpapers_dir);
    strcat(current_wallpaper_path, callback->wallpapers[callback->wallpapers_index]);
    
    if (callback->in_favorites) {
        if (remove(current_wallpaper_path) != 0) {
            printf("Error: Falied to remove from favorites, Shutting down.\n");
            exit(1);
        }

        struct collection_tuple tuple = get_wallpapers(callback->wallpapers_dir); 
        if(tuple.collection_array == NULL) {
            char *notify_title = "Selected folder is empty";
            send_notification(notify_title, callback->wallpapers_dir);
            gtk_drop_down_set_selected(callback->drop_down, 0);
            return;
        }

        free(callback->wallpapers);
        callback->wallpapers = tuple.collection_array;
        callback->wallpapers_size = tuple.collection_size;
        callback->wallpapers_index = 0;
        process_index(callback->wallpapers_index, callback);

        char *notify_title = "Removed from favorites";
        send_notification(notify_title, current_wallpaper_path);
        return;
    }

    char *fav_name = FAVORITES_NAME;
    char *fav_path = malloc(strlen(WALLPAPERS_PATH) + strlen(fav_name) + 1);
    strcpy(fav_path, WALLPAPERS_PATH);
    strcat(fav_path, fav_name);

    if (check_folder_exists(fav_path) != 0) {
        int mkdir_return = 0;
        #ifdef _WIN32
            mkdir_return = _mkdir(fav_path);
        #endif
        #ifdef linux
            mkdir_return = mkdir(fav_path, 0777);
        #endif

        if (mkdir_return != 0) {
            printf("Error creating path: %s, Shutting Down!\n", fav_path);
            free(fav_path);
            exit(1);
        }
    }

    char *symlink_dest = malloc(strlen(fav_path) + strlen(callback->wallpapers[callback->wallpapers_index]) + 1);
    strcpy(symlink_dest, fav_path);
    strcat(symlink_dest, callback->wallpapers[callback->wallpapers_index]);
    
    if (check_file_exists(symlink_dest)) {
        char *notify_title = "Picture already in favorites";
        char *notify_message = "This picture is already in the favorites directory";
        send_notification(notify_title, notify_message);
        free(symlink_dest);
        free(current_wallpaper_path);
        free(fav_path);
        return;
    }
    
    int favorites_return = 0;
    #ifdef _WIN32
    	printf("Symlinks dont work on windows, Creating a copy instead.\n");	
    	char *command_prelude = "start powershell.exe -NoExit cp";
	char *cp_command = malloc(strlen(current_wallpaper_path) + strlen(symlink_dest)
			+ strlen(command_prelude) + 6);
	sprintf(cp_command, "%s '%s' '%s'", command_prelude, current_wallpaper_path, symlink_dest);
	printf("command: %s\n", cp_command);
	favorites_return = exec_command(cp_command);
	free(cp_command);
    #endif
    #ifdef linux
        favorites_return = symlink(current_wallpaper_path, symlink_dest);
    #endif

    if (favorites_return != 0) {
        printf("Error adding to favorites\n") ;
        free(symlink_dest);
        free(current_wallpaper_path);
        free(fav_path);
        exit(1);
    }

    char *notify_title = "Added to favorites";
    send_notification(notify_title, NULL);
    
    free(symlink_dest);
    free(current_wallpaper_path);
    free(fav_path);
}

bool check_file_exists(char *path) {
    struct stat b;  
    return (stat(path, &b) == 0);
}

int check_folder_exists(char *path) {
    struct stat sb;

    if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 0;
    }
    else {
        return 1;
    }
}

void send_notification(char *title, char *message) {
    #ifdef linux
	NotifyNotification *noti;
	notify_init("no-tification");
	noti = notify_notification_new(title, message, NULL);
	notify_notification_show(noti, NULL);
    printf("%s: %s", title, message);
    #endif
    #ifdef _WIN32
	printf("Notifications not supported on windows.\n");
	printf("Title: %s\nMessage: %s\n", title, message);
    #endif
}

void update_button(struct wallpaper_callback *callback) {
    if (callback->in_favorites) {
        gtk_button_set_label(GTK_BUTTON(callback->favorite_button), "Remove from favorites");
    }
    else {
        gtk_button_set_label(GTK_BUTTON(callback->favorite_button), "Add to favorites");
    }

    gtk_widget_queue_draw(GTK_WIDGET(callback->favorite_button));
}
