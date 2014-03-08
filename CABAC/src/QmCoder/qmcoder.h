//---------------------------------------------------------------------------
// FILE: 		qmcoder.h
//
// Author:
// Wenlong Dong			<wdong@sipi.usc.edu>
// 				http://sipi.usc.edu/~wdong/
//
// July 14, 2000		University of Southern California
// LAST UPDATE:         Jan, 25, 2007
//---------------------------------------------------------------------------
#ifndef	_QM_CODING_
#define	_QM_CODING_
//#include <iostream.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

class QM{
protected:
	FILE                  *m_File;
	
private:
	int					count;	// counting of output bitstream
	
	//private varible
	unsigned long         C_register;
	unsigned long         A_interval;
	unsigned              ct;
	unsigned long         sc;
	unsigned char         MPS;
	unsigned              Qe;
	unsigned char     	BP;
	unsigned 				Cx;
	int					bEnd, bFirst;
	int 					next_MPS;
	int					next_st;
	int 					cur_st;
	int                   debug;
	int                   max_context;
	unsigned char			*st_table;
	unsigned char			*mps_table;
	
	//private function
	void AM_Code_Symbol( unsigned char );
	void Code_MPS();
	void Code_LPS();
	void Renorm_e();
	void Byte_out();
	void Output_stacked_zeros();
	void Output_stacked_0xffs();
	void Stuff_0();
	void Discard_final_zeros();
	void Interpret_marker( unsigned char );
	void Clear_final_bits();
	void Add_marker();
	unsigned char AM_decode_Symbol();
	unsigned char Cond_LPS_exchange();
	unsigned char Cond_MPS_exchange();
	void Renorm_d();
	void Byte_in();
	void Unstuff_0();
	void AM_sendout () { fputc(BP, m_File); count++;};
	
public:
	QM(FILE *);
	~QM();
	
	void StartQM(const char *direction);
	//debug flag
	void DebugOn() {this->debug = 1;};
	void DebugOff() {this->debug = 0;};
	//encode SYMBOL by CONTEXT;
	void encode(unsigned char symbol, int context);
	//encode SYMBOL by a fixed probility;
	void encode(unsigned char symbol, int prob, int mps_symbol);
	//counting the output bits during encoding;
	int Counting();
	//flush output to terminate encoding;
	void Flush();
	//decode by context;
	unsigned char decode(int context);
	//decode by a fixed probility;
	unsigned char decode(int prob, int mps_symbol);
	//detecting the end
	int isEnd() {return bEnd;};
	
	//reset all QM to initial condition;
	void reset();
};

#endif // _QM_CODING_
