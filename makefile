CC = gcc 
CFLAGS +=-g -O3

targets = knap knap_para
objects = knap.o knap_para.o

.PHONY : default
default : all

.PHONY : all
all : $(targets)

knap : knap.c 
	$(CC) $(CFLAGS) -o $@ $^

knap_para : knap_para.c 
	$(CC) $(CFLAGS) -fopenmp -o $@ $^

.PHONY : clean
clean:
	rm -f $(targets) $(objects) *~ 