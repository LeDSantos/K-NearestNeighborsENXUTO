CrossFold_Kvizinhos: main.o 
	gcc -o3 -o CrossFold_Kvizinhos main.o -lm -Wall

main.o: main.c
	gcc -c main.c -Wall

clean:
	rm  *.o CrossFold_Kvizinhos metricas.csv detalhes.txt
