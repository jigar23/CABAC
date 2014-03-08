

#include "fileIO.h"

//This is a global variable which stores the file Size
int FileSizeinBytes;
std::ofstream myFile;

using namespace std;

int getFileSize(char *filename){
	streampos size;
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	if(file.is_open()) {
	    size = file.tellg();
	    file.close();
	}else{
		cout << "Unable to open output file.!!!" << endl;
	}
	return size;
}

char* readFileByBytes(char* fileName){
		streampos size;
		  char * memblock;

		  ifstream file (fileName, ios::in|ios::binary|ios::ate);
		  if (file.is_open())
		  {
		    size = file.tellg();
		    memblock = new char [size];
		    file.seekg (0, ios::beg);
		    file.read (memblock, size);
		    file.close();

		    cout << "The entire file content is in memory" <<endl;
		    FileSizeinBytes = size;

//		    cout << "File Size : " << FileSizeinBytes << endl;
		    //delete[] memblock;
		  }
		  else {
			  cout << "Error: Unable to open file..!!!";

		  }

		  return memblock;
}

void WriteByte(unsigned char byte){
	cout << " " << (int)byte ;
}


/**
 * write the file.
 * @param -
 * 1. char data : data you want to write in the file.
 */
void writeFileByBytes(unsigned char data){
	//cout << "data " << (int)data << endl;
			  myFile << data;
}

namespace Wr{
	unsigned char b;
	int s;
}

/**
 * This function write the bits given from MSB-> LSB
 * The first bit is written to the MSB, and so on the
 * 8th bit written to the LSB.
 */

void WriteBit(bool x)
{
    Wr::b |= (x ? 1 : 0) << (7-Wr::s);
    Wr::s++;

    if (Wr::s == 8)
    {
    	writeFileByBytes(Wr::b);
        Wr::b = 0;
        Wr::s = 0;
    }
}


void writeSingleCode(unsigned long code, char size)	{

	for(int i = 0; i < size; i++){
				bool x = ((code & (1 << i))?1:0);
					WriteBit(x);
		}
}

/**
 * Check the last bits status
 */
void checkStatusOfLastBit(){
	if(Wr::s <= 8){
		for (int i = 0; i < Wr::s; i++)
			Wr::b |= 0 << Wr::s;
		writeFileByBytes(Wr::b);
	}
}

/**
 * Open the file once.
 * @param-
 * 1. fileName : name of the file in which you
 * want to write
 */
void writePrepare(char *fileName){
	  //ios::app-> append to end of file.//No need to append
	  //ios::binary-> file is binary not text.
	  //ios::out -> write to the file
	  myFile.open(fileName, ios::out|ios::binary);
}

//for read bits
namespace RB {
	int pointer;
	unsigned char b1;
	int s1;
}

/**
 * This function reads the file bit by bit (starting from MSB)
 * with the help of 3 global variables.
 * pointer -> stores the current location in the memory block.
 * b1 -> stores the current symbol.
 * s1 -> stores the current count from the 8 bits.
 */
bool ReadBit() {
	if (RB::s1 == 0) {
		RB::b1 = memblock[RB::pointer++];
//		cout << (char)b1 << endl;
		RB::s1 = 8;
	}
//	cout << "s: " << RB::s1 << endl;

	bool bit = (RB::b1 >> (RB::s1-1)) & 1;
	RB::s1--;
	return bit;
}

/**
 * This function supports the read function and helps
 * find out the EOF
 */
bool checkEOF() {
	bool bit = 0;
	//IF the count is equal to FileSize and s1-> points to LSB.
	if (RB::pointer == (FileSizeinBytes) && RB::s1 == 0)
		bit = 1;
//	cout << "pointer: " << RB::pointer << endl;
	return bit;
}


//void writeHeader(){
//	//write the total original file Size
//	// int- 4 bytes
//	unsigned int fileSize = FileSizeinBytes;
////	cout << fileSize;
//	unsigned char MSB1 = (fileSize << 24) & 0xFF;
//	writeFileByBytes(MSB1);
//	unsigned char MSB2 = (fileSize << 16) & 0xFF;
//	writeFileByBytes(MSB2);
//	unsigned char LSB1 = (fileSize << 8) & 0xFF;
//	writeFileByBytes(LSB1);
//	unsigned char LSB2 = (fileSize) & 0xFF;
//	writeFileByBytes(LSB2);
//	//No of symbols
//	writeFileByBytes((unsigned char)no_of_symbols);
//	for(int i = 0; i < no_of_symbols; i++){
//		//Write the symbol
//		writeFileByBytes(symbolList.at(i));
//		//write the count
//		unsigned short count = symbolFreq[LetterMap[symbolList.at(i)]];
//		//Get the MSB byte
//		unsigned char MSBbyte = (count << 8) & 0xFF;
//		writeFileByBytes(MSBbyte);
//		//Get the LSB byte
//		unsigned char LSBbyte = (count) & 0xFF;
//		writeFileByBytes(LSBbyte);
//	}
//}

/**
 * Close the file once all write operations done.
 */
void closeFile(){
	myFile.close();
}


