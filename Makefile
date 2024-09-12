bf-main: main.c
	gcc -O2 -obf-main main.c

clean:
	rm bf-main

.PHONY: clean
