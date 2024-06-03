.PHONY: clean main test

# Target for the main program
main: main.c cli.c message_encode.c
	clang -o main main.c cli.c message_encode.c -lm

# Target for testing
test: message_encode.h message_encode.c test_message_encode.c
	clang -o test -Wall message_encode.c test_message_encode.c -lm -lcriterion

# Clean target for cleaning up build artifacts
clean:
	rm -rf myprogram test a.out