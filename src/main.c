#include <dirent.h>
#include <gtk-4.0/gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "cli.h"
#include "gtk.h"


int main(int argc, char **argv) {
    int gui_flag = 1;
    int last_wallpaper_flag = 1;
    char *wallpaper_path = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "lgw:")) != -1) {
        switch (opt) {
            case 'l':
                last_wallpaper_flag = 0;
                break;
            case 'g':
                gui_flag = 0;
                break;
            case 'w':
                wallpaper_path = optarg;
                break;
            case '?':
                if (optopt == 'w') {
                    printf("You must supply a wallpaper path.\n");
                    exit(1);
                }
                printf("Unknown arg: %c\n", optopt);
                break;
        }
    }
         
    if (gui_flag == 0) {
        GtkApplication *app =
                gtk_application_new("com.Noelle.Cwall", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
        return g_application_run(G_APPLICATION(app), 0, NULL);
    }
    else if (last_wallpaper_flag == 0) {
        apply_last_wallpaper();
    }
    else {
        if (wallpaper_path == NULL) {
            printf("You need to supply a wallpaper path. (-w)\n");
            exit(1);
        }
        cli(wallpaper_path);
    }
}
