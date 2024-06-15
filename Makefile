.PHONY: all
all: generator primeCounter myprimeCounter

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c
	gcc -o primeCounter primeCounter.c

myprimeCounter:	myprimeCounter.c myprimeCounter.h
	gcc -o myprimeCounter myprimeCounter.c

.PHONY: clean
clean:
	-rm randomGenerator primeCounter myprimeCounter 2>/dev/null
