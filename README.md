# cwall

**cwall** is a wallpaper application written in C, its my first real C project, and definitely FULL of memory leaks.

## Screenshots
1. Niri.
![Niri](https://raw.githubusercontent.com/noelle-1/cwall/main/image.png)
2. Linux Mint.
![Mint](https://raw.githubusercontent.com/noelle-1/cwall/main/niri-2026-01-16%2002-44-27.png)

## Requirements
Before building cwall, ensure you have the following dependencies installed:

- pkg-config
- GTK libraries
- libnotify libraries

## Building from Source

1. Clone the repository:

```bash
git clone https://github.com/noelle-1/cwall
cd cwall
```
2. Configure:
```bash
vim src/config.c
```
Fill out variables for your system. (config includes examples.)

3. Install dependencies:

### On Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install pkg-config libgtk-4-dev libnotify-dev
```

### On other distributions, use your package manager to install pkg-config and the GTK dev libraries.

4. Build the application:

```bash
make
```

5. Install the application system-wide (optional):

```bash
sudo make install PREFIX=/usr
```

This will copy `cwall` to your system's bin directory (e.g., `/usr/bin/`).

## Usage

After installation, you can run the application by executing:
    
Gui:
```bash
cwall -g 

```
Cli, Apply a wallpaper: 
```bash
cwall -w /path/to/a/wallpaper
```

Cli, Load last applied wallpaper:
```bash
cwall -l
```

You must have a root wallpaper folder specified in the config, the wallpaper folder may contain Subfolders full of pictures <3
## Known bugs

1. If atleast 1 image file is not present in the root wallpaper folder, but the subdirectories contain images, the application will fail to open stating "Folder must contain atleast 1 wallpaper". to solve this just have atleast 1 "wallpaper" in the root folder you specified.

2. Memory leaks, i believe my code is pretty bad, and it was written over a year ago. there WILL be memory leaks im sure. but the application does not suffer from segfaults or crashes.

3. Windows support is broken, i dont remember how to build it for windows, i will update the readme with instructions in the future

4. Image file names with spaces may or may not work properly. sometimes will just show up as a blank empty image.

5. Webp support is not available. only JPG, PNG, JPEG.
