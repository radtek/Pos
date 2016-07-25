//---------------------------------------------------------------------------

#ifndef WeightStreamConverterH
#define WeightStreamConverterH

#include "Classes.hpp"
#include "Weight.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
enum TWSCExceptionType {twscNoData,twscPacketFormatError,twscDataError,twscOverLoaded,twscBCCError};

class TWSCException : public Exception
{
   public:
      TWSCExceptionType Type;
      virtual TWSCException(TWSCExceptionType inType,AnsiString inErrorMsg);
};

class IWeightStreamConverter
{
   private:
   public:
      virtual void GetWeight(TWeight &Weight) = 0;
      virtual void GetStable(bool &Stable) = 0;
};
#endif
