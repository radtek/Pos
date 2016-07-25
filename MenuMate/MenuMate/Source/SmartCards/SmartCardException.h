//---------------------------------------------------------------------------
#include  <SysUtils.hpp>

#ifndef SmartCardExceptionH
#define SmartCardExceptionH
//---------------------------------------------------------------------------
enum TSCExceptionType {tsceConnectionFailed,tsceNoReader,tsceNoCard,tsceIncorrectCard,
                       tsceToMuchCardData,tsceInvalidCRC,tsceInvalidSyndCRC,
                       tsceWriteFailed,tsceReadFailed,tsceValidateFailed,tsceUnknownVersion};

class TSCException  : public Exception
{
   public:
      TSCExceptionType Type;
      virtual TSCException(TSCExceptionType inType,UnicodeString inErrorMsg);
};
#endif
