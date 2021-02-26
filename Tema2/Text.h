//Grigore Irina-Domnica
//315CB

#ifndef TEXT_H
#define TEXT_H
struct Text
{
	char data[1000][1000];
	int lineCount;
};

int readFile(const char* path, struct Text* result);
int writeFile(const char* path, const struct Text* source);
void copyText(const struct Text* original, struct Text* result);
void clearText(struct Text* text);
int isParagraphSeparator(const char* line);
int isWordSeparator(char character);
int getParagraph(const struct Text* text, char paragraph[1000000], int* separatorLine);
int removeEndLines(struct Text* text);

int findLongestWord(struct Text* text);
int findLongestLine(struct Text* text);
int countSpaces(char* line);
int skipSpaces(char paragraph[1000000], int startIndex);

int alignLeft(struct Text* result, int start_line, int end_line);
int alignRight(struct Text* result, int start_line, int end_line);
int alignCenter(struct Text* result, int start_line, int end_line);
int justify(struct Text* result, int start_line, int end_line);
int wordWrap(struct Text* result, int max_line_lenght);
int paragraph(struct Text* result, int indent, int start_line, int end_line);
int list(struct Text* result, const char args[10], int start_line, int end_line);
int orderList(struct Text* result, const char args[10], int start_line, int end_line);

#endif
