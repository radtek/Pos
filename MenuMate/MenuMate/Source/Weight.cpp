//---------------------------------------------------------------------------


#pragma hdrstop

#include "Weight.h"
#include "SysUtils.hpp"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TWeight::TWeight()
{
//   SetWeight_Unknown(0);
   SetWeightIn_Kg(0);
}

double TWeight::ToDouble()
{
   return double(Weight);
}

WEIGHT_TYPE TWeight::GetWeight()
{
   return Weight;
}

UnicodeString TWeight::ToString()
{
   UnicodeString StrWeight = "";
   switch(State)
   {
      case ewuValid:
         StrWeight = FormatFloat("0.000",Weight);
      break;
      case ewuUnknown:
         StrWeight = "Unknown";
      break;
      case ewuInvalid:
         StrWeight = "Invalid";
      break;
   }
   return StrWeight;
}

UnicodeString TWeight::ToStringWithUnit()
{
   UnicodeString StrWeight = "";
   switch(State)
   {
      case ewuValid:
         StrWeight = FormatFloat("0.000 " + GetUnitAsString(),Weight);
      break;
      case ewuUnknown:
         StrWeight = "Unknown " + GetUnitAsString();
      break;
      case ewuInvalid:
         StrWeight = "Invalid " + GetUnitAsString();
      break;
   }
   return StrWeight;
}

UnicodeString TWeight::GetUnitAsString()
{
   switch(Unit)
   {
      case ewuGrams:
      return "g";
      case ewuKg:
      return "kg";
      case ewulb:
      return "lb";
      default :
      return "??";
   }
}

void TWeight::Assign(TWeight &inWeight)
{
   switch(Unit)
   {
      case ewuGrams:
         SetWeightIn_g(inWeight.AsGrams());
      break;
      case ewuKg:
         SetWeightIn_Kg(inWeight.AsKiloGrams());
      break;
      case ewulb:
         SetWeightIn_lb(inWeight.AsPounds());
      break;
   }
   State == inWeight.State;
}

void TWeight::GetWeightIn_g(TWeight &inWeight)
{
   switch(Unit)
   {
      case ewuGrams:
         inWeight.SetWeightIn_g(Weight);
      break;
      case ewuKg:
         inWeight.SetWeightIn_g(Weight*1000);
      break;
      case ewulb:
         inWeight.SetWeightIn_g(Weight*453.59237);
      break;
   }
}

void TWeight::GetWeightIn_Kg(TWeight &inWeight)
{
   switch(Unit)
   {
      case ewuGrams:
         inWeight.SetWeightIn_Kg(Weight * 0.001);
      break;
      case ewuKg:
         inWeight.SetWeightIn_Kg(Weight);
      break;
      case ewulb:
         inWeight.SetWeightIn_Kg(Weight*0.45359237);
      break;
   }
}

void TWeight::GetWeightIn_lb(TWeight &inWeight)
{
   switch(Unit)
   {
      case ewuGrams:
         inWeight.SetWeightIn_lb(Weight * 0.002204623);
      break;
      case ewuKg:
         inWeight.SetWeightIn_lb(Weight *2.204622622);
      break;
      case ewulb:
         inWeight.SetWeightIn_lb(Weight);
      break;
   }
}

void TWeight::SetWeightIn_g(WEIGHT_TYPE inWeight)
{
   Weight = inWeight;
   Unit = ewuGrams;
   State = ewuValid;
}

void TWeight::SetWeightIn_Kg(WEIGHT_TYPE inWeight)
{
   Weight = inWeight;
   Unit = ewuKg;
   State = ewuValid;
}

void TWeight::SetWeightIn_lb(WEIGHT_TYPE inWeight)
{
   Weight = inWeight;
   Unit = ewulb;
   State = ewuValid;
}


void TWeight::SetWeight_Invalid(WEIGHT_TYPE inWeight)
{
   Weight = inWeight;
   State  = ewuInvalid;
}

void TWeight::SetWeight_Unknown(WEIGHT_TYPE inWeight)
{
   Weight = inWeight;
   State  = ewuUnknown;
}


WEIGHT_TYPE TWeight::AsGrams()
{
   TWeight outWeight;
   GetWeightIn_g(outWeight);
   return outWeight.GetWeight();
}

WEIGHT_TYPE TWeight::AsKiloGrams()
{
   TWeight outWeight;
   GetWeightIn_Kg(outWeight);
   return outWeight.GetWeight();
}

WEIGHT_TYPE TWeight::AsPounds()
{
   TWeight outWeight;
   GetWeightIn_lb(outWeight);
   return outWeight.GetWeight();
}

bool TWeight::IsEqual(TWeight &inWeight)
{
   return (AsKiloGrams() == inWeight.AsKiloGrams());
}

bool TWeight::Valid()
{
   return State == ewuValid;
}

TWeight& __fastcall TWeight::operator - (TWeight rhs)
{
   TWeight NewWeight;
   NewWeight.SetWeightIn_Kg(AsKiloGrams() - rhs.AsKiloGrams());
   Assign(NewWeight);
   return *this;
}

void __fastcall TWeight::operator = (TWeight rhs)
{
   Assign(rhs);
}

