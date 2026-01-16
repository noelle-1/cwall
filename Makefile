PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
ICONDIR = $(PREFIX)/share/icons/hicolor/256x256/apps
DESKTOPDIR = $(PREFIX)/share/applications

CC = gcc


CFLAGS = $(shell pkg-config --cflags gtk4) \
	-Iinclude \
	-isystem

COMPFLAGS = -Wall \
	-pthread \
	-mfpmath=sse \
	-msse \
	-msse2 \
	-static \

LFLAGS = $(shell pkg-config --libs gtk4)

ifeq ($(OS),Windows_NT)
	LFLAGS += -L/mingw64/lib
	CFLAGS += -Iinclude/windows
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LFLAGS += -lnotify
endif

SRCDIR=src
OBJDIR=obj
HEADERDIR=include
TARGET=cwall

SOURCES := $(wildcard $(SRCDIR),*.c)
HEADERS := $(wildcard $(HEADERDIR),*.h)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(COMPFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LFLAGS) -o $@

clean:
	rm $(OBJDIR)/*.o

install:
ifeq ($(OS),Windows_NT)
	$(shell mkdir ./windows_portable)
	$(shell ldd cwall.exe | grep '\/mingw64\/bin\/.*dll' -o | xargs -I{} cp "{}" ./windows_portable/)
	$(shell mv ./cwall.exe ./windows_portable/)
endif
ifeq ($(UNAME_S),Linux)
	mkdir -pv $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)

	mkdir -pv $(ICONDIR)
	install -m 644 assets/${TARGET}.png $(ICONDIR)/${TARGET}.png
	
	mkdir -pv $(DESKTOPDIR)
	cp assets/$(TARGET).desktop $(DESKTOPDIR)/$(TARGET).desktop

endif
