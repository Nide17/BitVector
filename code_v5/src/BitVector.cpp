/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */

#include "BitVector.h"

// A struct to hold the bit positions and bit values
struct bit_array_struct
{
	int needed_bits_number;
	char *bit_values;

	// set the bit value at the given bit location
	void set_bit(int bit_loc, bool current_bit)
	{
		bit_values[bit_loc] = current_bit;
	}

	// get the bit value at the given bit location
	bool get_bit(int bit_loc)
	{
		return bit_values[bit_loc];
	}
};
/*
Program that Read from a file that has one integer on each line,
output a result file which contains a list of the unique integers in this file sorted in increasing order
*/
int BitVector::readNextItemFromFile(FILE *inputFileStream)
{
	if (!inputFileStream)
	{
		char message[35] = "Cannot open input file for reading";
		throw std::ios_base::failure(message);
	}

	int returnInteger = 1024;

	// Write code here to read the input file.
	int nextItem;

	// get lines of integers until the end of file
	while (true)
	{
		if (feof(inputFileStream))
		{
			break;
		}

		// character array to store one line of input
		char string_integer[2047];
		int integer;

		// parsing one line of input
		if (fgets(string_integer, 2047, inputFileStream))
		{
			int floating = false;
			int i = 0;
			int x;
			char y;

			// checking and skipping lines with floating numbers
			while (i < strlen(string_integer))
			{
				if (string_integer[i] == '.')
				{
					floating = true;
				}
				i++;
			}

			if (floating == true)
			{
				continue;
			}

			// skipping lines with in-between character spaces
			if (sscanf(string_integer, "%d %c", &x, &y) > 1)
			{
				continue;
			}

			// converting the parsed line into integer
			try
			{
				returnInteger = std::stoi(string_integer);
			}
			catch (const std::invalid_argument &e)
			{
				continue;
			}
			catch (const std::out_of_range &e)
			{
				continue;
			}
		}

		return returnInteger;
	}
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

	try
	{
		// initializing a 2D array of 41944 BitArrays
		bit_array_struct two_d_arr[41944];

		// initializing each BitArray to be inside the 2D array
		int index = 0;
		while (index < 41944)
		{
			// initializing each BitArray with 102400 bits
			two_d_arr[index] = {102400, new char[102400]};
			index++;
		}

		while (true)
		{
			// reading one number at a time
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			if (feof(inFileStream))
				break;

			// finding the index of the 2D array
			int index_of_2d_arr = (number + 2147483648) / 102400;
			int index_1d_arr = (number + 2147483648) % 102400;

			// checking if the number is already present in the 2D array
			if (two_d_arr[index_of_2d_arr].get_bit(index_1d_arr))
			{
				fprintf(outFileStream, "%d\n", 0);
			}
			else
			{
				fprintf(outFileStream, "%d\n", 1);
				two_d_arr[index_of_2d_arr].set_bit(index_1d_arr, true);
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
