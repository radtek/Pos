//---------------------------------------------------------------------------

#ifndef TaxProfileH
#define TaxProfileH

#include <StdCtrls.hpp>
#include <map>

//---------------------------------------------------------------------------

const int NumTaxTypes = 6;
const enum TaxType { SalesTax = 0, PurchaseTax, ServiceCharge, ServiceChargeTax, LocalTax, ProfitTax, DefaultNULLTax = 99 };
const AnsiString TaxStringType[] = {"Sales Tax", "Purchase Tax", "Service Charge", "Service Charge Tax", "Local Tax", "Profit Tax", ""};
const enum TaxProfStatus{ Unchanged = 0, Created, Deleted, Edited };

typedef Currency Percentage;
typedef int Priority;
typedef std::map<AnsiString, TaxType > TaxMap;
typedef std::pair<AnsiString, TaxType> TaxProfileKey;


class TaxProfile
{
   public:
      TaxProfile();
      TaxProfile(int dbKey);
      TaxProfile(AnsiString name, Percentage perc, TaxType type, Priority priority);

      __property AnsiString   taxProfileName = { read = _taxProfileName,   write = writetaxProfileName   };
      __property Percentage   taxPercentage  = { read = _taxPercentage,    write = writetaxPercentage    };
      __property TaxType      taxProfileType = { read = _taxProfileType,   write = writetaxProfileType   };
      __property Priority     taxPriority    = { read = _taxPriority,      write = writetaxPriority      };
      __property bool         taxSelectable  = { read = _taxSelectable,    write = writetaxSelectable    };

      static TaxType Convert(int type);
      static TaxType Convert(AnsiString type);
      static AnsiString Convert(TaxType type);

      int GetTaxProfileDBKey() const;
      void SetTaxProfileDBKey(int dbKey);
      TaxProfileKey GetUniqueProfileKeyPair();
      TaxProfStatus GetTaxProfileStatus() const;

      void operator = (const TaxProfile& rhs);
      bool operator == (const TaxProfile& rhs) const;
      bool operator < (const TaxProfile& rhs) const;

   private:
      AnsiString  _taxProfileName;
      TaxType     _taxProfileType;
      Percentage  _taxPercentage;
      Priority    _taxPriority;
      bool        _taxSelectable;
      int         _taxProfileDBKey;

      void  writetaxProfileName  ( AnsiString intaxProfileName );
      void  writetaxProfileType  ( TaxType intaxProfileType );
      void  writetaxPercentage   ( Percentage intaxPercentage );
      void  writetaxPriority     ( Priority intaxPriority );
      void  writetaxSelectable   ( bool intaxSelectable );

      static TaxMap stringToEnum;
      static TaxMap ret();
};

typedef std::map<TaxProfileKey, TaxProfile*> TaxProfileMap;
TaxMap TaxProfile::stringToEnum = ret();


#endif



