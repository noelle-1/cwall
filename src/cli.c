#include <stdio.h>

#include "cli.h"
#include "helpers.h"

void cli(char *wallpaper_path) {
    if (check_file_exists(wallpaper_path)) {
        printf("Setting wallpaper: %s\n", wallpaper_path);
        _apply_wallpaper(wallpaper_path);
    }
    else {
        printf("Could not find specified image: %s\nwallpaper will not be applied.\n", wallpaper_path);
        exit(1);
    }
}
void apply_last_wallpaper() {
    char *wallpaper_path = read_save_file();
    if (wallpaper_path == NULL) {
        printf("its null buddy\n");
        exit(1);
    }
    _apply_wallpaper(wallpaper_path);
}
