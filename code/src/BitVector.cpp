/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"

/**
 * IMPLEMENTATION OF THE BitArray CLASS FROM BitVector.h
 */
// CONSTRUCTOR - INITIALIZE numberOfBitsNeeded TO REPRESENT THE INPUT LINE IN AN ARRAY OF BOOLS
BitArray::BitArray(int numberOfBitsNeeded)
{
	// SETTING THE numberOfBitsNeeded TO REPRESENT THE INPUT LINE IN AN ARRAY OF BOOLS
	this->numberOfBitsNeeded = numberOfBitsNeeded;

	// CREATING AN ARRAY OF BOOLS TO REPRESENT THE CURRENT INPUT LINE
	this->bits = new bool[numberOfBitsNeeded];
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

// DESTRUCTOR TO FREE THE MEMORY ALLOCATED FOR THE BitArray OBJECT
BitArray::~BitArray()
{
	delete[] this->bits; // FREE THE MEMORY ALLOCATED FOR THE BOOLS ARRAY
}

// -----------------------------------------------------------------------------------------------
/**
 * IMPLEMENTATION OF THE Node CLASS
 */
// CONSTRUCTOR - INITIALIZE THE NODE WITH THE START AND END OF THE RANGE
Node::Node(int start, int end, BitArray *bitArray)
{
	this->start = start;	   // SET THE START OF THE RANGE
	this->end = end;		   // SET THE END OF THE RANGE
	this->bitArray = bitArray; // SET THE POINTER TO THE BitArray OBJECT
}
// DESTRUCTOR TO FREE THE MEMORY ALLOCATED FOR THE Node OBJECT
Node::~Node()
{
	delete this->bitArray; // FREE THE MEMORY ALLOCATED FOR THE BitArray OBJECT
}

// -----------------------------------------------------------------------------------------------
/**
 * IMPLEMENTATION OF THE LinkedList CLASS FROM BitVector.h
 */
// CONSTRUCTOR - INITIALIZE HEAD NODE TO NULL
LinkedList::LinkedList()
{
	this->head = NULL;  // SET THE HEAD NODE TO NULL - EMPTY LINKED LIST
}

// IMPLEMENT FUNCTION TO ADD A NODE TO THE END OF THE LINKED LIST
void LinkedList::addNode(int start, int end, BitArray *bitArray)
{
	// ALLOCATE MEMORY FOR THE NEW NODE - CONSTRUCT THE NEW NODE
	Node *newNode = new Node(start, end, bitArray);
	newNode->next = NULL; // SET THE NEXT NODE TO NULL

	// IF THE LinkedList's HEAD IS NULL, SET THE NEW NODE AS THE HEAD OF THE LINKED LIST
	if (this->head == NULL)
		this->head = newNode;

	// ELSE, TRAVERSE THE LINKED LIST AND ADD THE NEW NODE TO THE END OF THE LINKED LIST
	else
	{
		// MAKE THE CURRENT NODE POINT AS THE HEAD OF THE LINKED LIST
		Node *currentNode = this->head;

		// TRAVERSE THE LINKED LIST UNTIL THE END OF THE LIST IS REACHED
		while (currentNode->next != NULL)
			currentNode = currentNode->next; // SET THE CURRENT NODE TO THE NEXT NODE

		// ADD THE NEW CONSTRUCTED NODE TO THE END OF THE LIST
		currentNode->next = newNode;
	}
}

// CHECKING AND SETTING THE BIT AT THE LOCATION OF THE NUMBER IN THE BIT ARRAY
bool LinkedList::isSeenBefore(int number)
{
	// MAKE THE CURRENT NODE POINT TO THE HEAD OF THE LINKED LIST
	Node *currentNode = this->head;

	while (currentNode != nullptr)
	{
		if (number >= currentNode->start && number <= currentNode->end)
		{
			// IF THE BIT IS ALREADY SET, JUST RETURN TRUE
			if (currentNode->bitArray->getBit(number - currentNode->start))
			{
				return true;
			}
			// ELSE, SET THE BIT TO true(1) AND RETURN false (0)
			else
			{
				currentNode->bitArray->setBit(number - currentNode->start, true);
				return false;
			}
		}
		currentNode = currentNode->next;
	}
	return false;
}

// DESTRUCTOR TO FREE THE MEMORY ALLOCATED FOR THE LINKED LIST
LinkedList::~LinkedList()
{
	Node *currentNode = head;
	while (currentNode != nullptr)
	{
		Node *next = currentNode->next;
		delete currentNode; // delete Node object
		currentNode = next;
	}
}

// -----------------------------------------------------------------------------------------------
// READING INPUT FROM FILE, LINE BY LINE
int BitVector::readNextItemFromFile(FILE *inputFileStream)
{
	if (!inputFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open input file for reading");
		throw std::ios_base::failure(message);
		delete[] message;
	}

	// READ LINE BY LINE & RETURN INTEGER IF THE LINE IS VALID
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

// PROCESSING THE INPUT FILE, LINE BY LINE
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
			if (linkedList->isSeenBefore(number))
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

// GENERATE TEST CASES FOR THE PROGRAM TO TEST THE FUNCTIONALITY
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

// GENERATE TEST CASES FOR THE PROGRAM TO TEST THE FUNCTIONALITY
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
