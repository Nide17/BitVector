/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"

struct struct_arr
{
	// pointer to array of bits to store the bit values
	char *bits_ptr = new char[102400];

	// set the bit value at the given bit location
	void set_current_bit(int location_bit, int bit_val)
	{
		bits_ptr[location_bit] = bit_val;
	}

	// get the bit value at the given bit location
	int get_current_bit(int location_bit)
	{
		return bits_ptr[location_bit];
	}
};

// init the container structure array
struct_arr struct_arrays[41944];

// This function prints outputs into the file by iterating through the the array.
void printToFile(FILE *outFileStream, int nbr)
{
	// finding the index of the 2D array
	int arr_index = (nbr + 2147483648) / 102400;
	int bit_index = (nbr + 2147483648) % 102400;

	// checking if the number is already present and printing the output accordingly
	if (struct_arrays[arr_index].get_current_bit(bit_index) == 1)
	{
		fprintf(outFileStream, "%d\n", 0);
	}
	else
	{
		fprintf(outFileStream, "%d\n", 1);
		struct_arrays[arr_index].set_current_bit(bit_index, 1);
	}
}

// This function prints outputs into the file by iterating through the the array.
bool validate(std::string line) {

	int chunk1;
	char chunck2;

	// skipping the line having space in it using sscanf
	if (sscanf(line.c_str(), "%d%c", &chunk1, &chunck2) != 1)
	{
		return false;
	}

	return true;
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

	// Reading file
	std::string line;
	std::ifstream file(inputFilePath);

	// checking if the file is opened to do further operations on it opened.
	if (file.is_open())
	{
		// initializing each BitArray to be inside the 2D array
		int index = 0;
		while (index < 41944)
		{
			// initializing each BitArray with 102400 bits
			struct_arrays[index] = struct_arr();
			index++;
		}

		// looping through the file line by line
		while (getline(file, line))
		{
			int valid_integer;

			// checking if the line is valid or not
			if (!validate(line))
			{
				continue;
			}

			// converting the string to integer	
			try
			{
				valid_integer = stoi(line);
			}
			// catching the exception if the string is not a valid integer
			catch (std::exception &e)
			{
				continue;
			}

			// printing the output to the file
			printToFile(outFileStream, valid_integer);
		}
		// this closes file after operation are done.
		file.close();
	}

	LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::processFile", "Starting the processing");

	/*
	Your code to process a file should be written after these comments.
	*/

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
