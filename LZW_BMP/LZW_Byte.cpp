#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <sys/stat.h>
#include <string>

using namespace std;

/* Notes
I'm using uint8_t, and unsigned int of length 8, for bytes instead of chars.
*/

// Builds the initial dictionary, represented by a vector, as all possible byte values. The dictionary will eventually contain "strings" of bytes, so the dictionary is a vector of these strings, or a vector of vectors.
vector< vector<uint8_t> > buildDict()
{
	// Declare the dict, and give it the initial 256 spots.
	vector< vector<uint8_t> > dict;
	dict.reserve(256);

	// Pushes back 256 vectors, each containing just the number 0-256.
	for (int i = 0; i < 256; i++)
		dict.push_back(vector<uint8_t>(1, i));

	return dict;
}

// Takes in the binary file and the size of the file, and returns a vector of codes
vector<uint16_t> compress(uint8_t* &data, int size)
{
	// Build the initial dictionary and other variables
	vector< vector<uint8_t> > dict = buildDict();
	vector<uint16_t> compressed;
	vector<uint8_t> w;

	for (int i = 0; i < size; i++)
	{
		// The next character, c
		uint8_t c = data[i];

		// wc = w + c;
		vector<uint8_t> wc;
		wc.insert(wc.end(), w.begin(), w.end());
		wc.push_back(c);

		// If it's already in the dictionary, go to next loop (continue finding longest)
		// This step is a main issue for speed
		if ( find(dict.begin(), dict.end(), wc) != dict.end() )
			w = wc;

		// If it's not in the dictionary, add it to the output and dictionary
		else
		{
			// Add on the index (find - begin) to the output
			compressed.push_back( find(dict.begin(), dict.end(), w) - dict.begin() );

			// Add on to dictionary by pushing the vector representing wc
			dict.push_back(wc);

			// w = c
			w.clear();
			w.push_back(c);
		}
	}

	// Do the last one
	if (!w.empty())
		compressed.push_back( find(dict.begin(), dict.end(), w) - dict.begin() );

	// Free up memory to prevent leaks, and return.
	delete[] data;
	return compressed;
}

// Takes in an array of codes represented by compressed, and its size.
vector<uint8_t> decompress(uint16_t* &compressed, int size)
{
	// Build the initial dictionary and other variables
	vector< vector<uint8_t> > dict = buildDict();
	vector<uint8_t> w(1, (uint8_t) compressed[0]);
	//w.push_back((uint8_t) compressed[0]);
	vector<uint8_t> decompressed = w;
	vector<uint8_t> entry;

	for (int i = 1; i < size; i++)
	{
		uint16_t k = compressed[i];

		if (k < dict.size())
			entry = dict[k];
		else if (k == dict.size())
		{
			//entry = w + w[0];
			entry.clear();
			entry.insert(entry.end(), w.begin(), w.end());
			entry.push_back(w[0]);
		}
		else
			throw "bad k";

		//decompressed += entry;
		decompressed.insert(decompressed.end(), entry.begin(), entry.end());

		//dict.push_back(w + entry[0]);
		vector<uint8_t> temp;
		temp.insert(temp.end(), w.begin(), w.end());
		temp.push_back(entry[0]);
		dict.push_back(temp);		

		w = entry;
	}

	// Return the array of bytes
	return decompressed;
}

int main(int argc, char *argv[])
{
	// Explains usage
	if (argc != 3)
	{
		printf("Incorrected arguments.\n");
		printf("Compression Usage: 'LZW_Text c <File.ext>\n");
		printf("Decompression Usage: 'LZW_Text d input.lzw\n");
		return -1;
	}

	// Can be "c" for compress or "d" for "decompress"
	string mode = argv[1]; 
	// Gives input file path/name (raw text to compress or binary file to decompress)
	string inputFileName = argv[2]; 

	// Set up arguments for the functions above
	FILE* inputFile, *outputFile;
	int size;

	// Set up input file
	const char* inputFileNameArr = inputFileName.c_str();
	inputFile = fopen(inputFileNameArr, "rb");
	if (!inputFile)
	{
		printf("Could not open input file.\n");
		return -1;
	}

	// Compression Mode
	if ( (mode[0] == 'c') || (mode[0] == 'C') )
	{
		// Get the size of the file using a system utility
		int inputSize;
		struct stat st;
		if (stat(inputFileNameArr, &st) == 0)
			inputSize = st.st_size;
		else
			return -1;

		// Set up the array of bytes
		uint8_t* inputBytes = new uint8_t[inputSize];
		fread(inputBytes, sizeof(uint8_t), inputSize, inputFile);

		// Do the compression
		vector<uint16_t> compressed = compress(inputBytes, inputSize);

		// Set up the output file
		string outputFileName = inputFileName + ".lzw";
		const char* outputFileNameArr = outputFileName.c_str();

		// Opening it in binary format, since we're writing codes
		outputFile = fopen(outputFileNameArr, "wb");
		if (!outputFile)
		{
			printf("Could not open output file.\n");
			return -1;
		}

		// Write to output file
		size = compressed.size();
		fwrite(&size, sizeof(int), 1, outputFile);
		fwrite(compressed.data(), sizeof(unsigned short), size, outputFile);

		// Free up memory
		delete[] inputBytes;
	}

	// Decompression Mode
	else if ( (mode[0] == 'd') || (mode[0] == 'd') )
	{
		// Given an .lzw file, we want the original file
		string outputFileName = inputFileName.substr(0, inputFileName.size() - 4);
		// Tack on a "d" to represent it was decompressed, so it can be differentiated from the original file for testing purposes
		outputFileName += "d";
		const char* outputFileNameArr = outputFileName.c_str();
		fread(&size, sizeof(int), 1, inputFile);

		// Declare a new dynamic array
		uint16_t* compressedArr = new uint16_t[size];
		fread(compressedArr, sizeof(uint16_t), size, inputFile);
		
		// Do the decompression
		vector<uint8_t> decompressed = decompress(compressedArr, size);

		// Write to output file
		outputFile = fopen(outputFileNameArr, "wb");
		fwrite(decompressed.data(), sizeof(uint8_t), decompressed.size(), outputFile);

		// Free up the memory
		delete[] compressedArr;
	}

	// Bad arguments mode
	else
		printf("Bad arguments.");

	// Close files
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}