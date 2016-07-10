# Compiler
CC = g++

# Project name
PROJECT = run-vvjj-flavor-selector

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
ROOTCFLAGS = $(shell root-config --cflags) -Wall -Wextra -pedantic -O3
ROOTLIBS   = $(shell root-config --libs)

# Files and folders
SRCS    = $(shell find $(SRCDIR) -type f -name '*.cxx')
HEADERS = $(shell find $(SRCDIR) -type f \( -iname "*.h" ! -iname "*Linkdef*" \))
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS    = $(patsubst $(SRCDIR)/%.cxx,$(OBJDIR)/%.o,$(SRCS))

# Targets
$(PROJECT): buildrepo MyDict.cxx $(OBJS)
	$(CC) -o $@ MyDict.cxx $(OBJS) $(ROOTCFLAGS) $(ROOTLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	$(CC) -o $@ $< -c $(ROOTCFLAGS)

MyDict.cxx: $(HEADERS) src/Linkdef.h
	rootcint -f $@ -c $(ROOTCFLAGS) -p $^

clean:
	rm $(PROJECT)
	rm -rf $(OBJDIR)
	rm MyDict.cxx
	rm MyDict_rdict.pcm

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
