//---------------------------------------------------------------------------

#ifndef WeightStreamTypeAveryFxH
#define WeightStreamTypeAveryFxH
#include "SysUtils.hpp"
#include <memory>
#include <vector>


#include "WeightStreamConverter.h"

class TWeightStreamTypeAveryFx : public IWeightStreamConverter
{
   private:
      TWeightStreamTypeAveryFx(unsigned char *Buffer, int BufferSize);
      std::auto_ptr<TMemoryStream> Stream;
      static std::vector<double> RecentWeightsList;
   public:
      static TWeightStreamTypeAveryFx* Create(unsigned char *Buffer, int BufferSize);   // Uses the default constructor
      void GetWeight(TWeight &Weight);
      void GetStable(bool &Stable);
};
//---------------------------------------------------------------------------
#endif
