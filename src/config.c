#include "config.h"
//the following file contains a premade example that i use on my system. tweak it to fit yours.

char *WALLPAPERS_PATH = "/home/noelle/Pictures/wallpapers/"; //the root wallpapers folder.
                                                             //MAKE SURE TO ADD A TRAILING SLASH.
                                                             //ONLY USE FULL PATHS. NO ENV VARS.

char *WALLPAPER_CMD = "swaybg -m fill -i "; //leave a space after the command. the image will be appended to the end.
                                            //another exampe "feh --bg-fill " (note the space at the end)
                                            //any wallpaper applying command will work here since its a wrapper.

char *FAVORITES_NAME = "Favorites/"; //Name of the folder to store favorites in
                                     //The application will create a Folder of this name in the root wallpaper dir
                                     //MAKE SURE YOU HAVE RW ACCESS IT WILL BREAK IF YOU DONT

uint8_t KILL_COMMAND_FLAG = 0; //set to 1 if you DONT want to execute a kill command.
                               
char *KILL_COMMAND = "kill swaybg"; //the KILL_COMMAND, only executed if KILL_COMMAND_FLAG is equal to 0.
                                    //the kill command is useful for swaybg 
                                    //because running the command over and over creates lots of processes. 
                                    //some wallpaper commands, like feh, dont need the kill command.
