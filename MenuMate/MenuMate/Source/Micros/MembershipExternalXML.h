//---------------------------------------------------------------------------

#ifndef MembershipExternalXMLH
#define MembershipExternalXMLH
//---------------------------------------------------------------------------

#include <system.hpp>
#include "tinyxml.h"
#include "XMLConst.h"
#include <map>

//---------------------------------------------------------------------------
enum eMSResult {eMSFailed,eMSAccepted};

class TMSXMLBase
{
	public :
   TiXmlDocument Doc;
   TiXmlDocument ResultDoc;

	eMSResult Result;
	UnicodeString ResultText;

	int TransID;
	bool RepeatTransaction;
	void SaveToFile();
	void Clear();
	void Reset();
	UnicodeString SerializeOut();
	void SerializeIn(UnicodeString);
	TMSXMLBase *SetTransNo(int data);
	TMSXMLBase * SetRepeatTranaction(bool data);
	virtual void Parse();
	virtual void Build() = 0;
};

class TMSXMLStatus : public TMSXMLBase
{
	private:
	public:
	   TMSXMLStatus();
	   void Build();
	   void Parse();
};

class TMSXMLEnquiry : public TMSXMLBase
{
	protected:
		int TerminalID;
		AnsiString TerminalName;
		AnsiString Card;
		AnsiString PIN;


	public:
	   TMSXMLEnquiry();
	   TMSXMLEnquiry * SetTerminalID(int data);
	   TMSXMLEnquiry * SetTerminalName(AnsiString data);
	   TMSXMLEnquiry * SetCard(AnsiString data);
	   TMSXMLEnquiry * SetPIN(AnsiString data);


	   UnicodeString MemberName;
	   UnicodeString AccountNo;
       UnicodeString Note;
       UnicodeString Type;
       int MembersGroup;
	   int PointsBalance;
	   int CompBalance;
	   int Authorised; // (0 == false)
       int PromoBalance;
	   void Build();
	   void Parse();
};

typedef std::map<int,int> TCodeCurrency;
class TMSXMLTransaction : public TMSXMLEnquiry
{
	private:
		TCodeCurrency CodedValues;
	public:
	   TMSXMLTransaction * AddPayment(int ID,int value);

	   UnicodeString MemberName;
	   UnicodeString AccountNo;
	   int PointsBalance;
	   int CompBalance;
	   int Authorised; // (0 == false)
       int PromoBalance;
	   void Build();
	   void Parse();
};


#endif

