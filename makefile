FLAGS = -Wall
OBJS = particionaArquivo.o

# .PHONY: all debug clean purge

# %.o: %.c %.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(PROG): % : $(OBJS) %.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

all: cliente servidor

cliente: cliente.o $(OBJS)
	gcc -o cliente cliente.o $(OBJS) $(FLAGS)

servidor: servidor.o $(OBJS)
	gcc -o servidor servidor.o $(OBJS) $(FLAGS)

particionaArquivo.o: particionaArquivo.c
	gcc -c particionaArquivo.c $(FLAGS)  

servidor.o: servidor.c 
	gcc -c servidor.c $(FLAGS)
	
cliente.o: cliente.c
	gcc -c cliente.c $(FLAGS)

clean:
	@rm -f *~ *.o

purge: clean
