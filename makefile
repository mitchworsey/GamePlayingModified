CC = g++
CFLAGS = -O3
EXEC = GamePlayer
	
all:
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp
	
clean:
	rm -f $(EXEC)

test:
	make clean		
	make all
	./$(EXEC)
	
