TARGET = krishnamoorthynatarajanke_proj2.exe
SRC = krishnamoorthynatarajanke_proj2.c
INC = krishnamoorthynatarajanke_proj2.h
HELPER = helper_fn.c
CC = gcc

all: $(TARGET)

krishnamoorthynatarajanke_proj2.exe: $(SRC) $(INC) $(HELPER)
	$(CC) -o $(TARGET) $(SRC) $(HELPER)
	

.PHONY: clean
clean:
	rm -f *.txt $(TARGET)
