//#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include "./EasyBMP/EasyBMP.h"
#include <string>

/* Notes:
This has been explicitly hardcoded to only do 24-bit images. That is, 16,777,216 colors.
*/

using namespace std;

typedef unsigned char color;

// Needed to define the RGBApixel equality check operator because some of the vector functions will call on this
bool operator==(const RGBApixel &lhs, const RGBApixel &rhs)
{
	bool r = (lhs.Red == rhs.Red);
	bool g = (lhs. Green == rhs.Green);
	bool b = (lhs.Blue == rhs.Blue);

	if (r && g && b)
		return true;
	else
		return false;
}

// definining to make code easier
// returns true if lhs is less than rhs by each color
bool operator<=(const RGBApixel &lhs, const RGBApixel &rhs)
{
	bool r = (lhs.Red <= rhs.Red);
	bool g = (lhs. Green <= rhs.Green);
	bool b = (lhs.Blue <= rhs.Blue);

	if (r && g && b)
		return true;
	else
		return false;
}

bool operator>=(const RGBApixel &lhs, const RGBApixel &rhs)
{
	bool r = (lhs.Red >= rhs.Red);
	bool g = (lhs. Green >= rhs.Green);
	bool b = (lhs.Blue >= rhs.Blue);

	if (r && g && b)
		return true;
	else
		return false;
}

// Builds the base dictionary of 16,777,216 pixel/color (the "source alphabet"). Each pixel is like a character, a vector<pixel> is like a string, and a vector< vector<pixel> > is like a vector<string>.
void buildDict(vector< vector<RGBApixel> > &dict)
{
	int numColors = 16777216;
	
	dict.reserve(numColors);
	for (int i = 0; i < numColors; i++)
	{
		RGBApixel p;
		p.Blue = i & 255;
		p.Green = (i >> 8) & 255;
		p.Red = (i >> 16) & 255;
		vector<RGBApixel> pixels(1, p);
		dict.push_back(pixels);
	}
}

color norm(color c, float mult)
{
	if (c*mult <= 0)
		return 0;
	else if (c*mult >= 255)
		return 255;
	else
		return c*mult;
}

// Returns the dictionary index of pixels
// add tolerance for strings of pixels, since individual pixels will have an exact match
int findPixelInDict(vector<RGBApixel> &pixels, vector< vector<RGBApixel> > &dict, float tol)
{
	int numPixels = pixels.size();

	// If only 1 pixel is being searched for, look only in the source alphabet. It must be something from 0 to 16,777,215.
	if (numPixels == 1)
	{
		RGBApixel p = pixels[0];
 		int index = p.Red;
		index = (index << 8) + p.Green;
		index = (index << 8) + p.Blue;
		return index;
	}

	// If there's more than one pixel, then search only in the extended part of dictionary. If it's not found, it returns the invalid index of dict.size()
	/*
	else
	{
		vector< vector<RGBApixel> >::iterator extended = dict.begin() + 16777215;
		int index = find(extended, dict.end(), pixels) - dict.begin();
		return index;
	}
	*/

	
	else
	{
		// Check tolerance to see if it's 0, which means index doesn't need to be searched. 
		if (tol == 0)
			return dict.size();

		float min = 1-tol;
		float max = 1+tol;

		for (int i = 16777216; i < dict.size(); i++)
		{
			// c is the one we're checking against. 
			vector<RGBApixel> c = dict[i];
			int size = c.size();

			// just look for the ones whose sizes match. If they don't match, we know it can't be used, so continue to next iteration
			if (size != numPixels)
				continue;

			// at this point, size will equal numPixels
			//Make a lower and upper pixel for each one in c
			RGBApixel** clu = new RGBApixel*[size];
			for (int j = 0; j < size; j++)
			{
				//c[0] --> array of its associated upper and lower pixel
				RGBApixel cj = c[j];
				color r = cj.Red, g = cj.Green, b = cj.Blue;

				RGBApixel cjl, cju;

				cjl.Red = norm(r, min);
				cjl.Green = norm(g, min);
				cjl.Blue = norm(b, min);

				cju.Red = norm(r, max);
				cju.Green = norm(g, max);
				cju.Blue = norm(b, max);

				RGBApixel* cjlu = new RGBApixel[2];
				cjlu[0] = cjl;
				cjlu[1] = cju;
				clu[j] = cjlu;
			}

			int rgb = 0;
			// see if each pixel is within the tolerance, and if it is, break out of the loop to save time
			for (int k = 0; k < size; k++) // size = numPixels
			{
				bool kr, kg, kb;

				//we want to see if p is in between cl and cu
				RGBApixel cl = clu[k][0];
				RGBApixel cu = clu[k][1];
				RGBApixel p = pixels[k];

				// if it's in the range, increment the rgb counter
				if ( (p >= cl) && (p <= cu) )
					rgb++;
			}

			// delete all memory
			for (int l = 0; l < size; l++)
				delete[] clu[l];
			delete[] clu;
			clu = NULL;

			// if we found a match, return the index. not sure if found has to be set, might be pointless
			if (rgb == size)
				return i;
		}

		// if not found, return the size of the dict
		return dict.size();
	}
	
}

// Applies the LZW compression algorithm to BMP images as if pixels were characters. Note the use of unsigned 32 bit integers; since the base dictionary requires 24 bits, an extra 8 bits should be enough.
vector<uint32_t> compress(BMP &source, float tol)
{
	//vector< vector<RGBApixel> > dict = buildDict();
	vector< vector<RGBApixel> > dict;
	buildDict(dict);
	vector<uint32_t> compressed;
	vector<RGBApixel> w;

	int width = source.TellWidth();
	int height = source.TellHeight();

	for (int i = 0; i < width*height; i++)
	{
		// yw + x = i
		int x = i % width;
		int y = i / width;

		// The next pixel, c
		RGBApixel c = *(source(x,y));

		// wc = w + c;
		vector<RGBApixel> wc;
		wc.insert(wc.end(), w.begin(), w.end());
		wc.push_back(c);

		// If it is in the dictionary
		if ( findPixelInDict(wc, dict, tol) != dict.size() )
			w = wc;

		// If it is not found in the dictionary
		else
		{
			// Add on the index (find - begin) to the output
			compressed.push_back( findPixelInDict(w, dict, tol) );

			// Add on to dictionary by pushing the vector representing wc
			dict.push_back(wc);

			// w = c
			w.clear();
			w.push_back(c);
		}
	}

	// Do the last one
	if (!w.empty())
		compressed.push_back( findPixelInDict(w, dict, tol) );

	return compressed;
}

// Applies the LZW decompression algorithm on the codes. 
vector<RGBApixel> decompress(uint32_t* &compressed, int size)
{
	//vector< vector<RGBApixel> > dict = buildDict();
	vector< vector<RGBApixel> > dict;
	buildDict(dict);

	// w is supposed to start as a pixel 
	vector<RGBApixel> w = dict[ compressed[0] ];
	vector<RGBApixel> decompressed = w;
	vector<RGBApixel> entry;

	for (int i = 1; i < size; i++)
	{
		uint32_t k = compressed[i];

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
		vector<RGBApixel> temp;
		temp.insert(temp.end(), w.begin(), w.end());
		temp.push_back(entry[0]);
		dict.push_back(temp);		

		w = entry;
	}

	return decompressed;
}

int main(int argc, char *argv[])
{
	// Explains usage
	if (argc != 4)
	{
		printf("Incorrected arguments.\n");
		printf("Compression Usage: 'LZW_BMP c <File.bmp> <tolerance>\n");
		printf("Decompression Usage: 'LZW_BMP d <File.bmp.lzw> <anything>\n");
	}

	// Compression mode
	if ( (argv[1][0] == 'c') || (argv[1][0] == 'C') )
	{
		// Create a new image from the input file
		BMP image;
		image.ReadFromFile(argv[2]);

		// Get the width and height
		int w = image.TellWidth();
		int h = image.TellHeight();

		float tolerance = atof(argv[3]);

		// Do the compression
		vector<uint32_t> compressed = compress(image, tolerance);

		// Write to output file
		string inputFileName = argv[2];
		string outputFileName = inputFileName + ".lzw";
		FILE* outputFile = fopen(outputFileName.c_str(), "wb");
		if (!outputFile)
			return -1;
		// Write width and height frst
		fwrite(&w, sizeof(int), 1, outputFile);
		fwrite(&h, sizeof(int), 1, outputFile);
		// Write compressed.size
		int size = compressed.size();
		fwrite(&size, sizeof(int), 1, outputFile);
		fwrite(compressed.data(), sizeof(uint32_t), size, outputFile);
		fclose(outputFile);
	}

	// Decompression mode
	else if ( (argv[1][0] == 'd') || (argv[1][0] == 'D') )
	{
		string inputFileName = argv[2];
		// get rid of ".bmp.lzw" and add "Decompressed.bmp";
		string outputFileName = inputFileName.substr(0, inputFileName.size() - 8) + "Decompressed.bmp";

		// Read in from the file
		FILE* inputFile = fopen(inputFileName.c_str(), "rb");
		int w, h, size;
		fread(&w, sizeof(int), 1, inputFile);
		fread(&h, sizeof(int), 1, inputFile);
		fread(&size, sizeof(int), 1, inputFile);

		// Make a new dynamic array, and read in the compressed data
		uint32_t* compressed = new uint32_t[size];
		fread(compressed, sizeof(uint32_t), size, inputFile);
		fclose(inputFile);

		// Make the dstination image
		BMP decompressedBMP;
		decompressedBMP.SetSize(w, h);
		decompressedBMP.SetBitDepth(24);

		// Do the decompression
		vector<RGBApixel> decompressed = decompress(compressed, size);
		delete[] compressed;

		// Fill the image. Note the access of the pixel data using the matrix indeces to make a regular index (2D --> 1D array)
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				if ( !(decompressedBMP.SetPixel(i,j, decompressed[(j*w) + i])) )
					cout << "set pixel failed" << endl;

		// Write image to file, cleanup
		decompressedBMP.WriteToFile(outputFileName.c_str());
	}

	// Bad arguments mode
	else
		cout << "Bad Arguments." << endl;
}