//---------------------------------------------------------------------------

#ifndef WeightStreamQUQAH
#define WeightStreamQUQAH

#include "SysUtils.hpp"
#include <memory>
#include "Classes.hpp"
#include "Weight.h"
#include "WeightStreamConverter.h"

class TWeightStreamQUQA : public IWeightStreamConverter
{
   private:
      TWeightStreamQUQA(unsigned char *Buffer, int BufferSize);
      std::auto_ptr<TMemoryStream> Stream;
   public:
      static TWeightStreamQUQA* Create(unsigned char *Buffer, int BufferSize);   // Uses the default constructor
      void GetWeight(TWeight &Weight);
      void GetStable(bool &Stable);
};
#endif
