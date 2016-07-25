// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpDbOlk.pas' rev: 6.00

#ifndef OpDbOlkHPP
#define OpDbOlkHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <TypInfo.hpp>	// Pascal unit
#include <OpOlkXP.hpp>	// Pascal unit
#include <OpOutlk.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opdbolk
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TContactInfoKind { citDefault, citBusiness, citPersonal, citNetwork, citMisc };
#pragma option pop

typedef Set<TContactInfoKind, citDefault, citMisc>  TContactInfoKinds;

#pragma option push -b-
enum TContactListSource { cltContacts, cltFolders, cltCustom };
#pragma option pop

typedef void __fastcall (__closure *TGetListSourceEvent)(System::TObject* Sender, /* out */ Opolkxp::_di__Items &ContactList);

class DELPHICLASS TOpContactsDataSet;
class PASCALIMPLEMENTATION TOpContactsDataSet : public Db::TDataSet 
{
	typedef Db::TDataSet inherited;
	
private:
	TContactInfoKinds FContactInfo;
	bool FFoundMatch;
	Opolkxp::_di__Items FItems;
	AnsiString FListName;
	TContactListSource FListSource;
	bool FNeedFreeOutlook;
	TGetListSourceEvent FOnGetListSource;
	Opoutlk::TOpOutlook* FOutlook;
	int FRecordPos;
	void __fastcall DoPostItem(Opolkxp::_di__ContactItem Item, void * Buffer);
	Opolkxp::_di_MAPIFolder __fastcall FindFolderByName(const Opolkxp::_di__Folders Fldrs, const AnsiString Name);
	Opolkxp::_di__Items __fastcall FindMatchingFolder(Opolkxp::_di__Folders Fldrs);
	void __fastcall InitializeRecordBuffer(void * Buffer);
	void __fastcall SetOutlook(Opoutlk::TOpOutlook* Value);
	void __fastcall SetContactInfo(TContactInfoKinds Value);
	void __fastcall SetListSource(const TContactListSource Value);
	void __fastcall SetListName(const AnsiString Value);
	
protected:
	virtual char * __fastcall AllocRecordBuffer(void);
	virtual void __fastcall FreeRecordBuffer(char * &Buffer);
	virtual void __fastcall GetBookmarkData(char * Buffer, void * Data);
	virtual Db::TBookmarkFlag __fastcall GetBookmarkFlag(char * Buffer);
	virtual Db::TGetResult __fastcall GetRecord(char * Buffer, Db::TGetMode GetMode, bool DoCheck);
	virtual int __fastcall GetRecordCount(void);
	virtual int __fastcall GetRecNo(void);
	virtual Word __fastcall GetRecordSize(void);
	virtual void __fastcall InternalAddRecord(void * Buffer, bool Append);
	virtual void __fastcall InternalClose(void);
	virtual void __fastcall InternalDelete(void);
	virtual void __fastcall InternalFirst(void);
	virtual void __fastcall InternalGotoBookmark(void * Bookmark);
	virtual void __fastcall InternalHandleException(void);
	virtual void __fastcall InternalInitFieldDefs(void);
	virtual void __fastcall InternalInitRecord(char * Buffer);
	virtual void __fastcall InternalLast(void);
	virtual void __fastcall InternalOpen(void);
	virtual void __fastcall InternalPost(void);
	virtual void __fastcall InternalSetToRecord(char * Buffer);
	virtual bool __fastcall IsCursorOpen(void);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	virtual void __fastcall SetBookmarkFlag(char * Buffer, Db::TBookmarkFlag Value);
	virtual void __fastcall SetBookmarkData(char * Buffer, void * Data);
	virtual void __fastcall SetFieldData(Db::TField* Field, void * Buffer)/* overload */;
	virtual void __fastcall SetRecNo(int Value);
	
public:
	__fastcall virtual TOpContactsDataSet(Classes::TComponent* AOwner);
	virtual bool __fastcall GetFieldData(Db::TField* Field, void * Buffer)/* overload */;
	
__published:
	__property Active  = {default=0};
	__property TContactInfoKinds ContactInfo = {read=FContactInfo, write=SetContactInfo, default=1};
	__property AnsiString ListName = {read=FListName, write=SetListName};
	__property TContactListSource ListSource = {read=FListSource, write=SetListSource, default=0};
	__property Opoutlk::TOpOutlook* Outlook = {read=FOutlook, write=SetOutlook};
	__property BeforeOpen ;
	__property AfterOpen ;
	__property BeforeClose ;
	__property AfterClose ;
	__property BeforeInsert ;
	__property AfterInsert ;
	__property BeforeEdit ;
	__property AfterEdit ;
	__property BeforePost ;
	__property AfterPost ;
	__property BeforeCancel ;
	__property AfterCancel ;
	__property BeforeDelete ;
	__property AfterDelete ;
	__property BeforeScroll ;
	__property AfterScroll ;
	__property OnDeleteError ;
	__property OnEditError ;
	__property TGetListSourceEvent OnGetListSource = {read=FOnGetListSource, write=FOnGetListSource};
	__property OnNewRecord ;
	__property OnPostError ;
public:
	#pragma option push -w-inl
	/* TDataSet.Destroy */ inline __fastcall virtual ~TOpContactsDataSet(void) { }
	#pragma option pop
	
	
/* Hoisted overloads: */
	
protected:
	inline void __fastcall  SetFieldData(Db::TField* Field, void * Buffer, bool NativeFormat){ TDataSet::SetFieldData(Field, Buffer, NativeFormat); }
	
public:
	inline bool __fastcall  GetFieldData(int FieldNo, void * Buffer){ return TDataSet::GetFieldData(FieldNo, Buffer); }
	inline bool __fastcall  GetFieldData(Db::TField* Field, void * Buffer, bool NativeFormat){ return TDataSet::GetFieldData(Field, Buffer, NativeFormat); }
	
};


class DELPHICLASS TOpBaseData;
class PASCALIMPLEMENTATION TOpBaseData : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	int FPropCount;
	Typinfo::PPropInfo *FPropList;
	WideString __fastcall ReadDispStrProp(void * Disp, unsigned PropIdx);
	void __fastcall WriteDispStrProp(void * Disp, unsigned PropIdx, const WideString PropVal);
	
public:
	__fastcall virtual TOpBaseData(void);
	__fastcall virtual ~TOpBaseData(void);
	void __fastcall ReadPropertiesFromDisp(void * Disp);
	void __fastcall WritePropertiesToDisp(void * Disp);
	/*         class method */ static void __fastcall CreateFieldDefsFromProps(TMetaClass* vmt, Db::TFieldDefs* FieldDefs);
};


typedef TMetaClass*TOpBaseDataClass;

class DELPHICLASS TOpDefaultData;
class PASCALIMPLEMENTATION TOpDefaultData : public TOpBaseData 
{
	typedef TOpBaseData inherited;
	
private:
	AnsiString FMailingAddressCountry;
	AnsiString FMailingAddressState;
	AnsiString FFirstName;
	AnsiString FSuffix;
	AnsiString FMiddleName;
	AnsiString FMailingAddressCity;
	AnsiString FPrimaryTelephoneNumber;
	AnsiString FLastName;
	AnsiString FMailingAddressPostOfficeBox;
	AnsiString FPagerNumber;
	AnsiString FMailingAddressPostalCode;
	AnsiString FMobileTelephoneNumber;
	AnsiString FMailingAddressStreet;
	
__published:
	__property AnsiString FirstName = {read=FFirstName, write=FFirstName, index=120};
	__property AnsiString LastName = {read=FLastName, write=FLastName, index=170};
	__property AnsiString MiddleName = {read=FMiddleName, write=FMiddleName, index=189};
	__property AnsiString Suffix = {read=FSuffix, write=FSuffix, index=237};
	__property AnsiString MailingAddressCity = {read=FMailingAddressCity, write=FMailingAddressCity, index=175};
	__property AnsiString MailingAddressCountry = {read=FMailingAddressCountry, write=FMailingAddressCountry, index=177};
	__property AnsiString MailingAddressPostalCode = {read=FMailingAddressPostalCode, write=FMailingAddressPostalCode, index=179};
	__property AnsiString MailingAddressPostOfficeBox = {read=FMailingAddressPostOfficeBox, write=FMailingAddressPostOfficeBox, index=181};
	__property AnsiString MailingAddressState = {read=FMailingAddressState, write=FMailingAddressState, index=183};
	__property AnsiString MailingAddressStreet = {read=FMailingAddressStreet, write=FMailingAddressStreet, index=185};
	__property AnsiString MobileTelephoneNumber = {read=FMobileTelephoneNumber, write=FMobileTelephoneNumber, index=191};
	__property AnsiString PagerNumber = {read=FPagerNumber, write=FPagerNumber, index=221};
	__property AnsiString PrimaryTelephoneNumber = {read=FPrimaryTelephoneNumber, write=FPrimaryTelephoneNumber, index=225};
public:
	#pragma option push -w-inl
	/* TOpBaseData.Create */ inline __fastcall virtual TOpDefaultData(void) : TOpBaseData() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpBaseData.Destroy */ inline __fastcall virtual ~TOpDefaultData(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpBusinessData;
class PASCALIMPLEMENTATION TOpBusinessData : public TOpBaseData 
{
	typedef TOpBaseData inherited;
	
private:
	AnsiString FCustomerID;
	AnsiString FBusinessFaxNumber;
	AnsiString FBusinessHomePage;
	AnsiString FBusiness2TelephoneNumber;
	AnsiString FBusinessAddressPostOfficeBox;
	AnsiString FBusinessAddressStreet;
	AnsiString FCompanyMainTelephoneNumber;
	AnsiString FAccount;
	AnsiString FAssistantTelephoneNumber;
	AnsiString FAssistantName;
	AnsiString FOfficeLocation;
	AnsiString FBusinessAddressState;
	AnsiString FBillingInformation;
	AnsiString FProfession;
	AnsiString FBusinessAddressPostalCode;
	AnsiString FReferredBy;
	AnsiString FBusinessTelephoneNumber;
	AnsiString FCompanies;
	AnsiString FOrganizationalIDNumber;
	AnsiString FTitle;
	AnsiString FManagerName;
	AnsiString FBusinessAddressCity;
	AnsiString FBusinessAddressCountry;
	AnsiString FDepartment;
	AnsiString FGovernmentIDNumber;
	AnsiString FCompanyName;
	
__published:
	__property AnsiString BillingInformation = {read=FBillingInformation, write=FBillingInformation, index=6};
	__property AnsiString Companies = {read=FCompanies, write=FCompanies, index=12};
	__property AnsiString Account = {read=FAccount, write=FAccount, index=49};
	__property AnsiString AssistantName = {read=FAssistantName, write=FAssistantName, index=53};
	__property AnsiString AssistantTelephoneNumber = {read=FAssistantTelephoneNumber, write=FAssistantTelephoneNumber, index=55};
	__property AnsiString Business2TelephoneNumber = {read=FBusiness2TelephoneNumber, write=FBusiness2TelephoneNumber, index=59};
	__property AnsiString BusinessAddressCity = {read=FBusinessAddressCity, write=FBusinessAddressCity, index=63};
	__property AnsiString BusinessAddressCountry = {read=FBusinessAddressCountry, write=FBusinessAddressCountry, index=65};
	__property AnsiString BusinessAddressPostalCode = {read=FBusinessAddressPostalCode, write=FBusinessAddressPostalCode, index=67};
	__property AnsiString BusinessAddressPostOfficeBox = {read=FBusinessAddressPostOfficeBox, write=FBusinessAddressPostOfficeBox, index=69};
	__property AnsiString BusinessAddressState = {read=FBusinessAddressState, write=FBusinessAddressState, index=71};
	__property AnsiString BusinessAddressStreet = {read=FBusinessAddressStreet, write=FBusinessAddressStreet, index=73};
	__property AnsiString BusinessFaxNumber = {read=FBusinessFaxNumber, write=FBusinessFaxNumber, index=75};
	__property AnsiString BusinessHomePage = {read=FBusinessHomePage, write=FBusinessHomePage, index=77};
	__property AnsiString BusinessTelephoneNumber = {read=FBusinessTelephoneNumber, write=FBusinessTelephoneNumber, index=79};
	__property AnsiString CompanyMainTelephoneNumber = {read=FCompanyMainTelephoneNumber, write=FCompanyMainTelephoneNumber, index=90};
	__property AnsiString CompanyName = {read=FCompanyName, write=FCompanyName, index=92};
	__property AnsiString CustomerID = {read=FCustomerID, write=FCustomerID, index=96};
	__property AnsiString Department = {read=FDepartment, write=FDepartment, index=98};
	__property AnsiString GovernmentIDNumber = {read=FGovernmentIDNumber, write=FGovernmentIDNumber, index=129};
	__property AnsiString JobTitle = {read=FGovernmentIDNumber, write=FGovernmentIDNumber, index=159};
	__property AnsiString ManagerName = {read=FManagerName, write=FManagerName, index=187};
	__property AnsiString OfficeLocation = {read=FOfficeLocation, write=FOfficeLocation, index=199};
	__property AnsiString OrganizationalIDNumber = {read=FOrganizationalIDNumber, write=FOrganizationalIDNumber, index=201};
	__property AnsiString Profession = {read=FProfession, write=FProfession, index=227};
	__property AnsiString ReferredBy = {read=FReferredBy, write=FReferredBy, index=231};
	__property AnsiString Title = {read=FTitle, write=FTitle, index=241};
public:
	#pragma option push -w-inl
	/* TOpBaseData.Create */ inline __fastcall virtual TOpBusinessData(void) : TOpBaseData() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpBaseData.Destroy */ inline __fastcall virtual ~TOpBusinessData(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpPersonalData;
class PASCALIMPLEMENTATION TOpPersonalData : public TOpBaseData 
{
	typedef TOpBaseData inherited;
	
private:
	AnsiString FHomeAddressStreet;
	AnsiString FHomeFaxNumber;
	AnsiString FHomeAddressPostOfficeBox;
	AnsiString FHomeAddressPostalCode;
	AnsiString FHomeAddressState;
	AnsiString FSpouse;
	AnsiString FHomeTelephoneNumber;
	AnsiString FHomeAddressCountry;
	AnsiString FChildren;
	AnsiString FHobby;
	AnsiString FHomeAddressCity;
	AnsiString FHome2TelephoneNumber;
	AnsiString FNickName;
	
__published:
	__property AnsiString Children = {read=FChildren, write=FChildren, index=85};
	__property AnsiString Hobby = {read=FHobby, write=FHobby, index=131};
	__property AnsiString Home2TelephoneNumber = {read=FHome2TelephoneNumber, write=FHome2TelephoneNumber, index=133};
	__property AnsiString HomeAddressCity = {read=FHomeAddressCity, write=FHomeAddressCity, index=137};
	__property AnsiString HomeAddressCountry = {read=FHomeAddressCountry, write=FHomeAddressCountry, index=139};
	__property AnsiString HomeAddressPostalCode = {read=FHomeAddressPostalCode, write=FHomeAddressPostalCode, index=141};
	__property AnsiString HomeAddressPostOfficeBox = {read=FHomeAddressPostOfficeBox, write=FHomeAddressPostOfficeBox, index=143};
	__property AnsiString HomeAddressState = {read=FHomeAddressState, write=FHomeAddressState, index=145};
	__property AnsiString HomeAddressStreet = {read=FHomeAddressStreet, write=FHomeAddressStreet, index=147};
	__property AnsiString HomeFaxNumber = {read=FHomeFaxNumber, write=FHomeFaxNumber, index=149};
	__property AnsiString HomeTelephoneNumber = {read=FHomeTelephoneNumber, write=FHomeTelephoneNumber, index=151};
	__property AnsiString NickName = {read=FNickName, write=FNickName, index=197};
	__property AnsiString Spouse = {read=FSpouse, write=FSpouse, index=235};
public:
	#pragma option push -w-inl
	/* TOpBaseData.Create */ inline __fastcall virtual TOpPersonalData(void) : TOpBaseData() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpBaseData.Destroy */ inline __fastcall virtual ~TOpPersonalData(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpNetworkData;
class PASCALIMPLEMENTATION TOpNetworkData : public TOpBaseData 
{
	typedef TOpBaseData inherited;
	
private:
	AnsiString FEmail2Address;
	AnsiString FEmail3Address;
	AnsiString FComputerNetworkName;
	AnsiString FRadioTelephoneNumber;
	AnsiString FInternetFreeBusyAddress;
	AnsiString FISDNNumber;
	AnsiString FEmail1Address;
	AnsiString FNetMeetingServer;
	AnsiString FNetMeetingAlias;
	AnsiString FTTYTDDTelephoneNumber;
	AnsiString FFTPSite;
	AnsiString FTelexNumber;
	AnsiString FPersonalHomePage;
	AnsiString FWebPage;
	AnsiString FEmail1AddressType;
	AnsiString FEmail2AddressType;
	AnsiString FEmail3AddressType;
	
__published:
	__property AnsiString ComputerNetworkName = {read=FComputerNetworkName, write=FComputerNetworkName, index=94};
	__property AnsiString Email1Address = {read=FEmail1Address, write=FEmail1Address, index=100};
	__property AnsiString Email1AddressType = {read=FEmail1AddressType, write=FEmail1AddressType, index=102};
	__property AnsiString Email2Address = {read=FEmail2Address, write=FEmail2Address, index=106};
	__property AnsiString Email2AddressType = {read=FEmail2AddressType, write=FEmail2AddressType, index=108};
	__property AnsiString Email3Address = {read=FEmail3Address, write=FEmail3Address, index=112};
	__property AnsiString Email3AddressType = {read=FEmail3AddressType, write=FEmail3AddressType, index=114};
	__property AnsiString FTPSite = {read=FFTPSite, write=FFTPSite, index=122};
	__property AnsiString InternetFreeBusyAddress = {read=FInternetFreeBusyAddress, write=FInternetFreeBusyAddress, index=155};
	__property AnsiString ISDNNumber = {read=FISDNNumber, write=FISDNNumber, index=157};
	__property AnsiString NetMeetingAlias = {read=FNetMeetingAlias, write=FNetMeetingAlias, index=193};
	__property AnsiString NetMeetingServer = {read=FNetMeetingServer, write=FNetMeetingServer, index=195};
	__property AnsiString PersonalHomePage = {read=FPersonalHomePage, write=FPersonalHomePage, index=223};
	__property AnsiString RadioTelephoneNumber = {read=FRadioTelephoneNumber, write=FRadioTelephoneNumber, index=229};
	__property AnsiString TelexNumber = {read=FTelexNumber, write=FTelexNumber, index=239};
	__property AnsiString TTYTDDTelephoneNumber = {read=FTTYTDDTelephoneNumber, write=FTTYTDDTelephoneNumber, index=243};
	__property AnsiString WebPage = {read=FWebPage, write=FWebPage, index=255};
public:
	#pragma option push -w-inl
	/* TOpBaseData.Create */ inline __fastcall virtual TOpNetworkData(void) : TOpBaseData() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpBaseData.Destroy */ inline __fastcall virtual ~TOpNetworkData(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpMiscData;
class PASCALIMPLEMENTATION TOpMiscData : public TOpBaseData 
{
	typedef TOpBaseData inherited;
	
private:
	AnsiString FUser3;
	AnsiString FOtherAddressCountry;
	AnsiString FUser2;
	AnsiString FCategories;
	AnsiString FUserCertificate;
	AnsiString FUser4;
	AnsiString FOtherAddressCity;
	AnsiString FYomiFirstName;
	AnsiString FUser1;
	AnsiString FCallbackTelephoneNumber;
	AnsiString FOtherAddressState;
	AnsiString FLanguage;
	AnsiString FYomiLastName;
	AnsiString FOtherFaxNumber;
	AnsiString FOtherAddressPostOfficeBox;
	AnsiString FOtherAddressStreet;
	AnsiString FCarTelephoneNumber;
	AnsiString FYomiCompanyName;
	AnsiString FOtherTelephoneNumber;
	AnsiString FOtherAddressPostalCode;
	
__published:
	__property AnsiString Categories = {read=FCategories, write=FCategories, index=10};
	__property AnsiString CallbackTelephoneNumber = {read=FCallbackTelephoneNumber, write=FCallbackTelephoneNumber, index=81};
	__property AnsiString CarTelephoneNumber = {read=FCarTelephoneNumber, write=FCarTelephoneNumber, index=83};
	__property AnsiString Language = {read=FLanguage, write=FLanguage, index=163};
	__property AnsiString OtherAddressCity = {read=FOtherAddressCity, write=FOtherAddressCity, index=205};
	__property AnsiString OtherAddressCountry = {read=FOtherAddressCountry, write=FOtherAddressCountry, index=207};
	__property AnsiString OtherAddressPostalCode = {read=FOtherAddressPostalCode, write=FOtherAddressPostalCode, index=209};
	__property AnsiString OtherAddressPostOfficeBox = {read=FOtherAddressPostOfficeBox, write=FOtherAddressPostOfficeBox, index=211};
	__property AnsiString OtherAddressState = {read=FOtherAddressState, write=FOtherAddressState, index=213};
	__property AnsiString OtherAddressStreet = {read=FOtherAddressStreet, write=FOtherAddressStreet, index=215};
	__property AnsiString OtherFaxNumber = {read=FOtherFaxNumber, write=FOtherFaxNumber, index=217};
	__property AnsiString OtherTelephoneNumber = {read=FOtherTelephoneNumber, write=FOtherTelephoneNumber, index=219};
	__property AnsiString User1 = {read=FUser1, write=FUser1, index=245};
	__property AnsiString User2 = {read=FUser2, write=FUser2, index=247};
	__property AnsiString User3 = {read=FUser3, write=FUser3, index=249};
	__property AnsiString User4 = {read=FUser4, write=FUser4, index=251};
	__property AnsiString UserCertificate = {read=FUserCertificate, write=FUserCertificate, index=253};
	__property AnsiString YomiCompanyName = {read=FYomiCompanyName, write=FYomiCompanyName, index=257};
	__property AnsiString YomiFirstName = {read=FYomiFirstName, write=FYomiFirstName, index=259};
	__property AnsiString YomiLastName = {read=FYomiLastName, write=FYomiLastName, index=261};
public:
	#pragma option push -w-inl
	/* TOpBaseData.Create */ inline __fastcall virtual TOpMiscData(void) : TOpBaseData() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TOpBaseData.Destroy */ inline __fastcall virtual ~TOpMiscData(void) { }
	#pragma option pop
	
};


struct TContactRec;
typedef TContactRec *PContactRec;

#pragma pack(push, 4)
struct TContactRec
{
	Classes::TList* Data;
	Opolkxp::_di__ContactItem Row;
	int Idx;
	Db::TBookmarkFlag BMFlag;
} ;
#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opdbolk */
using namespace Opdbolk;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpDbOlk
