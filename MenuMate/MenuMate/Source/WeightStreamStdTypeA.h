//---------------------------------------------------------------------------

#ifndef WeightStreamStdTypeAH
#define WeightStreamStdTypeAH
#include "SysUtils.hpp"
#include <memory>
#include <vector>


#include "WeightStreamConverter.h"

class TWeightStreamStdTypeA : public IWeightStreamConverter
{
   private:
      TWeightStreamStdTypeA(unsigned char *Buffer, int BufferSize);
      std::auto_ptr<TMemoryStream> Stream;
      static std::vector<double> RecentWeightsList;
      UnicodeString GetFormattedString(UnicodeString unFormattedString);
   public:
      static TWeightStreamStdTypeA* Create(unsigned char *Buffer, int BufferSize);   // Uses the default constructor
      void GetWeight(TWeight &Weight);
      void GetStable(bool &Stable);
};
//---------------------------------------------------------------------------
#endif
