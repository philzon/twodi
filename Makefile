TITLE   := twodi
VERSION := 0.1.0
BUILD   := $(shell ./script/build-id.sh)
COMMIT  := $(shell git rev-parse --short HEAD)
BRANCH  := $(shell git rev-parse --abbrev-ref HEAD)
DATE    := $(shell date +"%Y-%m-%d %H:%M:%S")

CC      := g++
CFLAGS  := -std=c++11 \
           -DBUILD_TITLE='"$(TITLE)"' \
           -DBUILD_VERSION='"$(VERSION)"' \
           -DBUILD_BUILD='"$(BUILD)"' \
           -DBUILD_COMMIT='"$(COMMIT)"' \
           -DBUILD_BRANCH='"$(BRANCH)"' \
		   -DBUILD_DATE='"$(DATE)"' \
           -g
LIBS    := -lSDL2main -lSDL2

BINDIR  := bin
SRCDIR  := src
INCDIR  := include
LIBDIR  := lib
OBJDIR  := obj
RESDIR  := resource

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

.PHONY: all clean init generate build

all: init generate build

clean:
	@rm -rf $(BINDIR)
	@rm -rf $(OBJDIR)

init:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)

generate:
	@cp $(RESDIR)/* $(BINDIR)
	@echo "$(VERSION) ($(COMMIT))" > $(BINDIR)/version.txt

build: $(BINDIR)/$(TITLE)

$(BINDIR)/$(TITLE): $(OBJECTS)
	@$(CC) -L $(LIBDIR) -o $(BINDIR)/$(TITLE) $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -I $(INCDIR) -c -o $@ $<
	@echo "CC  $@"
