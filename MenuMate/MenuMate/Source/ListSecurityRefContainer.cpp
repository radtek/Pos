//---------------------------------------------------------------------------


#pragma hdrstop

#include "ListSecurityRefContainer.h"
#include "DBSecurity.h"
#include "DBOrder.h"
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TListSecurityRefContainer::TListSecurityRefContainer()
{
	SecurityRef = 0;
}

__fastcall TListSecurityRefContainer::~TListSecurityRefContainer()
{
	SecurityClear();
}

void TListSecurityRefContainer::SetSecurityRefNumber(int inSecurityRef)
{
	if(SecurityRef == 0)
	{
		SecurityRef = inSecurityRef;
	}
}

int TListSecurityRefContainer::GetSecurityRefNumber()
{
	return SecurityRef;
}

void TListSecurityRefContainer::SecurityClear()
{
	while(Count > 0)
	{
		SecurityDelete(SecurityGet(0));
	}
	SecurityRef = 0;
}

TSecurityReference * TListSecurityRefContainer::SecurityGet(int Index)
{
	TSecurityReference *RetVal = NULL;
	if(Count > Index)
	{
		RetVal = (TSecurityReference *)Items[Index];
	}
	return RetVal;
}

TSecurityReference *TListSecurityRefContainer::SecurityGetType(TSecurityTypes Type)
{
	for (int i = 0; i < Count; i++)
	{
		if(SecurityGet(i)->Event == SecurityTypes[Type])
		{
			return SecurityGet(i);
		}
	}
	return NULL;
}

Variant TListSecurityRefContainer::SecurityGetField(TSecurityTypes Type,TSecurityFields Field)
{
	for (int i = 0; i < Count; i++)
	{
		if(SecurityGet(i)->Event == SecurityTypes[Type])
		{
			switch(Field)
			{
				case secfUserKey:
					return SecurityGet(i)->UserKey;
				case secfEvent:
					return SecurityGet(i)->Event;
				case secfFrom:
					return SecurityGet(i)->From;
				case secfTo:
					return SecurityGet(i)->To;
				case secfTimeStamp:
					return SecurityGet(i)->TimeStamp;
			}
		}
	}
	return NULL;
}

void TListSecurityRefContainer::SecurityDelete(TSecurityReference *in)
{
	if(in != NULL)
	{
		if(IndexOf(in) != -1)
		{
			Remove(in);
			delete (TSecurityReference *)in;
			in = NULL;
		}
	}
}

int TListSecurityRefContainer::SecurityAdd(TSecurityReference *in)
{
	return Add(in);
}

int TListSecurityRefContainer::SecurityUpdate(TSecurityTypes Type,TSecurityReference in)
{
	TSecurityReference *OldRef = SecurityGetType(Type);
	int Index = IndexOf(OldRef);
	if(OldRef == NULL)
	{
		OldRef = new TSecurityReference;
		OldRef->SecRefCopy(&in);
		Index = Add(OldRef);
	}
	else
	{
		OldRef->SecRefCopy(&in);
	}
	return Index;
}


int TListSecurityRefContainer::SecurityAddUniqueRef(Database::TDBTransaction &DBTransaction,int OrderKey,TSecurityReference *in, bool isTransactioncommit)
{
	// Is there moer than one Order hooked to this Security Ref.
	if(SecurityGetOrderCount(DBTransaction,SecurityRef) > 1)
	{
		std::auto_ptr<TListSecurityRefContainer> Dupli(new TListSecurityRefContainer);
		// duplicate the orders old security ref and assign a new sec ref to this order.
		Dupli->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
		Dupli->SecurityLoad(DBTransaction,SecurityRef);
		TDBOrder::SetSecurityRef(DBTransaction,OrderKey,SecurityRef,Dupli->SecurityRef);
		TDBSecurity::ProcessSecurity(DBTransaction,Dupli.get(), isTransactioncommit);
		SecurityRef = Dupli->SecurityRef;
	}
	return Add(in);
}


int  TListSecurityRefContainer::SecurityGetOrderCount(Database::TDBTransaction &DBTransaction,int SecRef)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT COUNT(ORDER_KEY) ORDERCOUNT "
		" FROM ORDERS "
		" WHERE "
		"  SECURITY_REF = :SECURITY_REF";
		IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecRef;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("ORDERCOUNT")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}


void TListSecurityRefContainer::SecurityLoad(Database::TDBTransaction &DBTransaction,int SecRef)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT SECURITY_EVENT,FROM_VAL,TO_VAL,NOTE,TERMINAL_NAME,USER_KEY,TIME_STAMP "
		" FROM SECURITY "
		" WHERE "
		"  SECURITY_REF = :SECURITY_REF";
		IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecRef;
		IBInternalQuery->ExecQuery();
		while(!IBInternalQuery->Eof)
		{
			TSecurityReference   *SecRef = new TSecurityReference;
			SecRef->UserKey      = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
			SecRef->Event        = IBInternalQuery->FieldByName("SECURITY_EVENT")->AsString;
			SecRef->From         = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
			SecRef->To           = IBInternalQuery->FieldByName("TO_VAL")->AsString;
			SecRef->Note         = IBInternalQuery->FieldByName("NOTE")->AsString;
			SecRef->TimeStamp    = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			SecurityAdd(SecRef);
			IBInternalQuery->Next();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TListSecurityRefContainer::SecurityCopyList(TListSecurityRefContainer *inSecList)
{
	for(int i = 0; i < inSecList->Count; i++ )
	{
	  TSecurityReference *SecRef = new TSecurityReference;
	  SecRef->SecRefCopy(inSecList->SecurityGet(i));
	  SecurityAdd(SecRef);
	}
}
