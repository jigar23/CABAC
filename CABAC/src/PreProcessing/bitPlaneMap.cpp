/*
 * bitPlaneMap.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: jigar
 */
#include "bitPlaneMap.h"


/**
 * This function creates a mask for reading bits
 * starting from MSB -> LSB
 */
void createMask(vector <int> &mask){
	mask.push_back(0x80);
	mask.push_back(0x40);
	mask.push_back(0x20);
	mask.push_back(0x10);
	mask.push_back(0x08);
	mask.push_back(0x04);
	mask.push_back(0x02);
	mask.push_back(0x01);
}


/**
 * This function does the bitmap processing.
 * Read MSB's of all the 8-bit blocks, store it as stream,
 * then read 2nd MSB, store it in stream and so on.
 * e.g.
 * If the memory block has 2 elements:
 * memblock[0] = 01100010;
 * memblock[1] = 01110001;
 *
 * output bit stream: 0011110100001010
 */
void preprocessFile(char *memblock, int fileSize){

	vector <int> mask;
	createMask(mask);

	for(int i = 0; i < 8; i++){
			for(int j = 0; j < fileSize; j++){
				if((memblock[j] & mask.at(i)) != 0){
//					cout << 1 << " ";
					WriteBit(1);
				}
				else{
//					cout << 0 <<" ";
					WriteBit(0);
					}
				}//end of main if

			}
cout << endl;
}


/**
 * This function takes the input data read, does the bitmap
 * processing and stores the result in *PreProcessed.dat* file.
 * @param:
 * 1. char *memblock - Memory read
 * 2. int filesize - size of the input file in bytes
 */

void bitmapProcessing(char *memblock, int fileSize){
	//Write prepare the output file.
	char opFile[] = "PreProcessed.dat";
	char *outputFile = opFile;
	writePrepare(outputFile);

	//pre-process the file
	preprocessFile(memblock, fileSize);
	cout << "Preprocessed File stored in *PreProcessed.dat*" << endl;
	closeFile();

}



