#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

/*Notes
Using codes of size 2 bytes, uint16_t. This uses more space than is ideal, which would be variable width, or 12-bit codes.
*/

// Builds the initial dictionary of all possible ASCII character values. The dictionary will eventually contain strings and not just the original chars, so a vector of strings is being used.
vector<string> buildDict()
{
	// Declare the dict, and give it the initial 256 spots.
	vector<string> dict;
	dict.reserve(256);

	// Pushes back 256 strings, each containing just one character
	for (int i = 0; i < 256; i++)
		dict.push_back(string(1, i));

	return dict;
}

// Takes in a file pointer to a txt file, then reads in character by character, and outputs a vector of codes
vector<uint16_t> compress(FILE* orig)
{
	// Set up the dictionary and necessary variables
	vector<string> dict = buildDict();
	vector<uint16_t> compressed;
	string w = "";
	char c;

	while ( (c = fgetc(orig)) != EOF)
	{
		string wc = w + c;

		// If it's in the dictionary
		if ( find(dict.begin(), dict.end(), wc) != dict.end() )
			w = wc;

		// If it's not in the dictionary
		else
		{
			// Add on the index (find - begin) to the output. This represents the code.
			compressed.push_back( find(dict.begin(), dict.end(), w) - dict.begin() );
			// Add on to dictionary
			dict.push_back(wc);
			w = c;
		}
	}

	// Last one
	if (!w.empty())
		compressed.push_back( find(dict.begin(), dict.end(), w) - dict.begin() );

	return compressed;
}

// Takes in an array of codes, and outputs the original, uncompressed text as a string
string decompress(uint16_t* &compressed, int size)
{
	// Set up the dictionary and necessary variables
	vector<string> dict = buildDict();
	string w(1, compressed[0]);
	string decompressed = w;
	string entry;

	for (int i = 1; i < size; i++)
	{
		int k = compressed[i];

		if (k < dict.size())
			entry = dict[k];
		else if (k == dict.size())
			entry = w + w[0];
		else
			throw "bad k";

		decompressed += entry;
		dict.push_back(w + entry[0]);
		w = entry;
	}

	return decompressed;
}

int main(int argc, char *argv[])
{
	// Explain usage
	if (argc != 4)
	{
		printf("Incorrected number of arguments.\n");
		printf("Compression Usage: 'LZW_Text c <File.txt> output.lzw\n");
		printf("Decompression Usage: 'LZW_Text d input.lzw output.txt\n");
		return -1;
	}

	// Take in the command line arguments
	char* mode = argv[1]; // can be "c" for compress or "d" for "decompress"
	char* inputFileName = argv[2]; // gives input file path/name (raw text to compress or binary file to decompress)
	char* outputFileName = argv[3]; // gives output file path/name (raw text or binary file)

	// Set up the common variables
	vector<uint16_t> compressed;
	uint16_t* compressedArr;
	int size;
	FILE* inputFile, *outputFile;

	// Compression mode
	if ( (mode[0] == 'c') || (mode[0] == 'C') )
	{
		// Set up files
		inputFile = fopen(inputFileName, "r");
		outputFile = fopen(outputFileName, "wb");

		// Do the compression
		compressed = compress(inputFile);
		compressedArr = &compressed[0];

		// Write to output file: size, codes
		size = compressed.size();
		fwrite(&size, sizeof(size), 1, outputFile);
		fwrite(compressedArr, sizeof(uint16_t), size, outputFile);
	}

	// Decompression mode
	else if ( (mode[0] == 'd') || (mode[0] == 'd') )
	{
		// Set up files
		inputFile = fopen(inputFileName, "rb");
		outputFile = fopen(outputFileName, "w");

		// Read in the size, and make an appropriate array
		fread(&size, sizeof(int), 1, inputFile);
		compressedArr = new uint16_t[size];
		fread(compressedArr, sizeof(uint16_t), size, inputFile);

		// Do the decompression
		string decompressed = decompress(compressedArr, size);

		// Write text to file
		fputs(decompressed.c_str(), outputFile);

		// Free up memory
		delete[] compressedArr;
	}

	// Bad arguments mode
	else
		printf("Bad arguments.");

	// Close the files
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}