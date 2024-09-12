bf-main: main.c
	gcc -O2 -obf-main main.c

run: bf-main m.bf
	./bf-main

time: bf-main m.bf
	time ./bf-main

clean:
	rm bf-main

.PHONY: clean run time
