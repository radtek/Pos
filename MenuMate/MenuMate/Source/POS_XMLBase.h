//---------------------------------------------------------------------------

#ifndef POS_XMLBaseH
#define POS_XMLBaseH

#include <system.hpp>
#include "tinyxml.h"
#include "XMLConst.h"

//---------------------------------------------------------------------------
enum eIntaMateResult {eIMFailed,eIMCache,eIMAccepted};

class TPOS_XMLBase
{
   public :
   TPOS_XMLBase(UnicodeString FileName);
   UnicodeString IntaMateID;

   TiXmlDocument Doc;
   TiXmlDocument ResultDoc;

	eIntaMateResult Result;
	UnicodeString ResultText;

   UnicodeString Name;
   bool Cache;

   void Clear();
   void Reset(UnicodeString FileName, UnicodeString inID = "");
   void SaveToFile();
   void SaveToFile(UnicodeString ID);
   void LoadFromFile(UnicodeString FileName);
	UnicodeString SerializeOut();
	void SerializeIn(UnicodeString);
   virtual void Parse();

};

class TPOS_XMLStatus : public TPOS_XMLBase
{
   private:
   public:
      TPOS_XMLStatus();
      void Build();
      void Parse();
};


#endif
