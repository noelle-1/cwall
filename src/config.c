#include "config.h"

char *WALLPAPERS_PATH = "/home/noelle/Pictures/wallpapers/"; //make sure to add a trailing slash to the end
char *WALLPAPER_CMD = "swaybg -m fill -i "; //if the command requires a space add it
char *FAVORITES_NAME = "Favorites/"; //Name of the folder to store favorites in 
uint8_t KILL_COMMAND_FLAG = 0; //set to 1 if you DONT want to execute a kill command.
char *KILL_COMMAND = "kill swaybg"; //the KILL_COMMAND, only executed if KILL_COMMAND_FLAG is equal to 0.
