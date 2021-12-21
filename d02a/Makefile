.POSIX:

include $(CONFIG_MK)

OBJECTS = main.o

all: a.out

.c.o:
	$(CC) $(MYCFLAGS) -c $<

a.out: $(OBJECTS)
	$(CC) $(MYLDFLAGS) -o a.out $(OBJECTS)

test: a.out
	./a.out < ./test_in.txt | cmp -s - ./test_out.txt

clean:
	rm -f a.out $(OBJECTS)

.PHONY: all clean test
