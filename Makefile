-std=c99 -Wall -Wextra -Werror
parser: parser.c
	gcc token.h scanner.h symtable.h parser.h scanner.c symtable.c logic.c parser.c -o compiler
