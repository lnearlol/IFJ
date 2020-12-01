-std=c99 -Wall -Wextra -Werror
all: parser
parser: parser.c
	gcc token.h scanner.h symtable.h expression_translator.h parser.h scanner.c symtable.c code_generator.c expression_translator.c logic.c parser.c -o compiler
