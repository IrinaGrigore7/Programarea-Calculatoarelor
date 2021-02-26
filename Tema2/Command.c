//Grigore Irina-Domnica
//315CB

#include "Command.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int isValidListArgument(char arg)
{
	char validArgs[] = "naAb";
	if (strchr(validArgs, arg) == NULL)
		return 0;
	return 1;
}

int isValidCommandChar(char command)
{
	char validCommands[] = "WCLRJPIOwclrjpio";
	if (strchr(validCommands, command) == NULL)
		return 0;
	return 1;
}
int countCommands(const char* operation_string, const char* separator)
{
	char* commands = (char*)malloc(strlen(operation_string) * sizeof(char) + 1);
	strcpy(commands, operation_string);
	int count = 0;
	char* token = strtok(commands, separator);
	while (token != NULL)
	{
		token = strtok(NULL, separator);
		++count;
	}
	free(commands);
	return count;
}
int splitOperationString(const char* operation_string, struct CommandArray* commandArray)
{
	commandArray->commandCount = 0;
	char* commands = (char*)malloc(strlen(operation_string) * sizeof(char) + 1);
	strcpy(commands, operation_string);
	char* token = strtok(commands, ",");
	while (token != NULL && commandArray->commandCount < 10)
	{
		if(strlen(token+1) > 20)
		return 1;
		if (isspace(token[0]) != 0)
		{
			strcpy(commandArray->data[commandArray->commandCount], token + 1);
		}
		else
		{
			strcpy(commandArray->data[commandArray->commandCount], token);
		}
		token = strtok(NULL, ",");
		commandArray->commandCount++;
	}
	free(commands);
	return 0;
}

int splitSingleCommand(char* command, struct CommandArray* commandArray)
{
	commandArray->commandCount = 0;
	char commands[20];
	strcpy(commands, command);
	char* token = strtok(command, " ");
	while (token != NULL)
	{
		if (isspace(token[0]) != 0)
		{
			strcpy(commandArray->data[commandArray->commandCount], token + 1);
		}
		else
		{
			strcpy(commandArray->data[commandArray->commandCount], token);
		}
		token = strtok(NULL, " ");
		commandArray->commandCount++;
	}
	return 0;
}

int createCommand(struct Command* command, struct CommandArray* commandArray)
{
	char commandType = commandArray->data[0][0];
	command->start_line = -1;
	command->end_line = -1;
	command->console_arg_count = commandArray->commandCount;
	switch (commandType)
	{
	case 'W':
	case 'w':
		if (commandArray->commandCount == 2)
		{
			command->type = commandType;
			strcpy(command->args, commandArray->data[commandArray->commandCount - 1]);
		}
		else
		{
			command->type = commandType;
			strcpy(command->args, "-1");
		}
		break;
	case 'L':
	case 'l':
	case 'R':
	case 'r':
	case 'C':
	case 'c':
	case 'J':
	case 'j':
	{
		command->type = commandType;

		if (commandArray->commandCount >= 2)
		{
			command->start_line = atoi(commandArray->data[1]);
		}
		if (commandArray->commandCount > 2)
		{
			command->end_line = atoi(commandArray->data[2]);
		}
	}
	break;
	case 'P':
	case 'p':
	{
		if (commandArray->commandCount == 2)
		{
			command->type = commandType;
			strcpy(command->args, commandArray->data[1]);
		}
		else

		{
			command->type = commandType;
			strcpy(command->args, "-1");
		}
		if (commandArray->commandCount > 2)
		{
			strcpy(command->args, commandArray->data[1]);
			command->start_line = atoi(commandArray->data[2]);
		}
		if (commandArray->commandCount > 3)
		{
			command->end_line = atoi(commandArray->data[3]);
		}
	}
		break;
	case 'I':
	case 'i':
		command->type = commandType;
		if (commandArray->commandCount > 2)
		{
			strcpy(command->args, commandArray->data[1]);
			strcat(command->args, commandArray->data[2]);
		}
		if (commandArray->commandCount > 3)
		{
			command->start_line = atoi(commandArray->data[3]);
		}
		if (commandArray->commandCount > 4)
		{
			command->end_line = atoi(commandArray->data[4]);
		}
		break;
	
	case 'O':
	case 'o':
		command->type = commandType;
		if (commandArray->commandCount > 3)
		{
			strcpy(command->args, commandArray->data[1]);
			strcat(command->args, commandArray->data[2]);
			strcat(command->args, commandArray->data[3]);
		}
		if (commandArray->commandCount > 4)
		{
			command->start_line = atoi(commandArray->data[4]);
		}
		if (commandArray->commandCount > 5)
		{
			command->end_line = atoi(commandArray->data[5]);
		}
		break;
	break;
	default:
		command->type = commandType;
		break;
	}
	return 0;
}

int parseCommands(const char* operation_string, struct Command commands[10], int* commandCount)
{
	int i;
	//char commandsArray[10][20];
	struct CommandArray commandsArray;
	if (countCommands(operation_string, ",") > 10)
	{
		printf("Too many operations! Only the first 10 will be applied.\n");
	}
	if(splitOperationString(operation_string, &commandsArray) != 0)
		return 1;

	for (i = 0; i < commandsArray.commandCount; ++i)
	{
		struct CommandArray singleCommand;
		int argsCount = 0;
		splitSingleCommand(commandsArray.data[i], &singleCommand);
		createCommand(&commands[i], &singleCommand);
	}
	*commandCount = commandsArray.commandCount;
	return validateCommands(commands,commandCount);
}
int validateCommandConsoleArgs(struct Command* command)
{
	char commandType = command->type;
	switch (commandType)
	{
	case 'W':
	case 'w':
	if(command->console_arg_count != 2)
		return 1;
	break;
	case 'L':
	case 'l':
	case 'R':
	case 'r':
	case 'C':
	case 'c':
	case 'J':
	case 'j':
	{
		if(command->console_arg_count < 1 || 
			command->console_arg_count > 3)
		return 1;
	break;
	case 'P':
	case 'p':
	if(command->console_arg_count < 2 || 
			command->console_arg_count > 4)
		return 1;
	break;
	case 'I':
	case 'i':
		if(command->console_arg_count < 3 || 
			command->console_arg_count > 5)
		return 1;
		if(isValidListArgument(command->args[0])==0)
		return 1;
	break;
	case 'O':
	case 'o':
		if(command->console_arg_count < 4 || 
			command->console_arg_count > 6)
		return 1;
		if(isValidListArgument(command->args[0])==0)
		return 1;
	break;
	default:
		return 1;
		break;
	}
	}
	return 0;
}
int validateCommands(struct Command commands[10], int* commandCount)
{
	int i=0;
	for (i = 0; i < *commandCount; ++i)
	{
		if(isValidCommandChar(commands[i].type) == 0)
			return 1;
		if((commands[i].start_line != -1 && commands[i].end_line != -1) &&
			(commands[i].start_line < 0 || commands[i].end_line < 0))
			return 1;
		if((commands[i].start_line > commands[i].end_line) &&
			commands[i].end_line != -1)
			return 1;
		int validArgs = validateCommandConsoleArgs(&commands[i]);
		if(validArgs !=0)
			return validArgs;
		
	}
	return 0;
}
