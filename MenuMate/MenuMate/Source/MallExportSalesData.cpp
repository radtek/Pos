//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSalesData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TMallExportSalesData::SetMallExportSalesKey(int mallExportSaleKey)
{
    _mallExportSaleKey = mallExportSaleKey;
}
//---------------------------------------------------------------------------
void TMallExportSalesData::SetMallKey(int mallKey)
{
    _mallKey = mallKey;
}
//---------------------------------------------------------------------------
void TMallExportSalesData::SetField(UnicodeString field)
{
    _field = field;
}
//---------------------------------------------------------------------------
void TMallExportSalesData::SetFieldIndex(int fieldIndex)
{
    _fieldIndex = fieldIndex;
}
//---------------------------------------------------------------------------
 void TMallExportSalesData::SetDataValue(UnicodeString dataValue)
{
    _dataValue = dataValue;
}
//------------------------------------------------------------------------------------------------------------
 void TMallExportSalesData::SetDataValueType(UnicodeString dataValueType)
{
    _dataValueType = dataValueType;
}
//---------------------------------------------------------------------------
 void TMallExportSalesData::SetDateCreated(TDateTime dateCreated)
{
    _dateCreated = dateCreated;
}
//---------------------------------------------------------------------------
 void TMallExportSalesData::SetCreatedBy(UnicodeString createdBy)
{
    _createdBy = createdBy;
}
//---------------------------------------------------------------------------
 void TMallExportSalesData::SetArcBillKey(int arcBillKey)
{
    _arcBillKey = arcBillKey;
}
//---------------------------------------------------------------------------
 void TMallExportSalesData::SetZKey(int zKey)
{
    _zKey = zKey;
}
//---------------------------------------------------------------------------
void TMallExportSalesData::SetDeviceKey(int deviceKey)
{
    _deviceKey = deviceKey;
}
//---------------------------------------------------------------------------
void TMallExportSalesData::SetSalesBySalesType(std::map<int, Currency> saleBySalsType)
{
    _saleBySalsType = saleBySalsType;
}
//---------------------------------------------------------------------------


