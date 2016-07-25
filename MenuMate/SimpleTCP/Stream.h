//---------------------------------------------------------------------------
#ifndef StreamH
#define StreamH

#include <Classes.hpp>
#include <Set>
#include "PointsRulesSetUtils.h"

void StreamRead(TStream *Stream,UnicodeString &Value);
void StreamRead(TStream *Stream,int &Value);
void StreamRead(TStream *Stream,long &Value);
void StreamRead(TStream *Stream,unsigned long &Value);
void StreamRead(TStream *Stream,bool &Value);
void StreamRead(TStream *Stream,char &Value);
void StreamRead(TStream *Stream,unsigned char &Value);
void StreamRead(TStream *Stream,double &Value);
void StreamRead(TStream *Stream,TDateTime &Value);
void StreamRead(TStream *Stream,Currency &Value);
void StreamRead(TStream *Stream,unsigned short &Value);
void StreamRead(TStream *Stream,TStringList *Strings);
void StreamRead(TStream *Stream,std::set<byte> &Data);
void StreamRead(TStream *Stream,TPointsRulesSet &Data);
void StreamRead(TStream *Stream, AnsiString &Value);

//---------------------------------------------------------------------------
void StreamWrite(TStream *Stream,UnicodeString &Value);
void StreamWrite(TStream *Stream,int &Value);
void StreamWrite(TStream *Stream,long &Value);
void StreamWrite(TStream *Stream,unsigned long &Value);
void StreamWrite(TStream *Stream,bool &Value);
void StreamWrite(TStream *Stream,char &Value);
void StreamWrite(TStream *Stream,unsigned char &Value);
void StreamWrite(TStream *Stream,double &Value);
void StreamWrite(TStream *Stream,TDateTime &Value);
void StreamWrite(TStream *Stream,Currency &Value);
void StreamWrite(TStream *Stream,unsigned short &Value);
void StreamWrite(TStream *Stream,TStringList *Strings);
void StreamWrite(TStream *Stream,std::set<byte> &Data);
void StreamWrite(TStream *Stream,TPointsRulesSet &Data);
void StreamWrite(TStream *Stream,AnsiString &Value);
//---------------------------------------------------------------------------
#endif
