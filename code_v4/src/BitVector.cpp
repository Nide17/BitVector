/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"
#define BIT_ARRAY_SIZE 102400
#define NUMBER_OF_BIT_ARRAYS 41944
#define MIN_VALUE -2147483648

/**
 * IMPLEMENTATION OF THE BitArray CLASS FROM BitVector.h
 */
// CONSTRUCTING THE ARRAY OF BITS
BitArray::BitArray()
{
	this->numberOfBitsNeeded = BIT_ARRAY_SIZE;
	this->bits = new char[BIT_ARRAY_SIZE];
}

BitArray::BitArray(int numberOfBitsNeeded)
{
	this->numberOfBitsNeeded = numberOfBitsNeeded;
	this->bits = new char[numberOfBitsNeeded];
}

// SETTING THE BIT VALUE
void BitArray::setBit(int locationOfBit, int bitValue)
{
	if (locationOfBit < 0 || locationOfBit >= numberOfBitsNeeded)
	{
		char *message = new char[2048];
		sprintf(message, "Invalid locationOfBitition %d", locationOfBit);
		throw std::out_of_range(message);
		delete[] message;
	}

	bits[locationOfBit] = bitValue;
}

// GETTING THE BIT VALUE
bool BitArray::getBit(int locationOfBit)
{
	if (locationOfBit < 0 || locationOfBit >= numberOfBitsNeeded)
	{
		char *message = new char[2048];
		sprintf(message, "Invalid locationOfBitition %d", locationOfBit);
		throw std::out_of_range(message);
		delete[] message;
	}

	return bits[locationOfBit];
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
		// CREATING A CONTAINING ARRAY OF 41944 BitArray OBJECTS
		BitArray *bitArrays = new BitArray[NUMBER_OF_BIT_ARRAYS];

		// INITIALIZE EVERY BitArray OBJECT IN THE CONTAINING ARRAY
		for (int i = 0; i < NUMBER_OF_BIT_ARRAYS; i++)
		{
			bitArrays[i] = BitArray(BIT_ARRAY_SIZE); // 102400 BITS PER BitArray OBJECT
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

			// FINDING THE INDEX FOR A BitArray THAT CONTAINS THE number FROM THE CONTAINING ARRAY
			int bitArrayIndex = (number - MIN_VALUE) / BIT_ARRAY_SIZE;

			// FINDING THE INDEX OF THE BIT CORRESPONDING TO THE number IN THE BitArray
			int bitIndex = (number - MIN_VALUE) % BIT_ARRAY_SIZE;

			// IF THE BIT IS NOT SET, SET THE BIT TO 1 AND WRITE 1 TO THE OUTPUT FILE
			if (bitArrays[bitArrayIndex].getBit(bitIndex) == 0)
			{
				fprintf(outFileStream, "%d\n", 1);
				bitArrays[bitArrayIndex].setBit(bitIndex, 1);
			}

			else
			{
				// IF THE BIT IS ALREADY SET, WRITE 0 TO THE OUTPUT FILE
				fprintf(outFileStream, "%d\n", 0);
			}
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
