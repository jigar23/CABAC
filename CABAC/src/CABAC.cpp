//============================================================================
// Name        : CABAC.cpp
// Author      : Jigar Gada
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>
#include "QmCoder/qmcoder.h"
#include "FileIO/fileIO.h"
#include "PreProcessing/bitPlaneMap.h"

void printFileSize(int,char[], char[], int, int,int);
void shiftBit(bool *array, bool nextBit);
void printBoolArray(bool *array);
int getContext(bool *array);
using namespace std;

//Global Variables
char *memblock;
int n;


// Start of the main program //
int main() {

	int flag = 0, preprocess = 0;

	//User Interaction to enter filename
	cout << "-----------------------------" << endl;
	cout << "Enter the *full* file Path with name: " << endl;
	char filename[512];
	cin.getline(filename,512);

	//User Interaction to enter the context
	while(flag == 0){
		cout << "Enter the no. of context: (1,2,3 or 4) " << endl;
		cin >> n;
		//Enter the # of context dependency
		if(n > 4 || n < 1){
			cout << "Invalid Entry..!!!!" << endl;
			cout << "Enter either 1,2,3 or 4" << endl;
		}
		else{
			flag = 1;
		}
	}

	//User interaction to do preprocessing
	flag = 0;
	while(flag == 0){
		cout << "Enter: "<<endl <<
				"1. Do bitMap Preprocessing " << endl
				<<"2. Dont do any Preprocessing (Leave me alone :P)" << endl;
		cin >> preprocess;
		//Enter the # of context dependency
		if(!(preprocess == 1 || preprocess == 2)){
			cout << "Invalid Entry..!!!!" << endl;
			cout << "Enter either 1 or 2" << endl;
		}
		else{
			flag = 1;
		}
	}

//------------------------------------------------------------------------------//
//-----Main code begins--------------------------//

	//Array to store the previous (memory) for context bits
	bool *array = new bool[n];
	for(int i =0; i < n; i++){
		array[i] = 0;
	}

	// Read the Input file
	//	char filename[] = "resources/image.dat";
	//read the contents of the file and store it in memblock
	memblock = readFileByBytes(filename);
	int originalFileSize = FileSizeinBytes;

	//Do PreProcessing
	//This will store the preprocessed file in *PreProcessed.dat* file
	if(preprocess == 1){
		bitmapProcessing(memblock,originalFileSize);
		//Pre-processed Input filename
		char filename1[] = "PreProcessed.dat";
		//read the contents of the file and store it in memblock
		memblock = readFileByBytes(filename1);
	}

	//output the data to this file
	FILE *fp;
	fp=fopen("CABACencoded.dat", "w+");
	//calling the parameterized constructor for CABAC
	QM obj(fp);
	//Initialize the encoder parameters
	obj.StartQM("encode");

	//Read the file till EOF
	while (!checkEOF()) {
		if (ReadBit() == 0){
			int gc =  getContext(array);
			obj.encode(0,getContext(array));
			shiftBit(array,0);
		}
		else{
			int gc = getContext(array);
			obj.encode(1,getContext(array));
			shiftBit(array,1);
		}
	}

	//Flush the remaining contents
	obj.Flush();
	cout << "Done Encoding :)";
	fclose(fp);

	//Output file Size
	char outputFilename[] = "CABACencoded.dat";
	char *opFilename = outputFilename;
	int encodedFileSize = getFileSize(opFilename);

	//Print the result
	printFileSize(n,filename,outputFilename,encodedFileSize,originalFileSize, preprocess);
//
//	cout << "Done/!!";
	return 0;
}

/**
 * This function changes the context when a
 * new symbol is read.
 * Basically it shifts the array to the right.
 * In the bool array, position 0 is the most recent position.
 */
void shiftBit(bool *array, bool nextBit){
	for(int i = n-1; i >= 0; i--){
		if(i == 0){
			array[i] = nextBit;
		}
		else{
			array[i] = array[i-1];
		}
	}
}

void printBoolArray(bool *array){
	for(int i = 0; i < n; i++){
		cout << array[i] << " ";
	}
	cout << endl;
}

/**
 * THis function computes the context based
 * on the bool array.
 * e.g. if the bool array has entries {0,1,1},
 * then the context is 2^(0)*0 + 2^(1)*1 + 2^(2)*1 = 5
 */
int getContext(bool *array){
	int context = 0;
	for(int i = 0; i< n; i++){
		context += pow(2,i)*array[i];
	}
	return context;
}

/**
 * This function prints the output info.
 */
void printFileSize(int n, char inputFileName[], char outputFileName[], int encodedFileSize, int originalFileSize, int pre){
	cout << endl;
	cout << "**********************************" << endl << endl;
	cout << "Input File Name      : " <<inputFileName << endl;
	cout << "Output File Name     : " << outputFileName << endl;
	cout << "Context Dependency   : " << n << endl;
	if(pre == 1){
		cout << "PreProcessing has been done. " << endl;
	}
	else{
		cout << "PreProcessing has *not* been done. " << endl;
	}
	cout << "Original File size   : " << originalFileSize << " bytes" << endl;
	cout << "Compressed File size : " << encodedFileSize << " bytes" << endl;
	cout << "Compression Ratio    : " << (float)encodedFileSize/originalFileSize*100 << "%" << endl << endl;
	cout << "**********************************" << endl;
}
