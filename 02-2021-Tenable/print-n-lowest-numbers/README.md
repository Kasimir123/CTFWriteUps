# Tenable Print N Lowest Numbers Write Up

## Details:
Points: 25

Jeopardy style CTF

Category: Code

Comment:

Below is some C/C++ code that reads a list of integers from stdin and calls "PrintNLowestNumbers". Implement PrintNLowestNumbers, which takes an int array of these integers, an int representing the length of that integer array , and an unsigned short "nLowest," which indicates how many of the lowest numbers in the integer array are to be printed to stdout. The printed integers must be printed in order (lowest to highest) and should each be seperated by a space character and nothing else.

``` c
#include <iostream>

void PrintNLowestNumbers(int arr[], unsigned int length, unsigned short nLowest)
{
 // print the n lowest numbers to stdout seperated by space character, do not de-dupe
}

int main()
{
	char input[0x100];
	int integerList[0x100];
	unsigned int length;
	unsigned short nLowest;
	std::cin >> nLowest;
	std::cin >> length;
	for (int i=0;i<length;i++)
		 std::cin >> integerList[i];
	PrintNLowestNumbers(integerList, length, nLowest);
}
```


stdin example:

```
5 8 0 22 -2000 8 62 -26 8 99
```

stdout example:

```
-2000 -26 0 8 8
```

You can ignore the default code below and instead use code above. Once function is implemented, simply copy/paste into box and run. Ensure not to print anything else besides your answer during your code execution!

## Write up:

This challenge said we could use C or C++, I chose c++ but sadly the challenge did not allow the use of vector. Rather than attempting to mess with memory management I just wrote this ugly program to print out the smallest number while replacing it with the largest number. This destroyed the list but functioned as needed.

``` c++
#include <iostream>

void PrintNLowestNumbers(int arr[], unsigned int length, unsigned short nLowest)
{
    int x = arr[0];
    int max = arr[0];
    int index = 0;
 	for (int i = 0; i < nLowest; i++)
 	{
     	for (int j = 0; j < length; j++)
    	{
         	if (arr[j] < x)
         	{
             	x = arr[j];
             	index = j;
         	}
         	if (arr[j] > max)
         	{
             	max = arr[j];
         	}
     	}

     	std::cout << x;
     	std::cout << " ";
     	arr[index] = max;
     	x = max;
 }

std::cout << std::endl;
}

int main()
{
    char input[0x100];
    int integerList[0x100];
    unsigned int length;
    unsigned short nLowest;
    std::cin >> nLowest;
    std::cin >> length;
    for (int i=0;i<length;i++)
         std::cin >> integerList[i];
    PrintNLowestNumbers(integerList, length, nLowest);
}
```