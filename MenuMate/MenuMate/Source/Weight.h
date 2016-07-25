//---------------------------------------------------------------------------

#ifndef WeightH
#define WeightH

#include "system.hpp"

#define WEIGHT_TYPE Currency

//---------------------------------------------------------------------------
enum eWeightUnit { ewuGrams, ewuKg , ewulb };
enum eWeightState { ewuValid = 0, ewuUnknown, ewuInvalid };



class TWeight
{
   private :
      WEIGHT_TYPE Weight;
   public :
      eWeightUnit Unit;
      eWeightState State;

      TWeight();

      double ToDouble();
      WEIGHT_TYPE GetWeight();
      void Assign(TWeight &inWeight);

      UnicodeString ToString();
      UnicodeString ToStringWithUnit();
      UnicodeString GetUnitAsString();

      void GetWeightIn_g(TWeight &Weight);
      void GetWeightIn_Kg(TWeight &Weight);
      void GetWeightIn_lb(TWeight &Weight);

      void SetWeightIn_g(WEIGHT_TYPE Weight);
      void SetWeightIn_Kg(WEIGHT_TYPE Weight);
      void SetWeightIn_lb(WEIGHT_TYPE Weight);
      void SetWeight_Invalid(WEIGHT_TYPE Weight);
      void SetWeight_Unknown(WEIGHT_TYPE inWeight);

      WEIGHT_TYPE AsGrams();
      WEIGHT_TYPE AsKiloGrams();
      WEIGHT_TYPE AsPounds();

      bool IsEqual(TWeight &inWeight);
      bool Valid();

     TWeight& __fastcall operator - (TWeight rhs);
     void __fastcall operator = (TWeight rhs);           
};
#endif
