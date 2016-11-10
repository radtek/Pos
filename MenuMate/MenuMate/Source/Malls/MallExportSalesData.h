//---------------------------------------------------------------------------

#ifndef MallExportSalesDataH
#define MallExportSalesDataH
#include <System.hpp>
//---------------------------------------------------------------------------
class TMallExportSalesData
{
    private:
    int _mallExportSalesId;
    int _mallKey;
    UnicodeString _field;
    UnicodeString _dataValue;
    UnicodeString _dataValueType;
    int _enumType;
    TDateTime _dateCreated;
    UnicodeString _createdBy;
    int _arcBillKey;
    int _zKey;

    void SetMallExportSalesId(int mallexportSalesId);
    void SetMallKey(int mallKey);
    void SetField(UnicodeString field);
    void SetDataValue(UnicodeString dataValue);
    void SetDataValueType(UnicodeString dataValueType);
    void SetEnumType(int enumType);
    void SetDateCreated(TDateTime dateCreated);
    void SetCreatedBy(UnicodeString createdBy);
    void SetArcBillKey(int arcBillKey);
    void SetZKey(int zKey);

    public:
    __property int MallExportSalesId = {read = _mallExportSalesId, write = SetMallExportSalesId};
    __property int MallKey = {read = _mallKey, write = SetMallKey};
    __property UnicodeString Field = {read = _field, write = SetField};
    __property UnicodeString DataValue = {read = _dataValue, write = SetDataValue};
    __property UnicodeString DataValueType = {read = _dataValueType, write = SetDataValueType};
    __property int enumType = {read = _enumType, write = SetEnumType};
    __property TDateTime DateCreated = {read = _dateCreated, write = SetDateCreated};
    __property UnicodeString CreatedBy = {read = _createdBy, write = SetCreatedBy};
    __property int ArcBillKey = {read = _arcBillKey, write = SetArcBillKey};
    __property int ZKey = {read = _zKey, write = SetZKey};

};
#endif
