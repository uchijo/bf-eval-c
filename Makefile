bf-main: main.c preprocess.h preprocess.c
	gcc -O2 -obf-main main.c preprocess.c

run: bf-main m.bf
	./bf-main

clean:
	rm bf-main

.PHONY: clean run time
