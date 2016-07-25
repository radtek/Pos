//---------------------------------------------------------------------------

#ifndef WeightStreamKenkoKK300H
#define WeightStreamKenkoKK300H

#include "SysUtils.hpp"
#include <memory>
#include "Classes.hpp"
#include "Weight.h"
#include "WeightStreamConverter.h"
#include <vector>

class TWeightStreamKenkoKK300 : public IWeightStreamConverter
{
    private:
        TWeightStreamKenkoKK300(unsigned char *Buffer, int BufferSize);
        std::auto_ptr<TMemoryStream> Stream;
        static std::vector<double> RecentWeightsList;

    public:
        static TWeightStreamKenkoKK300* Create(unsigned char *Buffer, int BufferSize);// Uses the default constructor
        void GetWeight(TWeight &Weight);
        void GetStable(bool &Stable);
};
#endif
