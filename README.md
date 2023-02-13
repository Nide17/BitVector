## BitVector

This repository contains the code for the class BitVector, which provides a solution for processing a file to indicate unique integers in it. The code uses C++ to implement the solution.

### Description
The code in BitVector.cpp reads an input file line by line, parsing each line as an integer and writing either a 0 or 1 to an output file based on whether the integer has been previously encountered.

The code uses an array "elementsPresent" of size 2 * 2147483648 + 1 to store whether a number has been previously encountered or not. The index in the array for each number is calculated by adding 2147483648 to the number.

__Here's how the code works in detail:__

1. Open the input file and output file for reading and writing respectively.
2. Create an array of size 2 * 2147483648 + 1 to store whether a number has been previously encountered or not.
3. Read the input file line by line until the end of the file is reached.
4. For each line, parse the line as an integer using the "readNextItemFromFile" function.
5. Check if the parsed integer is already present in the "elementsPresent" array.
6. If the integer is not present, write 1 to the output file and set the corresponding element in the "elementsPresent" array to true.
7. If the integer is present, write 0 to the output file.
8. Repeat steps 3-7 until the end of the file is reached.
9. Close the input and output files.

### Requirements

The code requires a C++ compiler to build and run the code. The code has been tested with GCC on Linux and Microsoft Visual Studio on Windows.

### Usage

To use the code, you need to create an instance of the BitVector class and call the processFile method. The method takes two parameters: the input file path and the output file path. The method will process the input file and write the result to the output file.

```c++

#include "BitVector.h"

int main(int argc, char *argv[])
{
    BitVector bitVector;
    bitVector.processFile(argv[1], argv[2]);
    return 0;
}
```

### Exception handling

The code uses the ios_base::failure exception from the C++ Standard Library to handle file I/O errors. The exception message contains the error details.

### Logging

The code uses a logging manager (LogManager) to log messages for debugging and status purposes. The logging level can be set to control the verbosity of the logging messages.

### Limitations

The code is limited to processing files that contain integers that can be represented as a signed 32-bit integer. The code will ignore any lines in the file that are not integers, or integers that are out of the range of a signed 32-bit integer.