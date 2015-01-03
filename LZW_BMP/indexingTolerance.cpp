#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include "./EasyBMP/EasyBMP.h"
#include <string>

using namespace std;

typedef unsigned char color;

color norm(color c, float mult)
{
	if (c*mult <= 0)
		return 0;
	else if (c*mult >= 255)
		return 255;
	else
		return c*mult;
}

// This indexes the image. However, it looks for pixels in the index within a certain tolerance. That is, if a pixel is "close" to another pixel already in the index (within the tolerance), it can be replaced by the one in the index
void compress(BMP &source, vector<RGBApixel> &index, uint16_t** &indexedImage, float tol)
{
	int w = source.TellWidth();
	int h = source.TellHeight();

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			// Read in the pixel
			RGBApixel* p = source(x,y);
			color pr = p->Red;
			color pg = p->Green;
			color pb = p->Blue;

			// Upper and lower bounds based on tolerance
			float min = 1-tol;
			float max = 1+tol;
			bool found = false;

			// If it is found in the existing index, then add it to indexedImage and break
			for (int i = 0; i < index.size(); i++)
			{
				// Check tolerance to see if it's 0, which means index doesn't need to be searched
				if (tol == 0)
					break;

				// c is the pixel in the index we're checking against
				RGBApixel c = index[i];
				color cr = c.Red;
				color cg = c.Green;
				color cb = c.Blue;

				// cxl/cxu is the lower and upper bounds for each color, based on the tolerance
				// limited to 0 to 255 using norm function above
				color crl = norm(cr, min);
				color cgl = norm(cg, min);
				color cbl = norm(cb, min);

				color cru = norm(cr, max);
				color cgu = norm(cg, max);
				color cbu = norm(cb, max);

				// Check each of the conditions
				bool r = (pr >= crl) && (pr <= cru);
				bool g = (pg >= cgl) && (pg <= cgu);
				bool b = (pb >= cbl) && (pb <= cbu);

				// If it's in the index already, then add it to the indexedImage, and break out of the loop
				// We're looking for the first one within the tolerance
				if ( r && b && g )
				{
					indexedImage[x][y] = i;
					found = true;
					break;
				}
			}

			// If it wasn't in the index, add the pixel to the index, and the pixel's new index to the indexedImage
			if (!found)
			{
				index.push_back(*p);
				indexedImage[x][y] = index.size() - 1;
			}
		}
	}
}

// For each "code" in indexedImage, find the original pixel in the index, and write it to destination
// This doesn't bring it back to the original state, since that data is gone (because of the tolerance). This simply brings it back to a BMP form.
void decompress(BMP &destination, RGBApixel* &index, uint16_t** &indexedImage, int width, int height)
{
	uint16_t temp;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			temp = indexedImage[x][y];
			destination.SetPixel(x, y, index[temp]);
		}
	}
}

int main(int argc, char *argv[])
{
	// Explain usage
	if (argc != 4)
	{
		printf("Incorrect number of arguments.\n");
		printf("Compression Usage: indexingTolerance c <File Name> <tolerance>\n");
		printf("Decompression Usage: indexingTolerance d <File Name> <anything>");
		return -1;
	}

	// Compression mode
	if ( (argv[1][0] == 'c') || (argv[1][0] == 'C') )
	{
		// Create a new image from the input file
		BMP image;
		image.ReadFromFile(argv[2]);

		// Get the width and height
		int width = image.TellWidth();
		int height = image.TellHeight();

		// Smaller tolerance means better quality
		float tolerance = atof(argv[3]);
		if (tolerance == 0)
		{
			cout << "Tolerance has to be greater than 0, less than or equal to 1." << endl;
			return -1;
		}

		// Index of colors used in the image
		vector<RGBApixel> index;

		// The image represented using the index. This is a 2D array of numbers. Using uint16_ts because this should cover most images with, since we're using 16 bits
		uint16_t** indexedImage = new uint16_t*[width];
		for (int i = 0; i < width; i++)
			indexedImage[i] = new uint16_t[height];

		// Do the "compression"
		compress(image, index, indexedImage, tolerance);
		int indexSize = index.size();

		// Open an output file in binary format
		string outputFileName = argv[2];
		outputFileName += ".lzw";
		FILE* outputFile = fopen(outputFileName.c_str(), "wb");

		// We want to write the following: width, height, index size, index, indexedImage

		// Width, height
		fwrite(&width, sizeof(int), 1, outputFile);
		fwrite(&height, sizeof(int), 1, outputFile);

		// Index size
		fwrite(&indexSize, sizeof(int), 1, outputFile);

		// Index
		for (int i = 0; i < indexSize; i++)
		{
			RGBApixel p = index[i];
			color pArr[] = {p.Red, p.Green, p.Blue};
			fwrite(&pArr[0], sizeof(color), 3, outputFile);
		}

		// indexedImage[i] is an array of size height, which means it's a pointer
		for (int i = 0; i < width; i++)
			fwrite(indexedImage[i], sizeof(uint16_t), height, outputFile);

		// Close the file
		fclose(outputFile);

		// Free up memory
		for (int i = 0; i < width; i++)
			delete[] indexedImage[i];
		delete[] indexedImage;
	}

	// Decompression mode
	else if ( (argv[1][0] == 'd') || (argv[1][0] == 'D') )
	{
		BMP decompressed;
		int width, height, indexSize;

		FILE* inputFile = fopen(argv[2], "rb");
		if (!inputFile)
			cout << "Input file didn't open" << endl;

		// Read in width, height, and indexSize
		fread(&width, sizeof(int), 1, inputFile);
		fread(&height, sizeof(int), 1, inputFile);
		fread(&indexSize, sizeof(int), 1, inputFile);

		// Initialize the image
		decompressed.SetSize(width, height);
		decompressed.SetBitDepth(24);

		// Declare and fill the index
		RGBApixel* index = new RGBApixel[indexSize];

		// Fill the index
		for (int i = 0; i < indexSize; i++)
		{
			color pArr[3];
			fread(&pArr[0], sizeof(color), 3, inputFile);
			RGBApixel p;
			// Have to set each one manually, otherwise it does it backwards for some reason
			p.Red = pArr[0];
			p.Green = pArr[1];
			p.Blue = pArr[2];
			index[i] = p;
		}

		// Declare and fill the indexedImage
		uint16_t** indexedImage = new uint16_t*[width];
		for (int i = 0; i < width; i++)
		{
			indexedImage[i] = new uint16_t[height];
			fread(indexedImage[i], sizeof(uint16_t), height, inputFile);
		}
		fclose(inputFile);		

		// "Decompress" it and write it to the output file
		cout << indexSize << endl;
		decompress(decompressed, index, indexedImage, width, height);
		string inputFileName = argv[2];
		string outputFileName = inputFileName.substr(0, inputFileName.size() - 8) + "Decompressed.bmp";
		decompressed.WriteToFile(outputFileName.c_str());

		// Free up memory
		delete[] index;

		for (int i = 0; i < width; i++)
			delete[] indexedImage[i];
		delete[] indexedImage;
	}

	// Bad arguments mode
	else
		printf("Bad arguments.");

	return 0;
}