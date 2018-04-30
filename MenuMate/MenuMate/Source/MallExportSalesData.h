//---------------------------------------------------------------------------

#ifndef MallExportSalesDataH
#define MallExportSalesDataH
#include <System.hpp>
#include <map.h>
#include <list>
//---------------------------------------------------------------------------
class TMallExportSalesData
{
private:
    int _mallExportSaleKey;
    int _mallKey;
    UnicodeString _field;
    int _fieldIndex;
    UnicodeString _dataValue;
    UnicodeString _dataValueType;
    TDateTime _dateCreated;
    UnicodeString _createdBy;
    int _arcBillKey;
    int _zKey;
    int _deviceKey;
    UnicodeString _invoiceNumber;

    void SetMallExportSalesKey(int mallexportSalesId);
    void SetMallKey(int mallKey);
    void SetField(UnicodeString field);
    void SetFieldIndex(int fieldIndex);
    void SetDataValue(UnicodeString dataValue);
    void SetDataValueType(UnicodeString dataValueType);
    void SetDateCreated(TDateTime dateCreated);
    void SetCreatedBy(UnicodeString createdBy);
    void SetArcBillKey(int arcBillKey);
    void SetZKey(int zKey);
    void SetDeviceKey(int deviceKey);
    void SetInvoiceNumber(UnicodeString invoiceNumber);

public:
    __property int MallExportSalesId = {read = _mallExportSaleKey, write = SetMallExportSalesKey};
    __property int MallKey = {read = _mallKey, write = SetMallKey};
    __property UnicodeString Field = {read = _field, write = SetField};
    __property int FieldIndex = {read = _fieldIndex, write = SetFieldIndex};
    __property UnicodeString DataValue = {read = _dataValue, write = SetDataValue};
    __property UnicodeString DataValueType = {read = _dataValueType, write = SetDataValueType};
    __property TDateTime DateCreated = {read = _dateCreated, write = SetDateCreated};
    __property UnicodeString CreatedBy = {read = _createdBy, write = SetCreatedBy};
    __property int ArcBillKey = {read = _arcBillKey, write = SetArcBillKey};
    __property int ZKey = {read = _zKey, write = SetZKey};
    __property int DeviceKey = {read = _deviceKey, write = SetDeviceKey};
    __property UnicodeString InvoiceNumber = {read = _invoiceNumber, write = SetInvoiceNumber};
};

class TMallExportSalesWrapper
{
private:
    std::list<TMallExportSalesData> _mallExportSalesData;
    std::map<int, double> _saleBySalsType;

    void SetMallExportSalesData(std::list<TMallExportSalesData> salesData);
    void SetSalesBySalesType(std::map<int, double> saleBySalsType);

public:
    __property std::list<TMallExportSalesData> SalesData =  {read = _mallExportSalesData, write = SetMallExportSalesData};
    __property std::map<int, double> SaleBySalsType = {read = _saleBySalsType, write = SetSalesBySalesType};
};
#endif
