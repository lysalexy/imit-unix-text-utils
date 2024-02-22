CC=gcc

CFLAGS =-Wall -Werror -Wextra

CAT_DIRECTORY = 
COMMON_DIRECTORY = ../common/

CAT_SOURCES= $(CAT_DIRECTORY)imit_cat_options.c $(CAT_DIRECTORY)imit_cat.c $(CAT_DIRECTORY)imit_cat_processing.c
CAT_OBJECTS=$(COMMON_SOURCES:.c=.o) $(CAT_SOURCES:.c=.o)
CAT_EXECUTABLE=$(CAT_DIRECTORY)imit_cat

imit_cat: $(CAT_SOURCES) $(CAT_EXECUTABLE)
	
$(CAT_EXECUTABLE): $(CAT_OBJECTS) 
	$(CC) $(CAT_OBJECTS) -o $@

.cp.o:
	$(CC) $(CFLAGS) $< -o $@	


clean:
	rm $(CAT_DIRECTORY)*.o
	rm $(COMMON_DIRECTORY)*.o
	rm $(CAT_DIRECTORY)imit_cat
