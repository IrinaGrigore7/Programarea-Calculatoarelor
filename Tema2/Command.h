//Grigore Irina-Domnica
//315CB

#ifndef COMMAND_H
#define COMMAND_H
struct  Command
{
	char type;
	char args[10];
	int start_line;
	int end_line;
	int console_arg_count;
};

struct CommandArray
{
	char data[10][20];
	char commandCount;
};

int isValidCommandChar(char command);
int isValidListArgument(char arg);
int countCommands(const char* operation_string, const char* separator);
int splitOperationString(const char* operation_string, struct CommandArray* commandArray);
int splitSingleCommand(char* command, struct CommandArray* commandArray);
int createCommand(struct Command* command, struct CommandArray* commandArray);
int parseCommands(const char* operation_string, struct Command commands[10], int* commandCount);
int vlaidateCommandConsoleArgs(struct Command* command);
int validateCommands(struct Command commands[10], int* commandCount);

#endif

