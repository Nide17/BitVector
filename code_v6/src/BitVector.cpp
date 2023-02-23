/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"
int num;
int in;
int out;
using namespace std;

// construct the bit array
class BitArray
{
private:
    int num_bits;
    char *bits_arr;

public:
    BitArray() // default constructor
    {
        this->num_bits = 102400;           // initialize the number of bits to 102400
        this->bits_arr = new char[102400]; // allocate memory for the bits array
    }

    BitArray(int num_bits) // constructor with parameter
    {
        this->num_bits = num_bits;           // initialize the number of bits to the parameter
        this->bits_arr = new char[num_bits]; // allocate memory for the bits array
    }

    // set the bit value
    void set_bit(int i, int bit_value)
    {
        bits_arr[i] = bit_value; // set the bit value at the given bit location i
    }

    // get the bit value
    bool get_bit(int i)
    {
        return bits_arr[i]; // return the bit value at the given bit location i
    }
};

// Check if the input string is valid
bool is_valid(const std::string &str)
{
    int check = 0;
    for (int i = 0; i < str.length(); i++)
    {
        // Check if the string contains any characters other than digits, spaces, and the following special characters: ! ? . =
        if (std::isalpha(str[i]) || str[i] == '!' || str[i] == '?' || str[i] == '.' || str[i] == '=')
        {
            check++;
        }

        // Check if the string contains more than one space in a row
        if (i > 0 && str[i] == ' ')
        {
            check++;
        }
    }

    // Check if the string is empty or contains only spaces
    if (str.empty() || str.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
    {
        check++;
    }
    return check == 0;
}

// Check if the input string is a number
bool is_number(const string &str)
{
    // Determine the starting index for the loop, either 1 if the string starts
    int start = (str[0] == '-') ? 1 : 0;

    // Loop to the end of the string, starting from the determined index.

    for (int i = start; i < str.length(); i++)
    {
        // If the character is not a digit, return false
        if (isalpha(str[i]))

        {
            return false;
        }
    }

    // If all characters in the string are digits, return true.
    return true;
}

// Read the next item from the file
int BitVector::processFile(char *inputFilePath, char *outputFilePath)
{

    // Open input file for reading
    FILE *inFileStream = fopen(inputFilePath, "r");

    if (!inFileStream)
    {
        char message[2048];
        sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
        throw std::ios_base::failure(message);
    }

    // Open output file for writing
    FILE *outFileStream = fopen(outputFilePath, "w");

    if (!outFileStream)

    {
        char message[2048];
        sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
        throw std::ios_base::failure(message);
    }

    // Read input file line by line and process each line
    string line;                  // string to store each line
    ifstream file(inputFilePath); // open the input file

    if (file.is_open())
    {
        BitArray *arrays = new BitArray[41944]; // array of bit arrays of size 102400

        // Initialize all bit arrays to 0
        for (int i = 0; i < 41944; i++)
        {
            arrays[i] = BitArray(102400); // initialize the bit array to 0
        }

        // Read each line from the file
        while (getline(file, line))
        {
            if (!is_valid(line))
            {
                continue;
            }
            try
            {
                // Convert the string to an integer
                num = std::stoi(line);

                // Check if the number is in the range of -2147483648 to 2147483647
                out = (num - (-2147483648)) / 102400;
                in = (num - (-2147483648)) % 102400;
            }
            catch (std::out_of_range &ex)
            {
                continue;
            }

            // Check if the number is already set to 1
            if (arrays[out].get_bit(in) == 0)
            {
                fprintf(outFileStream, "%d\n", 1); // write 1 to the output file
                arrays[out].set_bit(in, 1);        // set the bit to 1
            }
            // If the number is already set to 1, write 0 to the output file
            else
            {
                fprintf(outFileStream, "%d\n", 0);
            }
        }
    }
    else

    {
        throw std::ios_base::failure("Unable to open input file");
    }

    // Close files

    fclose(outFileStream);

    fclose(inFileStream);

    // Log status message

    LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::processFile", "Processing complete");
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