PROGRAM = libdelogo.so
OBJS = delogo.o

CC = gcc
CFLAGS = -O3 -I/usr/local/include/avisynth -fPIC -ffast-math -Wall -Wshadow -Wempty-body -I. -std=gnu99 -fpermissive -fomit-frame-pointer -s -fno-tree-vectorize 
LDLAGS = -shared -fPIC 

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^ $(LDLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
