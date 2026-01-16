# cwall

**cwall** is a wallpaper application written in C, its my first real C project, and definitely FULL of memory leaks.
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
sudo make install PREFIX=/usr/local
```

This will copy `cwall` to your system's bin directory (e.g., `/usr/local/bin/`).

## Usage

After installation, you can run the application by executing:

```bash
cwall -g
```

Follow the on-screen instructions to browse and select wallpapers.
