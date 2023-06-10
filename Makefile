############################################################
# file: plot.py
# authors: Maxence Montavon & Damian Boquete Costa
############################################################

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11 -Werror
LINKS = -lm
SANITIZERS  = -fsanitize=address -fsanitize=leak -fsanitize=undefined
OBJECTS = optimization.o coordinates.o
TARGET = exe
TEST = test
DATAS = data*.txt

$(TARGET): main.c $(OBJECTS)
	$(CC) $(CFLAGS) $(SANITIZERS) $^ -o $@ $(LINKS)

$(TEST): test.c $(OBJECTS)
	$(CC) $(CFLAGS) $(SANITIZERS) $^ -o $@ $(LINKS)

optimization.o: lib/optimization.c
	$(CC) $(CFLAGS) $(SANITIZERS) $^ -c $<

coordinates.o: lib/coordinates.c
	$(CC) $(CFLAGS) $(SANITIZERS) $^ -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(TEST)
	rm -f $(DATAS)

rebuild: clean $(TARGET)

# target: element1.o element2.o
# $^ = all elements
# $< = first element
# $@ = name of target
