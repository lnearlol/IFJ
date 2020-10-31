-std=c99 -Wall -Wextra -Werror
scanner: scanner.c
	gcc token.h scanner.h scanner.c -o scanner
