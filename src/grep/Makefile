CC=gcc

CFLAGS =-Wall -Werror -Wextra
PCRE_FLAG = -lpcre

GREP_DIRECTORY = 

GREP_SOURCES= $(GREP_DIRECTORY)imit_grep.c
GREP_OBJECTS= $(GREP_SOURCES:.c=.o)
GREP_EXECUTABLE=$(GREP_DIRECTORY)imit_grep

imit_grep: $(GREP_SOURCES) $(GREP_EXECUTABLE)
	
$(GREP_EXECUTABLE): $(GREP_OBJECTS) 
	$(CC) -L  /usr/local/lib/libpcre.a  $(GREP_OBJECTS) -o $@ $(PCRE_FLAG) -std=c11

.cp.o:
	$(CC) $(CFLAGS) $< -o $@	


clean:
	rm $(GREP_DIRECTORY)*.o
	rm $(GREP_DIRECTORY)imit_grep
