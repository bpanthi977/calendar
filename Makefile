# Declaration of variables
CC = gcc
CC_FLAGS = -w 
LINKER_FLAGS = 	-L/usr/local/lib -lncurses -ltinfo


# File names
EXEC = bin/main
SOURCES = main.c ADandBS.c time.c
OBJECTS = main.o ADandBS.o time.o
HEADERS = common.h
CODEFILES = $(SOURCES) $(HEADERS)
CBPFILE = Calendar.cbp
WINCODEDIR = /tmp/wincode


#ALL
all: bindir $(EXEC)

#Create necessary directories	
bindir:
	mkdir -p bin/


# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(addprefix bin/, $(OBJECTS)) $(LINKER_FLAGS) -o $(EXEC)


# To compile 
%.o: %.c
	$(CC) -c $(CC_FLAGS) $(@:%.o=%.c) -o bin/$@

# To link object file to standalone exec files
link-%:
	$(CC) $(CC_FLAGS) $(addprefix bin/, $(OBJECTS)) $(LINKER_FLAGS) -o bin/$(@:link-%=%)

# Preprocess the OS dependent directive for creating code that run on windows only (Useful for sharing minimal code to friends)
wincode: windir $(addprefix win-, $(CODEFILES)) cbpAndPackage


win-%:
	unifdef -x 2 -U__linux__ -o $(WINCODEDIR)/$(@:win-%=%) $(@:win-%=%)

windir: 
	mkdir -p $(WINCODEDIR)/

cbpAndPackage: #Copy codeblocks project file and package everything in a zip file.
	cp $(CBPFILE) $(WINCODEDIR)/
	tar --remove-files -C $(WINCODEDIR)/.. -cf $(CBPFILE:%.cbp=%.zip) $(notdir $(WINCODEDIR))
#	rm -r $(WINCODEDIR)


# To remove generated files
clean:
#rm -f $(EXEC) $(addprefix bin/, $(OBJECTS))
	rm -f -r bin/	
	rm -f $(CBPFILE:%.cbp=%.zip)

