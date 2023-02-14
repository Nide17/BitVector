/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"

// IMPLEMENTATION OF BitArray CLASS
// CONSTRUCTOR - IMPLEMENT THE BitArray CLASS FROM BitVector.h
BitArray::BitArray(int numberOfBitsNeeded)
{
	// SETTING THE NUMBER OF BITS NEEDED TO REPRESENT THE INPUT LINE IN AN ARRAY OF BOOLS
	this->numberOfBitsNeeded = numberOfBitsNeeded;

	// CREATING AN ARRAY OF BOOLS TO REPRESENT THE INPUT LINES
	this->bits = new bool[numberOfBitsNeeded];

	// INITIALIZE THE BITS ARRAY WITH ALL ZEROS - USING THE memset FUNCTION
	memset(this->bits, 0, numberOfBitsNeeded);
}

// GETTERS & SETTERS
// SET THE VALUE OF THE BIT AT A SPECIFIC LOCATION IN THE BOOLS ARRAY
void BitArray::setBit(int locationOfBit, bool bitValue)
{
	this->bits[locationOfBit] = bitValue;
}

// GET THE BIT AT THE GIVEN LOCATION IN THE BOOLS ARRAY
bool BitArray::getBit(int locationOfBit)
{
	return this->bits[locationOfBit];
}

// IMPLEMENTATION OF THE LinkedList
// CONSTRUCTOR
LinkedList::LinkedList()
{
	this->head = NULL;
}

// ADD A NODE TO THE LINKED LIST
void LinkedList::addNode(int start, int end, BitArray *bitArray)
{
	// CREATE A NEW NODE
	Node *newNode = new Node();
	// SET THE START, END, BITARRAY AND NEXT NODE
	newNode->start = start;
	newNode->end = end;
	newNode->bitArray = bitArray;
	newNode->next = NULL;

	// IF THE HEAD IS NULL, SET THE HEAD TO THE NEW NODE
	if (this->head == NULL)
		this->head = newNode;

	// ELSE, TRAVERSE THE LINKED LIST AND ADD THE NEW NODE TO THE END
	else
	{
		Node *currentNode = this->head; // SET THE CURRENT NODE TO THE HEAD

		// TRAVERSE THE LINKED LIST UNTIL THE NEXT NODE IS NULL
		while (currentNode->next != NULL)
			currentNode = currentNode->next; // SET THE CURRENT NODE TO THE NEXT NODE

		// ADD THE NEW NODE TO THE END OF THE LINKED LIST
		currentNode->next = newNode;
	}
}

// INSERT A NUMBER IN RESPECTIVE BIT ARRAY
bool LinkedList::isInsertedSeen(int number)
{
	// TRAVERSE THE LINKED LIST
	Node *currentNode = this->head; // SET THE CURRENT NODE TO THE HEAD
	while (currentNode != NULL)
	{
		// IF THE NUMBER IS IN THE RANGE OF THE CURRENT NODE, SET THE BIT AT THE LOCATION OF THE NUMBER TO 1
		if (number >= currentNode->start && number <= currentNode->end)
		{
			// IF THE BIT IS ALREADY SET, JUST RETURN TRUE
			if (currentNode->bitArray->getBit(number - currentNode->start))
			{
				return true;
			}
			// ELSE, SET THE BIT TO 1 AND RETURN 0
			else
			{
				currentNode->bitArray->setBit(number - currentNode->start, true);
				return false;
			}
		}
		// ELSE, MOVE TO THE NEXT NODE
		currentNode = currentNode->next;
	}
}

// DESTRUCTOR
LinkedList::~LinkedList()
{
	Node *currentNode = this->head;
	while (currentNode != NULL)
	{
		Node *nextNode = currentNode->next;
		delete currentNode->bitArray;
		delete currentNode;
		currentNode = nextNode;
	}
}

int BitVector::readNextItemFromFile(FILE *inputFileStream)
{
	if (!inputFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open input file for reading");
		throw std::ios_base::failure(message);
		delete[] message;
	}

	// CODE TO READ LINE BY LINE & RETURN INTEGER
	int returnInteger;

	// READ ONE LINE AT A TIME FROM THE INPUT FILE - UNTIL EOF IS REACHED
	while (true)
	{
		char lineBuffer[2047];
		int part1;
		char part2[2047];
		int number;

		// IF EOF IS NOT REACHED, READ ONE LINE AT A TIME FROM THE INPUT FILE
		if (!feof(inputFileStream) && fgets(lineBuffer, 2047, inputFileStream) != NULL)
		{
			// CHECK THAT INTEGER IF IT IS NOT IN INT RANGE USING strtol FUNCTION
			if (strtol(lineBuffer, NULL, 10) < INT_MIN || strtol(lineBuffer, NULL, 10) > INT_MAX)
				continue;

			// IF THE LINE HAS SPACE IN-BETWEEN CONTENTS, SKIP THE LINE
			if (sscanf(lineBuffer, "%d %s", &part1, part2) != 1)
				continue;

			// IF THE LINE CAN BE CONVERTED TO INTEGER, STORE THE INTEGER IN number
			if (sscanf(lineBuffer, "%d", &number) == 1)
				returnInteger = number;

			// IF THE LINE CAN'T BE CONVERTED TO INTEGER, SKIP THE LINE
			else
				continue;
		}

		// BREAK THE IF ABOVE CONDITION IS NOT SATISFIED
		break;
	}

	// RETURN THE INTEGER TO THE CALLER
	return returnInteger;
}

int BitVector::processFile(char *inputFilePath, char *outputFilePath)
{
	FILE *inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
		delete[] message;
	}

	FILE *outFileStream = fopen(outputFilePath, "w");
	if (!outFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
		delete[] message;
	}

	LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::processFile", "Starting the processing");

	/*
	Your code to process a file should be written after these comments.
	*/
	// READ THE INPUT FILE UNTIL EOF IS REACHED
	try
	{
		// CREATE A LINKED LIST TO STORE THE NODES WITH START, END, BITARRAY AND NEXT NODE INFORMATION OF EACH NODE IN THE LINKED LIST
		LinkedList *linkedList = new LinkedList();

		// CONNECT NODES OF 983055 SIZE EACH, IN THE RANGE -2147483648 AND 2147483647
		for (int i = -2147483648; i <= 2147483647; i += 983055)
		{
			// CREATE A BIT ARRAY OF SIZE 983055
			BitArray *bitArray = new BitArray(983055);

			// ADD A NODE WITH START, END, BITARRAY AND NEXT NODE INFORMATION
			linkedList->addNode(i, i + 983054, bitArray);

			// IF THE END OF THE RANGE IS REACHED, BREAK THE LOOP
			if (i > 2147483647 - 983055)
				break;
		}

		// READING THE INPUT FILE UNTIL EOF IS REACHED
		while (true)
		{
			// READ ONE INTEGER AT A TIME FROM THE INPUT FILE
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			// IF EOF IS REACHED, BREAK THE LOOP
			if (feof(inFileStream))
				break;

			// INSERT THE NUMBER IN THE RESPECTIVE BIT ARRAY OF THE NODE, PRINT TO FILE
			if(linkedList->isInsertedSeen(number))
				fprintf(outFileStream, "%d\n", 0);
			else
				fprintf(outFileStream, "%d\n", 1);
		}
	}

	catch (std::invalid_argument &e)
	{
		LogManager::writePrintfToLog(LogManager::Level::Status,
									 "UniqueInt::processFile",
									 "invalid_argument exception caught=%s\n", e.what());
		if (strcmp(e.what(), "EoF reached") == 0)
		{
			LogManager::writePrintfToLog(LogManager::Level::Status,
										 "UniqueInt::processFile",
										 "OK to continue");
		}
		else
		{
			return -1;
		}
	}

	fflush(outFileStream);
	fclose(outFileStream);

	fclose(inFileStream);
}

int BitVector::generateTestCases(char *outputFilePath, int min, int max, int numberOfEntries, int errorTypes)
{
	if (false)
		return 0;
	FILE *outFileStream = fopen(outputFilePath, "w");
	if (!outFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
		delete[] message;
	}
	int currInt;
	int maxRandomToAddError = 5;
	for (int i = 0; i < numberOfEntries; i++)
	{
		currInt = getRandomInt(min, max);
		if (errorTypes == 0)
		{
			fprintf(outFileStream, "%d\n", currInt);
		}
		if (errorTypes == 1)
		{
			if (getRandomInt(0, maxRandomToAddError) == 3)
			{
				int intNum = getRandomInt(0, maxRandomToAddError);
				float floatNum = ((float)intNum) / 100;
				fprintf(outFileStream, "%f\n", floatNum);
			}
			if (getRandomInt(0, maxRandomToAddError) == 2)
				fprintf(outFileStream, "%c", generateRandomWhiteSpace());
			fprintf(outFileStream, "%d\n", currInt);
		}

		if (errorTypes == 2)
		{
			if (getRandomInt(0, maxRandomToAddError) == 2)
				fprintf(outFileStream, "%c\n", generateRandomWhiteSpace());
			fprintf(outFileStream, "%d\n", currInt);
		}

		if (errorTypes == 3)
		{
			if (getRandomInt(0, maxRandomToAddError) == 2)
			{
				std::string randomStr = genRandomString(3);
				fprintf(outFileStream, "%s\n", randomStr.c_str());
			}
			fprintf(outFileStream, "%d\n", currInt);
		}
	}
	fflush(outFileStream);
	fclose(outFileStream);
}

int BitVector::generateTestCases(char *outputFolderPath)
{
	char filePath[1024];
	int numElements;
	int minValue[2] = {-1023, INT_MIN};
	int maxValue[2] = {1023, INT_MAX};
	char dataType[2][5] = {"easy", "hard"};

	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(filePath, "%s/%s_sample_0%d.txt", outputFolderPath, dataType[k], i + 1);
			numElements = getRandomInt(500000, 800000);
			// numElements = 10;
			LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::generateTestCases",
										 "numElements=%d", numElements);
			generateTestCases(filePath, minValue[k], maxValue[k], numElements, i);

			sprintf(filePath, "%s/%s_test_0%d.txt", outputFolderPath, dataType[k], i + 1);
			numElements = getRandomInt(500000, 800000);
			LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::generateTestCases",
										 "numElements=%d", numElements);
			generateTestCases(filePath, minValue[k], maxValue[k], numElements, i);
		}
	}
}
