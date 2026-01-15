#include "gtk/gtk.h"

#ifndef STRUCT_DEF
#define STRUCT_DEF

    struct collection_tuple {
        char **collection_array;
        int collection_size;
    };

    struct wallpaper_callback {
        GtkWidget *window;
        GtkWidget *image;
        GtkWidget *label;
        GtkWidget *current_wp_label;
        GtkWidget *favorite_button;

        char *wallpapers_dir;
        int wallpapers_size;
        int wallpapers_index;
        char **wallpapers;
        GtkWidget *check_button;
        GtkDropDown *drop_down;
        char **subdirs;
        int subdirs_size;
        bool in_favorites;
    };

#endif
