-std=c99 -Wall -Wextra -Werror
all: parser
parser: parser.c
	gcc token.h scanner.h symtable.h expressionBT.h expression-translator.h parser.h scanner.c symtable.c expressionBT.c expression-translator.c logic.c parser.c -o compiler
