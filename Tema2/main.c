//Grigore Irina-Domnica
//315CB

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Command.h"
#include "Text.h"


int applyOperations(struct Text* result, 
					const struct Command commands[10], 
					const int* commandCount)
{

	int i = 0;
	int error = 0;
	for (i = 0; i < *commandCount; ++i)
	{
		if (error == 0)
		{
			char commandType = commands[i].type;
			switch (commandType)
			{
			case 'W':
			case 'w':
			{
				int max_line_length = atoi(commands[i].args);
				wordWrap(result, max_line_length);
			}
			break;
			case 'L':
			case 'l':
				alignLeft(result, commands[i].start_line, commands[i].end_line);
				break;
			case 'R':
			case 'r':
				alignRight(result, commands[i].start_line, commands[i].end_line);
				break;
			case 'C':
			case 'c':
				alignCenter(result, commands[i].start_line, commands[i].end_line);
				break;
			case 'J':
			case 'j':
				justify(result, commands[i].start_line, commands[i].end_line);
				break;
			case 'P':
			case 'p':
			{
				int indent = atoi(commands[i].args);
				paragraph(result, indent, commands[i].start_line, commands[i].end_line);
			}
				break;
			case 'I':
			case 'i':
				list(result, commands[i].args, commands[i].start_line, commands[i].end_line);
				break;
			case 'O':
			case 'o':
				orderList(result, commands[i].args, commands[i].start_line, commands[i].end_line);
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	int i;
	struct Text original_text, result_text;
	clearText(&original_text);
	if (argc != 4) { 
		fprintf(stderr,
			"Usage: %s operations_string input_file output_file\n",
			argv[0]);
		return -1;
	}
	char* operation_string = (char*)malloc(strlen(argv[1])*sizeof(char)+1);
	strcpy(operation_string,argv[1]);
	if (readFile(argv[2], &original_text) != 0)
		return -2;

	copyText(&original_text, &result_text);
	int commandCount = 0;
	struct Command commands[10];
	if(parseCommands(operation_string, commands, &commandCount) == 0){
	applyOperations(&result_text, commands, &commandCount);
	}
	else{
		printf("Invalid operation!\n");
	}

	if (writeFile(argv[3], &result_text) != 0) {
		return -2;
	}

	return 0;

}


