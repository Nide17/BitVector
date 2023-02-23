/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"


bool *BoolArray[41945];

// bool integer_validation(const std::string &str)
// {
// 	int i = 0;
// 	if (str[0] == '-')
// 	{
// 		i = 1;
// 	}
// 	for (; i < str.length(); i++)
// 	{
// 		if (!isdigit(str[i]))
// 		{
// 			return false;
// 		}
// 	}
// 	return true;

// }

int set_inner_outer_index(int num, int choice)
{
	int Index1 = floor((num - (-2147483648)) / 102400);
	if (choice == 1)
	{
		return Index1;
	}
	else if (choice == 2)
	{
		int Index2 = num - (-2147483648 + 102400 * Index1);
		return Index2;
	}
}

// std::string trimSTr(std::string str)
// {
// 	std::string result;
// 	auto start = str.find_first_not_of(' ');
// 	if (start == std::string::npos)
// 	{

// 		return result;
// 	}
// 	result = str.substr(start);

// 	auto end = result.find_last_not_of(' ');
// 	if (end != std::string::npos)
// 	{
// 		result.erase(end + 1);
// 	}

// 	return result;
// }
int IsPresent(int ind1, int ind2)
{
	int status = 0;
	if (BoolArray[ind1] == NULL)
	{
		BoolArray[ind1] = new bool[102400];
		BoolArray[ind1][ind2] = true;
		status = 1;
		return status;
	}
	else
	{
		if (BoolArray[ind1][ind2])
		{

			return status;
		}
		else
		{
			BoolArray[ind1][ind2] = true;
			status = 1;
			return status;
		}
	}
}
// bool validLine(std::string linechars)
// {
// 	if (linechars.find_first_not_of(" \t") == std::string::npos || linechars.find_first_of(".") != std::string::npos)
// 	{
// 		return false;
// 	}
// 	return true;
// }
int IsPresent_before(int num)
{
	int Index1, Index2;

	Index1 = set_inner_outer_index(num, 1);
	Index2 = set_inner_outer_index(num, 2);

	return IsPresent(Index1, Index2);
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
}

int BitVector::processFile(char *inputFilePath, char *outputFilePath)
{
	FILE *inFileStream = fopen(inputFilePath, "r");
	FILE *outFileStream = fopen(outputFilePath, "w");
	if (!inFileStream)
	{
		char *message = new char[2048];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
		delete[] message;
	}
	std::string line;
	bool checked = true;

	std::ifstream file(inputFilePath);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			int floating = false;
			int i = 0;
			int x;
			char y;
			// checking if the line is a floating point number
			while (i < line.length())
			{
				if (line[i] == '.')
				{
					floating = true;
					break;
				}
				i++;
			}
			if (floating)
			{
				continue;
			}

			// skipping the line having space in it using sscanf
			if (sscanf(line.c_str(), "%d%c", &x, &y) != 1)
			{
				continue;
			}
			// bool val_ine=validLine(line);
			// if (!val_ine){
			// 	continue;
			// }
			
			// if (integer_validation(trimSTr(line)))
			// {
				// Put stoi in try catch
				try
				{
					int number = stoi(line);
					// int number = stoi(trimSTr(line));
					int seen = IsPresent_before(number);
					fprintf(outFileStream, "%d\n", seen);
					std::cout << number << std::endl;
				}
				catch (std::invalid_argument &e)
				{
					checked = false;
				}
				catch (std::out_of_range &e)
				{
					checked = false;
				}
				catch (std::exception &e)
				{
					checked = false;
				}
			// }
			
		}
	}
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