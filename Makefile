# Declaration of variables
CC = gcc
CC_FLAGS = -w 
LINKER_FLAGS = 	-L/usr/local/lib -lncurses -ltinfo


# File names
EXEC = bin/main
SOURCES = main.c ADandBS.c time.c
OBJECTS = main.o ADandBS.o time.o 

#ALL
all: bindir $(EXEC)

#Create necessary directories	
bindir:
	mkdir -p bin/


# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(addprefix bin/, $(OBJECTS)) $(LINKER_FLAGS) -o $(EXEC)


# To compile 
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $(@:%.o=%.cpp) -o bin/$@

%.o: %.c
	$(CC) -c $(CC_FLAGS) $(@:%.o=%.c) -o bin/$@

# To link object file to standalone exec files
link-%:
	$(CC) $(CC_FLAGS) $(addprefix bin/, $(OBJECTS)) $(LINKER_FLAGS) -o bin/$(@:link-%=%)

# To remove generated files
clean:
	rm -f $(EXEC) $(addprefix bin/, $(OBJECTS))

