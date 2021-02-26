//Grigore Irina-Domnica
//315CB

#include "Text.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


int readFile(const char* path, struct Text* result)
{
	char buf[1000];
	FILE *input_file = fopen(path, "r");
	if (input_file == NULL) {
		return -2;
	}
	while (fgets(buf, 1000, input_file)) {
		strcpy(result->data[result->lineCount], buf);
		result->lineCount++;
	}
	fclose(input_file);
	return 0;
}
int writeFile(const char* path, const struct Text* source)
{

	int i;
	FILE *output_file = fopen(path, "w");

	if (output_file == NULL) {
		fprintf(stderr, "Could not open or create file \"%s\"\n", path);
		return -2;
	}
	for (i = 0; i < source->lineCount; i++) {
		fprintf(output_file, "%s", source->data[i]);
	}

	fclose(output_file);
	return 0;
}

void copyText(const struct Text* original, struct Text* result)
{
	clearText(result);
	int i;
	for (i = 0; i < original->lineCount; i++) {
		strcpy(result->data[i], original->data[i]);
	}
	result->lineCount = original->lineCount;
}
void clearText(struct Text* text)
{
	int i = 0;
	for (i = 0; i < 1000; ++i)
	{
		text->data[i][0] = 0;
	}
	text->lineCount = 0;
}
int findLongestWord(struct Text* text)
{
	int result = 0;
	int i = 0;
	for (i = 0; i < text->lineCount; ++i)
	{
		int startIndex = 0;
		int j = 0;
		for (j = 0; j < strlen(text->data[i]); ++j)
		{
			if (text->data[i][j] == ' ' ||
				text->data[i][j] == '\t' || 
				text->data[i][j] == '\n') 
				{
				if (j - startIndex - 1 > result)
					result = j - startIndex - 1;
				while (text->data[i][j] == ' ' || text->data[i][j] == '\t')
					j++;
				startIndex = j;
			}
		}
	}

	return result;
}

int findLongestLine(struct Text* text) {
	int result = 0;
	int i;
	for (i = 0; i < text->lineCount; ++i)
	{
		if (strlen(text->data[i]) >= strlen(text->data[result]))
		{
			result = i;
		}
	}
	return result;
}

int countSpaces(char* line)
{
	int result = 0;
	int i = 0;
	for (i = 0; i < strlen(line); ++i)
	{
		if (line[i] == ' ') {
			result++;
		}
	}
	return result;
}

int skipSpaces(char paragraph[1000000], int startIndex)
{
	int result = startIndex;
		while (isWordSeparator(paragraph[result])==1)
		{
			++result;
		}
	return result;
}

int isWordSeparator(char character)
{
	if(character == ' ' || character == '\t' || character == '\n')
	return 1;
	return 0;
}
int isParagraphSeparator(const char* line)
{
	if (strcmp(line, "\n") == 0)
		return 1;
	return 0;
}

int getParagraph(const struct Text* text, char paragraph[1000000], int* separatorLine)
{
	strcpy(paragraph, "\0");
	int line = *separatorLine;
	while (isParagraphSeparator(text->data[line]) == 0 && *separatorLine < text->lineCount)
	{
		strncat(paragraph, text->data[line], strlen(text->data[line]));
		++line;
		if (line >= text->lineCount || isParagraphSeparator(text->data[line]) == 1)
		{
			strncat(paragraph, "\0", 1);
			*separatorLine = line;

		}
	}
	paragraph[strlen(paragraph) - 1] = '\0';
	return 0;
}

int wordWrap(struct Text* result, int max_line_lenght)
{
	if (max_line_lenght == -1)
	{
		printf("Invalid operation!\n");
		return 1;
	}
	if (max_line_lenght < findLongestWord(result))
	{
		printf("Cannot wrap!\n");
		return 1;
	}
	struct Text temp;
	int i = 0;
	copyText(result, &temp);
	int separatorLine = 0;
	int currentLine = 0;
	clearText(result);
	int operationEnded = 0;
	while (separatorLine <= temp.lineCount && operationEnded == 0)
	{
		char paragraph[1000000];
		strcpy(paragraph, "");
		getParagraph(&temp, paragraph, &separatorLine);
		int startCharIndex = 0;
		int endCharIndex = 0;
		int length = strlen(paragraph);
		startCharIndex = skipSpaces(paragraph,startCharIndex);
		if (startCharIndex > 0)
		{
			int difference = startCharIndex;
			while (difference >= max_line_lenght) {
				strcpy(result->data[currentLine], "\n");
				difference -= max_line_lenght;
				currentLine++;
			}
			if (difference < 0)
			{
				difference = startCharIndex;
			}
			strcpy(result->data[currentLine], "");
			strncat(result->data[currentLine], paragraph + startCharIndex - difference, difference);
		}
		for (i = startCharIndex; i < strlen(paragraph); ++i)
		{
			if (paragraph[i] == '\n')
				paragraph[i] = ' ';
			if (isWordSeparator(paragraph[i]) == 1  || paragraph[i + 1] == '\0')
			{
				endCharIndex = i;
				int wordLenght = endCharIndex - startCharIndex;
				int lineLength = strlen(result->data[currentLine]);
				if (lineLength + wordLenght <= max_line_lenght)
				{
					strncat(result->data[currentLine], paragraph + startCharIndex, wordLenght + 1);
					startCharIndex = endCharIndex + 1;
					
					startCharIndex = skipSpaces(paragraph,startCharIndex);
					i = startCharIndex;
				}
				else
				{
					int lastIndex = strlen(result->data[currentLine]) - 1;
					result->data[currentLine][lastIndex] = '\n';
					currentLine++;
					startCharIndex = skipSpaces(paragraph,startCharIndex);
					strncat(result->data[currentLine], paragraph + startCharIndex, wordLenght + 1);

					startCharIndex = endCharIndex + 1;
					startCharIndex = skipSpaces(paragraph,startCharIndex);
					i = startCharIndex;
				}
			}
		}

		if (separatorLine == temp.lineCount)
		{
			operationEnded = 1;
			currentLine++;
			strcpy(result->data[currentLine], "\n");
			result->lineCount = currentLine + 1;
		}
		else
		{
			int lastIndex = strlen(result->data[currentLine]) - 1;
			if (result->data[currentLine][lastIndex] != ' ' || 
				result->data[currentLine][lastIndex] != '\t' || 
				result->data[currentLine][lastIndex] != '\0')
			{
				lastIndex++;
			}
			result->data[currentLine][lastIndex] = '\n';
			result->data[currentLine][lastIndex + 1] = '\0';
			while (isParagraphSeparator(temp.data[separatorLine]) == 1)
			{
				currentLine++;
				strcpy(result->data[currentLine], "\n");
				separatorLine++;
			}
			currentLine++;
		}
	}
return 0;
}



int alignLeft(struct Text* result, int start_line, int end_line)
{
	int i = 0;
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	for (i = start_line; i <= end_line; i++)
	{
		int space_count = 0;
		char buf[1000];
		strcpy(buf, result->data[i]);
		while (buf[space_count] == ' ' || buf[space_count] == '\t')
		{
			space_count++;
		}
		strcpy(result->data[i], "");
		strcpy(result->data[i], buf + space_count);
	}
	return 0;
}
int alignRight(struct Text* result, int start_line, int end_line)
{
	int i = 0;
	int longestLineLength = strlen(result->data[findLongestLine(result)]);
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	for (i = start_line; i <= end_line; ++i)
	{
		char buf[1000];
		strcpy(buf, "");
		if (isParagraphSeparator(result->data[i]) == 0) {
			int needed_space = longestLineLength - strlen(result->data[i]);
			if (needed_space > 0) {
				while (needed_space > 0)
				{
					strcat(buf, " ");
					needed_space--;
				}
			}
		}
		strcat(buf, result->data[i]);
		strcpy(result->data[i], "");
		strcpy(result->data[i], buf);
	}
	return 0;
}

int alignCenter(struct Text* result, int start_line, int end_line)
{
	alignLeft(result, start_line, end_line);
	int i = 0;
	int longestLineLength = strlen(result->data[findLongestLine(result)]);
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if (start_line > end_line)
	{
		printf("Invalid operation!\n");
		return 1;
	}
	for (i = start_line; i <= end_line; i++)
	{
		char buf[1000];
		strcpy(buf, "");
		if (isParagraphSeparator(result->data[i]) == 0) {
			int needed_space = longestLineLength - strlen(result->data[i]);
			if (needed_space > 0) {
				if (needed_space % 2 == 0) {
					needed_space = needed_space / 2;
				}
				else {
					needed_space = (needed_space + 1) / 2;
				}
				while (needed_space > 0)
				{
					strcat(buf, " ");
					needed_space--;
				}
			}
		}
		strcat(buf, result->data[i]);
		strcpy(result->data[i], "");
		strcpy(result->data[i], buf);
	}


return 0;

}

int justify(struct Text* result, int start_line, int end_line)
{
	alignLeft(result, start_line, end_line);
	int i = 0;
	int longestLineLength = strlen(result->data[findLongestLine(result)]);
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	for (i = start_line; i < end_line; ++i)
	{
		char buf[1000];
		int j = 0;
		strcpy(buf, "");
		if (isParagraphSeparator(result->data[i]) == 0 && 
			isParagraphSeparator(result->data[i + 1]) == 0) 
		{
			int currentLineLength = strlen(result->data[i]);
			int needed_space = longestLineLength - currentLineLength;
			int spaceCount = countSpaces(result->data[i]);
			int distribution = 0;
			if (needed_space > spaceCount) {
				distribution = needed_space / spaceCount;
			}
			else {
				distribution = 1;
			}
			int diff = needed_space - distribution * spaceCount;
			if (needed_space > 0) {
				while (needed_space > 0 && spaceCount > 0)
				{
					int startIndex = 0;
					for (j = 0; j < strlen(result->data[i]); ++j)
					{
						if (result->data[i][j] == ' ')
						{
							spaceCount--;
							strncat(buf, result->data[i] + startIndex, j - startIndex + 1);
							if (diff > 0)
							{
								strcat(buf, " ");
								diff--;
							}
							int tempDistribution = distribution;
							while (tempDistribution > 0 && needed_space > 0)
							{
								strcat(buf, " ");
								tempDistribution--;
								needed_space--;
							}
							startIndex = j;
							while (result->data[i][startIndex] == ' ' || result->data[i][startIndex] == '\t')
								startIndex++;

						}
						if (result->data[i][j] == '\n')
						{
							strncat(buf, result->data[i] + startIndex, j - startIndex + 1);
						}
					}
				}
			}
			else
			{
				int startIndex = 0;
				while (result->data[i][startIndex] == ' ' || result->data[i][startIndex] == '\t')
					startIndex++;

				strcat(buf, result->data[i] + startIndex);
			}
			strcpy(result->data[i], "");
			strcpy(result->data[i], buf);
		}

	}
	return 0;
}

int paragraph(struct Text* result,int indent, int start_line, int end_line)
{
	int i = 0;
	if (indent > 8)
	{
		indent = 8;
	}
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	int longestLineLength = strlen(result->data[findLongestLine(result)]);
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	for (i = start_line; i <= end_line; ++i)
	{
		if ((isParagraphSeparator(result->data[i - 1]) == 1 && 
		isParagraphSeparator(result->data[i]) == 0) || i ==0)
		{
			char buf[1000];
			strcpy(buf, "");
			int tmpIndent = indent;
			while (tmpIndent > 0) 
			{
				tmpIndent--;
				strcat(buf, " ");
			}
			strcat(buf, result->data[i]);
			strcpy(result->data[i], "");
			strcpy(result->data[i], buf);
		}
	}
	return 0;
}

int list(struct Text* result, const char args[10], int start_line, int end_line)
{
	char type=args[0];
	char special=args[1];
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	int tempType = type;
	switch (tempType)
	{
	case 'n':
		tempType = 1;
		break;
	case 'b':
		tempType = special;
		break;
	default:
		break;
	}
	alignLeft(result, start_line, end_line);
	int i = 0;
	for (i = start_line; i <= end_line; ++i) 
	{
		char buf[1000];
		strcpy(buf, "");
		if (type != 'b')
		{
			if(type =='n')
				sprintf(buf, "%d%c %s", tempType, special, result->data[i]);
			else
				sprintf(buf, "%c%c %s", tempType, special, result->data[i]);
			tempType++;
		}
		else
		{
			sprintf(buf, "%c %s",special, result->data[i]);
		}
		strcpy(result->data[i], "");
		strcpy(result->data[i], buf);
	}
	return 0;
}

int orderList(struct Text* result, const char args[10], int start_line, int end_line)
{
	char type=args[0];
	char special=args[1];
	char order=args[2];
	if (start_line == -1 && end_line == -1)
	{
		start_line = 0;
		end_line = result->lineCount;
	}
	if (end_line == -1 || end_line > result->lineCount)
	{
		end_line = result->lineCount;
	}
	if(start_line > end_line)
	{
		return 1;
	}
	int i = 0;
	for (i = start_line; i <=end_line-1; ++i) 
	{
		int j = 0;
		for(j = i+1;j<=end_line;++j)
		{
			char buf[1000];
			if(strcmp(result->data[i],"") !=0 && strcmp(result->data[j],"") !=0 ){
			int cmpResult = strcmp(result->data[i],result->data[j]);
			if(order == 'a')
			{
				if(cmpResult >= 0)
				{
					strcpy(buf, result->data[i]);
					strcpy(result->data[i], result->data[j]);
					strcpy(result->data[j],buf);
				}
			}
			else
			{
				if(cmpResult <= 0)
				{
					strcpy(buf, result->data[i]);
					strcpy(result->data[i], result->data[j]);
					strcpy(result->data[j],buf);
				}
			}
			
		}
		}
	}
	list(result,args,start_line,end_line);
	
	return 0;
}

