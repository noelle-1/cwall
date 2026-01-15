#include "buttons.h"
#include "helpers.h"
#include <time.h>
#include <stdio.h>

void button_right(GtkWidget *widget, gpointer data) {

    struct wallpaper_callback *callback = (struct wallpaper_callback *)data;

    if (callback->wallpapers_index + 1 == callback->wallpapers_size) {
    process_index(0, callback);
    return;
    }
    process_index(callback->wallpapers_index + 1, callback);
}

void button_left(GtkWidget *widget, gpointer data) {
    struct wallpaper_callback *callback = (struct wallpaper_callback *)data;

    if (callback->wallpapers_index == 0) {
    process_index(callback->wallpapers_size - 1, callback);
    return;
    }
    process_index(callback->wallpapers_index - 1, callback);
}

void select_wallpaper(GtkWidget *widget, gpointer data) {
    struct wallpaper_callback *callback = (struct wallpaper_callback *)data;
    apply_wallpaper(callback);
}

void button_fav(GtkWidget *widget, gpointer data) {
    struct wallpaper_callback *callback = (struct wallpaper_callback *)data;
    favorite_wallpaper(callback);
}

void button_random(GtkWidget *widget, gpointer data) {
    struct wallpaper_callback *callback = (struct wallpaper_callback *)data;
    
    int rand_num;
    srand(time(NULL));
    rand_num = (rand() % (callback->wallpapers_size)) + 0;
    
    if (callback->wallpapers_index != rand_num) {
        callback->wallpapers_index = rand_num;
        process_index(callback->wallpapers_index, callback);
    }

}
