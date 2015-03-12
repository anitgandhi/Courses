This was a project I did during my semester abroad at Denmark Technical University, for my Data Compression course. The goal of the course project was to implement a compression algorithm of our choice, and analyze the results of its compression. I choose to implement the Lempel Ziv Welsh, a branch of the well known LZ family of lossless compression algorithms. 

While this algorithm is already used in things like text files and GIFs, I originally wanted to apply the algorithm to images and audio. Video compression was outside the scope of the class, and I didn't have enough time to implement direct audio compression. While it can certainly be argued that this is useless, I wanted to implement it both for fun and, of course, to learn.

A quick summary of what I did and the results follow, but the full results can be seen in the Report.pdf.

Note: this was compiled and tested on Windows using MinGW. So, this can be compiled on Linux as well without issue, using gcc; I don't believe I'm using any platform specific libraries. 

This project consists of a few different building blocks:
* LZW_Text.cpp is for text compression. It takes a .txt file (or any other file with just ASCII text), and outputs a compressed binary file.
* LZW_Byte.cpp is for general compression of any file. It takes any input file, and outputs a compressed binary file.
* indexingTolerance.cpp was an intermediate step for me. It doesn't actually implement LZW. Instead it indexes the pixels of an image, while checking if each one is within a certain tolerance (user inputted percentage) of the previous pixels. This uses the EasyBMP library.
* LZW_BMP.cpp was the main part of the project, where I applied the LZW algorithm, with tolerance like above, to an arbitrary BMP image. As above, it uses the EasyBMP library.

Naturally, the larger the input file, the longer time and more memory it took to compress the file. Decompression, in general, was very fast, because it doesn't have to look back at the dictionary to look for existing entries, which was a major source of poor running time during encoding. 

Overall, there are quite a few improvements that can be made with more effort. The code I wrote is not particularly optimized. As seen in the results, the time and memory consumption can be quite high. Further, my choice of vectors for the dictionary data structure was definitely poor, as there was unnecessary overhead that could have been spared if I had used an alternate data structure. This likely would have improved running time as well. The other major improvement I would have liked to have done is variable length codes. As of right now, everything is just on fixed, 16-bit, code word lengths, which is unncessary and inefficient for smaller files.

As I said above, it can be argued that apply LZW to images is not very practical in light of better algorithms. However, when it comes to text compression, I believe that LZW can still be useful when you need to send text files over the Internet. The best example that comes to mind is HTML and CSS compression. These files are the foundation of the Web, and if looked at objectively, are quite repetitive. For example, HTML tags like "<p>" show up very frequently. Since LZW can easily be implemented in scripting languages like Python, PHP, or JavaScript, one could make a system where the server sends compressed HTML and CSS, and the browser can decompress these (very quickly) on the client side to display the page to the user. There's also no need to send a dictionary file with the compressed data.