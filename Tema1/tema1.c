//Grifore Irina-Domnica 
//315 CB

#include <stdio.h>
#include <inttypes.h>
#include <math.h>


/*
Functia displayBoard afiseaza o harta de joc primita ca parametru

board - parametru ce reprezinta harta de joc 
*/
void displayBoard(uint64_t board) {
	int index = 63;
	while (index >= 0) {
		uint64_t mask = 1;
		mask = mask << index;
		if ((board & mask) != 0)
			printf("#");
		else
			printf(".");
		if (((index) % 8 == 0) && (index != 63))
			printf("\n");
		index--;
	}
	printf("\n");
}

/*
Functia displayPiece afiseaza o piesa de joc primita ca parametru

piece - parametru ce reprezinta piesa de joc 
*/
void displayPiece(uint16_t piece) {
	int index = 15;
	while (index >= 0) {
		uint16_t mask = 1;
		mask = mask << index;
		if ((piece & mask) != 0)
			printf("#");
		else
			printf(".");
		if (((index) % 8 == 0) && (index != 15))
			printf("\n");
		index--;
	}
	printf("\n");
}

/*
Functie utilizata pentru formatarea si afisarea scorului

score - parametru ce reprezinta scorul
*/
void displayScore(double score)
{
	printf("GAME OVER!\n");
	printf("Score:%.2f\n", score);
}

/*
Functia extrage si returneaza o linie, in functie de numarul primit ca parametru

lineNumber - numarul liniei ce se doreste a fi extrasa
*/
uint8_t extractLine(uint64_t board, int lineNumber) {
	if (lineNumber > 7)
		return 0;
	int index = lineNumber * 8;
	uint64_t lineMask = 0xFF;
	lineMask = lineMask << index;
	uint64_t line = board & lineMask;
	line = line >> index;
	return line;
}

/*
Functia verifica daca intre piesa si linia primite ca parametru exista coliziune
Se utilizeaza cand se doreste deplasarea in jos a piesei

line - linia pe care se verifica daca exista coliziune
piece - piesa de joc cu care se vrea verificarea coliziunilor

Rezultat:
1 - coliziune detectata
0 - nu exita coliziune
*/
int isColliding(uint8_t line, uint8_t piece) {
	if ((line & piece) != 0) {
		return 1;
	}
	return 0;
}

/*
Functia verifica daca exista coliziuni intre piesa primita ca parametru si cele 2 linii pe care aceasta se afla
Se utilizeaza cand se doreste deplasarea in stanga sau in dreapta a piesei

piece - piesa de joc cu care se vrea verificarea coliziunilor
lineNumber - numarul liniei curente, cea de a doua calculandu-se prin formula lineNumber + 1
direction - directia in care se doreste miscarea: 
            direction < 0 => miscare spre stanga
		    direction > 0 => miscare spre dreapta

Rezultat:
1 - coliziune detectata
0 - nu exita coliziune
			
*/
int colidesWithLines(uint16_t piece, uint64_t board, int lineNumber, int direction)
{
	uint16_t topMask = 0xFF00;
	uint16_t bottomMask = 0x00FF;
	uint16_t bottomPart = piece & bottomMask;
	uint16_t topPart = piece & topMask;
	topPart = topPart >> 8;
		if (direction < 0)
		{
			topPart = topPart << 1;
			bottomPart = bottomPart << 1;
		}
		else
		{
			topPart = topPart >> 1;
			bottomPart = bottomPart >> 1;
		}

	if (lineNumber < 7)
	{
		uint8_t currentLine = extractLine(board, lineNumber);
		uint8_t prevLine = extractLine(board, lineNumber + 1);

		if (isColliding(currentLine, bottomPart) == 1 || isColliding(prevLine, topPart) == 1)
		{
			return 1;
		}
	}
	else
	{
		uint8_t currentLine = extractLine(board, lineNumber);
		if (isColliding(currentLine, bottomPart) == 1)
		{
			return 1;
		}
	}
	return 0;
}


/*
Functia efectueaza miscarile spre stanga sau spre dreapta ale unei piese

piece - piesa care se doreste a fi miscata
direction - directia in care se doreste miscarea: 
            direction < 0 => miscare spre stanga
		    direction > 0 => miscare spre dreapta
board - harta de joc, necesara pentru verificarea de coliziuni
lineNumber - numarul liniei pe care se doreste efectuarea miscarii
*/
uint16_t shiftPiece(uint16_t piece, int direction, uint64_t board, int lineNumber) {
	uint16_t resultPiece = piece;
	uint16_t topMask = 0xFF00;
	uint16_t bottomMask = 0x00FF;
	uint16_t bottomPart = piece & bottomMask;
	uint16_t topPart = piece & topMask;
	topPart = topPart >> 8;
	if (direction < 0)
	{
                uint16_t tempPiece = piece;
		direction = -direction;
		while (direction > 0)
		{
			short mask = 128;
			if ((topPart&mask)!=0 || (bottomPart&mask)!=0 || colidesWithLines(tempPiece,board,lineNumber,-direction)==1)
			{
				resultPiece = topPart;
				resultPiece = resultPiece << 8;
				resultPiece = resultPiece | bottomPart;
				return resultPiece;
			}
			topPart = topPart << 1;
			bottomPart = bottomPart << 1;
			tempPiece = (topPart<<8)|bottomPart;
			direction--;
		}
	}
	else
	{
		uint16_t tempPiece = piece;
		while (direction > 0)
		{
			short mask = 1;
			if ((topPart & mask) != 0 || (bottomPart & mask) != 0 || colidesWithLines(tempPiece, board, lineNumber, direction) == 1)
			{
				resultPiece = topPart;
				resultPiece = resultPiece << 8;
				resultPiece = resultPiece | bottomPart;
				return resultPiece;
			}
			topPart = topPart >> 1;
			bottomPart = bottomPart >> 1;
			tempPiece = (topPart<<8)|bottomPart;
			direction--;
		}
	}
	resultPiece = topPart;
	resultPiece = resultPiece << 8;
	resultPiece = resultPiece | bottomPart;
	return resultPiece;
}

/*
Functia verifica daca o linie primita ca parametru este completa

line - linia care se doreste a fi verificata

Rezultat:
1 - linia este completa
0 - linia nu este completa
*/
int isFullLine(uint8_t line) {
	uint8_t fullLine = 0xFF;
	if (line == fullLine) {
		return 1;
	}
	return 0;
}


/*
Functia elimina o linie dintr-o harta primita ca parametru

board - harta din care se doreste eliminarea
lineNumber - numarul liniei care se doreste a fi eliminata
*/
uint64_t removeLine(uint64_t board, int lineNumber) {
	uint64_t resultBoard = 0;
	uint64_t beforeLine = 0;
	uint64_t tempLine = 0;
	int line=0;
	for (line = 7; line > lineNumber; --line) {
		tempLine = 0xFF;
		tempLine = tempLine << line * 8;
		beforeLine = beforeLine | tempLine;
	}
	uint64_t afterLine = 0;
	for (line = lineNumber - 1; line >= 0; --line) {
		tempLine = 0xFF;
		tempLine = tempLine << line * 8;
		afterLine = afterLine | tempLine;
	}
	beforeLine = board & beforeLine;
	afterLine = board & afterLine;
	beforeLine = beforeLine >> 8;
	resultBoard = beforeLine + afterLine;
	return resultBoard;
}

/*
Funcita este utilizata pentru eliminarea tuturor liniilor complete dintr-o harta

board - harta din care se doresc eliminarile
*/
uint64_t removeFullLines(uint64_t board)
{
	uint64_t resultBoard = board;
	int currentLine = 7;
	while (currentLine >= 0)
	{
		if (isFullLine(extractLine(resultBoard, currentLine)) == 1)
		{
			resultBoard = removeLine(resultBoard, currentLine);
		}
		--currentLine;
	}
	return resultBoard;
}

/*
Functia are rolul de a numara toate valorile de zero dintr-o harta primita ca parametru

board - harta pentru care se doreste numararea valorilor de zero
*/
int countZeros(uint64_t board)
{
	int result = 0;
	int index = 63;
	uint64_t mask = 1;
	while (index >= 0)
	{
		if ((mask & board) == 0)
			++result;
		mask = mask << 1;
		index--;
	}
	return result;
}

/*
Functia numara toate liniile complete dintr-o harta de joc primita ca parametru

board - harta pentru care se doreste numararea liniilor complete
*/
int countFullLines(uint64_t board)
{
	int result = 0;
	int currentLine = 7;
	while (currentLine >= 0)
	{
		if (isFullLine(extractLine(board, currentLine)) == 1)
		{
			++result;
		}
		--currentLine;
	}
	return result;
}


/*
Functia verifica daca o piesa primita ca parametru poate fi deplasata in jos pe 
o harta, primita de asemenea ca parametru

board - harta pe care se face verificarea
piece - piesa pentru care se face verificarea
lineNumber - linia curenta pe care se afla piesa

Rezultat:
1 - piesa poate fi mutata in jos
0 - piesa nu poate fi mutata in jos
*/
int canMoveDown(uint64_t board, uint16_t piece, int lineNumber)
{
	if (lineNumber == 0)
		return 0;
	uint16_t topMask = 0xFF00;
	uint16_t bottomMask = 0x00FF;
	uint16_t bottomPart = piece & bottomMask;
	uint16_t topPart = piece & topMask;
	topPart = topPart >> 8;
	if (lineNumber == 7)
	{
		uint16_t tempLine = extractLine(board, lineNumber);
		if (isColliding(tempLine, bottomPart) == 1)
		{
			return 0;
		}

	}
	uint16_t line = extractLine(board, lineNumber);
	uint16_t nextLine = extractLine(board, lineNumber - 1);
	if (isColliding(line, topPart) == 1 || isColliding(nextLine, bottomPart) == 1)
	{
		return 0;
	}
	return 1;
}

/*
Functia combina o piesa primita ca parametru cu o harta, primita de asemenea ca parametru
si returneaza harta rezultata

board - harta care se doreste a fi combinata
piece - piesa care se doreste a fi combinata
lineNumber - linia pe care trebuie adaugata piesa
*/
uint64_t mergePiece(uint64_t board, uint16_t piece, int lineNumber)
{
	uint64_t tempBoard = board;
	uint64_t tempPiece = piece;
	if (lineNumber == 7)
	{
		uint16_t bottomMask = 0x00FF;
		uint16_t bottomPart = piece & bottomMask;
		tempPiece = bottomPart;
	}
	tempPiece = tempPiece << (lineNumber * 8);
	tempBoard = tempBoard | tempPiece;
	return tempBoard;
}

/*
Functia reprezinta logica in care celelalte functii se combina pentru a putea juca jocul
*/
void runGame()
{
	uint64_t board;
	int moves;
	int fullLines = 0;
	scanf("%llu", &board);
	displayBoard(board);
	scanf("%d", &moves);
	int gameOver = 0;
	while (moves > 0 && gameOver == 0)
	{
		uint16_t currentPiece = 0;
		scanf("%hd", &currentPiece);
		int currentLine = 8;
		int collided = 0;
		int i=0;
		for (i = 0; i < 8; ++i)
		{
			int currentTransform = 0;
			scanf("%d", &currentTransform);
			if (collided == 0) {
				if (canMoveDown(board, currentPiece, currentLine) == 1)
				{
					currentLine--;
					currentPiece = shiftPiece(currentPiece, currentTransform, board, 	  currentLine);					
					displayBoard(mergePiece(board, currentPiece, currentLine));
				}
				else
				{
					collided = 1;
				}
			}
		}
		if (currentLine == 8)
		{
			displayBoard(board);
			gameOver = 1;
		}
		else 
		{
			board = mergePiece(board, currentPiece, currentLine);
			int currentFullLines = countFullLines(board);
			if(currentFullLines > 0){			
			fullLines += currentFullLines;
			//displayBoard(board);
			board = removeFullLines(board);
			displayBoard(board);
			}
		}
	--moves;
	}
	int zeros = countZeros(board);
	double score = sqrt(zeros)+pow(1.25,fullLines);
	displayScore(score);

}

int main()
{
	runGame();


	return 0;
}
