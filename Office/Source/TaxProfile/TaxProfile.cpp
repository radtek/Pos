//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfile.h"

//---------------------------------------------------------------------------
TaxProfile::TaxProfile(int dbKey)
{
   taxProfileName = "";
   taxPercentage  = 0;
   taxProfileType = DefaultNULLTax;
   taxPriority    = 0;
   taxSelectable  = true;
   _taxProfileDBKey = dbKey;
   //taxCode        = 0;
}

TaxProfile::TaxProfile()
{
   taxProfileName = "";
   taxPercentage  = 0;
   taxProfileType = DefaultNULLTax;
   taxPriority    = 0;
   taxSelectable  = true;
   _taxProfileDBKey = 0;
   //taxCode        = 0;
}

TaxProfile::TaxProfile(AnsiString name, Percentage perc, TaxType type, Priority priority)//, int code)
{
   taxProfileName = name;
   taxPercentage  = perc;
   taxProfileType = type;
   taxPriority    = priority;
   taxSelectable  = true;
   _taxProfileDBKey = 0;
   //taxCode        = code;
}

void TaxProfile::writetaxProfileName( AnsiString intaxProfileName )
{
   _taxProfileName = intaxProfileName;
}
void TaxProfile::writetaxProfileType( TaxType intaxProfileType )
{
   _taxProfileType = intaxProfileType;
}
void TaxProfile::writetaxPercentage( Percentage intaxPercentage )
{
   _taxPercentage =  intaxPercentage;
}
void   TaxProfile::writetaxPriority( Priority intaxPriority )
{
   _taxPriority = intaxPriority;
}
void TaxProfile::writetaxSelectable( bool intaxSelectable )
{
   _taxSelectable = intaxSelectable;
}
//-- to be incorpotated in future--//
/*void TaxProfile::writetaxCode( int intaxCode )
{
   _taxCode = intaxCode;
}*/

TaxType TaxProfile::Convert(int type)
{
    TaxType returnType;

    switch(type)
    {
        case 0:
            returnType = SalesTax;
            break;
        case 1:
            returnType = PurchaseTax;
            break;
        case 2:
            returnType = ServiceCharge;
            break;
        case 3:
            returnType = ServiceChargeTax;
            break;
        case 4:
            returnType = LocalTax;
            break;
        case 5:
            returnType = ProfitTax;
            break;
        default:
            returnType = DefaultNULLTax;
            break;
    }

    return returnType;
}

TaxType TaxProfile::Convert(AnsiString type)
{
   AnsiString typeStr = type.UpperCase();
   if(stringToEnum.find(typeStr) != stringToEnum.end())
      return stringToEnum[typeStr];
   else
      return DefaultNULLTax;
}

AnsiString TaxProfile::Convert(TaxType type)
{
   AnsiString result = "";
   try
   {
      if(type == DefaultNULLTax)
      {
          result = "";
      }
      else
      {
           result = TaxStringType[type];
      }
   }
   catch(...)
   {
      result = "";
   }

   return result;
}

TaxMap TaxProfile::ret()
{
   TaxMap r;
   r[TaxStringType[0].UpperCase()] = SalesTax;
   r[TaxStringType[1].UpperCase()] = PurchaseTax;
   r[TaxStringType[2].UpperCase()] = ServiceCharge;
   r[TaxStringType[3].UpperCase()] = ServiceChargeTax;
   r[TaxStringType[4].UpperCase()] = LocalTax;
   r[TaxStringType[5].UpperCase()] = ProfitTax;
   return r;
}


void TaxProfile::operator = (const TaxProfile& rhs)
{
   _taxProfileName = rhs.taxProfileName;
   _taxProfileType = rhs.taxProfileType;
   _taxPercentage  = rhs.taxPercentage;
  //-- to be incorpotated in future--//
  // _taxCode        = rhs.taxCode;
   _taxPriority = rhs.taxPriority;
   _taxSelectable = rhs.taxSelectable;
   _taxProfileDBKey = rhs.GetTaxProfileDBKey();
}

bool TaxProfile::operator == (const TaxProfile& rhs) const
{
   return _taxProfileName == rhs.taxProfileName;
/*          _taxProfileType == rhs.taxProfileType &&
          _taxPercentage == rhs.taxPercentage &&
          _taxPriority == rhs.taxPriority &&
          _taxSelectable == rhs.taxSelectable &&
          _taxProfileDBKey == rhs.GetTaxProfileDBKey();     */
}

bool TaxProfile::operator < (const TaxProfile& rhs) const
{
   return false;
}


int TaxProfile::GetTaxProfileDBKey() const
{
   return _taxProfileDBKey;
}

void TaxProfile::SetTaxProfileDBKey(int dbKey)
{
   if( _taxProfileDBKey == 0)
      _taxProfileDBKey = dbKey;
}

TaxProfileKey TaxProfile::GetUniqueProfileKeyPair()
{
   TaxProfileKey key(this->taxProfileName,taxProfileType);
   return key;
}

TaxProfStatus TaxProfile::GetTaxProfileStatus() const
{
   TaxProfStatus status = Unchanged; 

   if(!_taxSelectable)
      status = Deleted;
   else if( _taxProfileDBKey > 0 )
      status = Edited;
   else if(_taxProfileDBKey == 0)
      status = Created;
   else
      status = Unchanged; // todo: wrong

   return status;
}

#pragma package(smart_init)


