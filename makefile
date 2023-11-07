FLAGS = -Wall
OBJS = particionaArquivo.o libCliente.o libServidor.o

all: cliente servidor

cliente: cliente.o $(OBJS)
	gcc -o cliente cliente.o $(OBJS) $(FLAGS)

servidor: servidor.o $(OBJS)
	gcc -o servidor servidor.o $(OBJS) $(FLAGS)

particionaArquivo.o: particionaArquivo.c
	gcc -c particionaArquivo.c $(FLAGS)  

libCliente.o: libCliente.c
	gcc -c libCliente.c $(FLAGS)

libServidor.o: libServidor.c
	gcc -c libServidor.c $(FLAGS)

servidor.o: servidor.c 
	gcc -c servidor.c $(FLAGS)
	
cliente.o: cliente.c
	gcc -c cliente.c $(FLAGS)

clean:
	@rm -f *~ *.o

purge: clean
