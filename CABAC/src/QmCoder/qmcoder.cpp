//---------------------------------------------------------------------------
// FILE: 		qmcoder.cpp
//
// Author:
// Wenlong Dong			<wdong@sipi.usc.edu>
// 				http://sipi.usc.edu/~wdong/
//
// July 14, 2000		University of Southern California
// LAST UPDATE:         Jan, 25, 2007
//---------------------------------------------------------------------------
#include <stdlib.h>
#include "qmcoder.h"

#define QMputc(BP, m_File)		if (bFirst) {fputc(BP, m_File);} else {bFirst = 1;};

using namespace std;

int lsz[256]= {
	0x5a1d, 0x2586, 0x1114, 0x080b, 0x03d8,
	0x01da, 0x0015, 0x006f, 0x0036, 0x001a,
	0x000d, 0x0006, 0x0003, 0x0001, 0x5a7f,
	0x3f25, 0x2cf2, 0x207c, 0x17b9, 0x1182,
	0x0cef, 0x09a1, 0x072f, 0x055c, 0x0406,
	0x0303, 0x0240, 0x01b1, 0x0144, 0x00f5,
	0x00b7, 0x008a, 0x0068, 0x004e, 0x003b,
	0x002c, 0x5ae1, 0x484c, 0x3a0d, 0x2ef1,
	0x261f, 0x1f33, 0x19a8, 0x1518, 0x1177,
	0x0e74, 0x0bfb, 0x09f8, 0x0861, 0x0706,
	0x05cd, 0x04de, 0x040f, 0x0363, 0x02d4,
	0x025c, 0x01f8, 0x01a4, 0x0160, 0x0125,
	0x00f6, 0x00cb, 0x00ab, 0x008f, 0x5b12,
	0x4d04, 0x412c, 0x37d8, 0x2fe8, 0x293c,
	0x2379, 0x1edf, 0x1aa9, 0x174e, 0x1424,
	0x119c, 0x0f6b, 0x0d51, 0x0bb6, 0x0a40,
	0x5832, 0x4d1c, 0x438e, 0x3bdd, 0x34ee,
	0x2eae, 0x299a, 0x2516, 0x5570, 0x4ca9,
	0x44d9, 0x3e22, 0x3824, 0x32b4, 0x2e17,
	0x56a8, 0x4f46, 0x47e5, 0x41cf, 0x3c3d,
	0x375e, 0x5231, 0x4c0f, 0x4639, 0x415e,
	0x5627, 0x50e7, 0x4b85, 0x5597, 0x504f,
	0x5a10, 0x5522, 0x59eb
};

int nlps[256]= {
	1, 14, 16, 18, 20,   23, 25, 28, 30, 33,
	35,  9, 10, 12, 15,   36, 38, 39, 40, 42,
	43, 45, 46, 48, 49,   51, 52, 54, 56, 57,
	59, 60, 62, 63, 32,   33, 37, 64, 65, 67,
	68, 69, 70, 72, 73,   74, 75, 77, 78, 79,
	48, 50, 50, 51, 52,   53, 54, 55, 56, 57,
	58, 59, 61, 61, 65,   80, 81, 82, 83, 84,
	86, 87, 87, 72, 72,   74, 74, 75, 77, 77,
	80, 88, 89, 90, 91,   92, 93, 86, 88, 95,
	96, 97, 99, 99, 93,   95,101,102,103,104,
	99,105,106,107,103,  105,108,109,110,111,
	110,112,112
};

int nmps[256]= {
	1,  2,  3,  4,  5,    6,  7,  8,  9, 10,
	11, 12, 13, 13, 15,   16, 17, 18, 19, 20,
	21, 22, 23, 24, 25,   26, 27, 28, 29, 30,
	31, 32, 33, 34, 35,    9, 37, 38, 39, 40,
	41, 42, 43, 44, 45,   46, 47, 48, 49, 50,
	51, 52, 53, 54, 55,   56, 57, 58, 59, 60,
	61, 62, 63, 32, 65,   66, 67, 68, 69, 70,
	71, 72, 73, 74, 75,   76, 77, 78, 79, 48,
	81, 82, 83, 84, 85,   86, 87, 71, 89, 90,
	91, 92, 93, 94, 86,   96, 97, 98, 99,100,
	93,102,103,104, 99,  106,107,103,109,107,
	111,109,111
};

int swit[256]= {
	1,0,0,0,0,    0,0,0,0,0,
	0,0,0,0,1,    0,0,0,0,0,
	0,0,0,0,0,    0,0,0,0,0,
	0,0,0,0,0,    0,1,0,0,0,
	0,0,0,0,0,    0,0,0,0,0,
	0,0,0,0,0,    0,0,0,0,0,
	0,0,0,0,1,    0,0,0,0,0,
	0,0,0,0,0,    0,0,0,0,0,
	1,0,0,0,0,    0,0,0,1,0,
	0,0,0,0,0,    1,0,0,0,0,
	0,0,0,0,0,    1,0,0,0,0,
	1,0,1
};

QM::QM(FILE *FP)
{
	m_File = FP;
	max_context = 4096;
	//max_context = 3;
	st_table = (unsigned char *) calloc(max_context, sizeof(unsigned char));
	mps_table= (unsigned char *) calloc(max_context, sizeof(unsigned char));
}

void QM::StartQM(const char *direction)
{
	if (! strcmp(direction, "encode"))
	{
		sc = 0;
		A_interval = 0x10000; 
		C_register = 0;
		ct = 11;
		
		count = -1;
		debug = 0;
		BP = 0;
		bFirst = 0;
	}
	else if(! strcmp(direction, "decode"))
	{
		count = 0;
		
		MPS = 0;
		A_interval = 0x10000 ;
		C_register = 0 ;
		bEnd = 0;
		
		Byte_in( );
		C_register <<= 8 ;
		Byte_in( );
		C_register <<= 8 ;
		Cx = (unsigned) ((C_register & 0xffff0000) >> 16)  ;	
		
		ct = 0;
		debug = 0;
	}
	else{
		cout << "Command " << direction << " cannot be recognized, please use encode/decode only." << endl;
	}
}


QM::~QM()
{
	free(st_table);
	free(mps_table);
}


void
QM::reset()
{
	for (int i = 0; i < max_context; i++)
	{
		st_table[i] = 0;
		mps_table[i] = 0;
	}
}


void
QM::encode(unsigned char symbol, int context )
{
	if (this->debug) cout <<(char) (symbol+'0') << " " << context << endl;
	
	if (context >= max_context)
	{
		unsigned char *new_st, *new_mps;
		new_st = (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
		new_mps= (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
		memcpy(new_st, st_table, max_context*sizeof(unsigned char));
		memcpy(new_mps,mps_table,max_context*sizeof(unsigned char));
		max_context *= 2;
		free(st_table);
		free(mps_table);
		st_table = new_st;
		mps_table = new_mps;
	}
	
	next_st = cur_st = st_table[context];
	next_MPS = MPS = mps_table[context];
	Qe  = lsz[ st_table[context] ];
	
	if (MPS == symbol)
		Code_MPS( );
	else
		Code_LPS( );
	
	st_table[context] = next_st;
	mps_table[context] = next_MPS;
};


void
QM::encode(unsigned char symbol, int prob, int mps_symbol )
{
	if (this->debug) cout <<(char) (symbol+'0') << " " << prob << endl;
	
	next_st = cur_st = 0;
	next_MPS = MPS = mps_symbol;
	Qe  = prob;
	
	if (MPS == symbol)
		Code_MPS();
	else
		Code_LPS();
};


void
QM::Flush()
{
	Clear_final_bits();
	C_register <<= ct ;
	Byte_out();
	C_register <<= 8;
	Byte_out();
	QMputc(BP, m_File);
	QMputc(0xff, m_File);count++;
	QMputc(0xff, m_File);count++;
}


unsigned char
QM::decode(int context)
{
	if (context >= max_context)
	{
		unsigned char *new_st, *new_mps;
		new_st = (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
		new_mps= (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
		memcpy(new_st, st_table, max_context*sizeof(unsigned char));
		memcpy(new_mps,mps_table,max_context*sizeof(unsigned char));
		max_context *= 2;
		free(st_table);
		free(mps_table);
		st_table = new_st;
		mps_table = new_mps;
	}
	next_st = cur_st = st_table[context];
	next_MPS = MPS = mps_table[context];
	Qe  = lsz[ st_table[context] ];
	unsigned char ret_val = AM_decode_Symbol();
	st_table[context] = next_st;
	mps_table[context] = next_MPS;
	
	if (this->debug) cout <<(char) (ret_val+'0') << " " << context << endl;
	return ret_val;
};


unsigned char
QM::decode(int prob, int mps_symbol)
{
	next_st = cur_st = 0;
	next_MPS = MPS = mps_symbol;
	Qe = prob;
	unsigned char ret_val = AM_decode_Symbol();
	
	if (this->debug) cout <<(char) (ret_val+'0') << " " << prob << endl;
	return ret_val;
};


void
QM::Code_LPS()
{
	A_interval -= Qe ;
	
	if (!(A_interval < Qe))
	{
		C_register += A_interval ;
		A_interval = Qe ;
	}
	
	if (swit[cur_st] == 1)
	{
		next_MPS = 1 - MPS;
	}
	next_st = nlps[cur_st];
	
	Renorm_e();
};


void
QM::Code_MPS()
{
	A_interval -= Qe ;
	
	if (A_interval < 0x8000)
	{       
		if (A_interval < Qe)
		{
			C_register += A_interval ;
			A_interval = Qe ;
		}
		next_st = nmps[cur_st]; 
		Renorm_e();
	}
}


void
QM::Renorm_e()
{
	while (A_interval < 0x8000)
	{
		A_interval <<= 1 ;
		C_register <<= 1 ;
		ct--;
		
		if (ct == 0)
		{
			Byte_out();
			ct = 8 ;
		}
	}
}


void
QM::Byte_out()
{
	unsigned t = C_register>>19;
	
	if (t > 0xff)
	{
		BP++;
		Stuff_0();
		Output_stacked_zeros();
		QMputc(BP, m_File);count++;
		BP = t;
	}
	else
	{
		if (t == 0xff)
		{
			sc++;
		}
		else
		{
			Output_stacked_0xffs();
			QMputc(BP, m_File);count++;
			BP = t;
		}
	}
	C_register &= 0x7ffff;
}


void
QM::Output_stacked_zeros()
{
	while (sc > 0)
	{
		QMputc(BP, m_File);count++;
		BP = 0;
		sc--;
	}
}


void
QM::Output_stacked_0xffs()
{
	while (sc > 0)
	{
		QMputc(BP, m_File);count++;
		BP = 0xff ;
		QMputc(BP, m_File);count++;
		BP = 0 ;
		sc--;
	}
}


void
QM::Stuff_0()
{
	if (BP == 0xff)
	{
		QMputc(BP, m_File);count++;
		BP = 0;
	}
}


void
QM::Clear_final_bits()
{
	unsigned long t;
	t = C_register + A_interval - 1 ;
	t &= 0xffff0000 ;
	
	if (t < C_register) t += 0x8000 ;
	
	C_register = t ;
}


unsigned char
QM::AM_decode_Symbol()
{
	unsigned char D;
	
	A_interval -= Qe ;
	
	if (Cx < A_interval)
	{
		if (A_interval < 0x8000)
		{
			D = Cond_MPS_exchange();
			Renorm_d();
		}
		else
			D = MPS;
	}
	else
	{
		D = Cond_LPS_exchange();
		Renorm_d();
	}
	
	return D;
}


unsigned char
QM::Cond_LPS_exchange()
{
	unsigned char	D;
	unsigned  C_low;
	
	
	if (A_interval < Qe)
	{
		D = MPS;
		Cx -= A_interval;
		C_low = C_register & 0x0000ffff;
		
		C_register = ((unsigned long)Cx<<16) + (unsigned long)C_low;
		A_interval = Qe;
		next_st = nmps[ cur_st ];
	}
	else
	{
		D = 1 - MPS;
		Cx -= A_interval;
		C_low = C_register & 0x0000ffff;
		C_register = ((unsigned long)Cx << 16) + (unsigned long)C_low;
		A_interval = Qe;
		
		if ( swit[ cur_st ]==1 )
		{
			next_MPS = 1-MPS;
		}
		next_st = nlps[ cur_st ];
	}
	
	return D;
}


unsigned char
QM::Cond_MPS_exchange( )
{
	unsigned char	D;
	
	if (A_interval < Qe)
	{
		D = 1 - MPS;
		if (swit[cur_st] == 1)
		{
			next_MPS = 1 - MPS;
		}
		next_st = nlps[cur_st];
	}
	else
	{
		D = MPS;
		next_st = nmps[cur_st];
	}
	
	return D;
}


void
QM::Renorm_d( )
{
	while (A_interval<0x8000)
	{
		if (ct==0)
		{
			if (bEnd == 0) Byte_in();
			ct = 8 ;
		}
		A_interval <<= 1 ;
		C_register <<= 1 ;
		ct--;
	}
	
	Cx = (unsigned) ((C_register & 0xffff0000) >> 16); 
};


void
QM::Byte_in( )
{
	unsigned char B;
	B = fgetc(m_File), count++;
	
	if (B == 0xff)
	{
		Unstuff_0();
	}
	else
	{
		C_register += (unsigned) B << 8 ;
	}
};


void
QM::Unstuff_0( )
{
	unsigned char B;
	B = fgetc(m_File), count++;
	
	if( B == 0 )
	{
		C_register |= 0xff00 ;
	}
	else
	{
		if( B == 0xff )
		{
			//cerr << "\nEnd marker has been met!\n";
			bEnd  = 1;
		} 
	} 
}


int QM::Counting()
{
	if (ct == 0)
	{
		return count*8;
	}
	else
	{
		return count*8+8-ct;
	}
}
