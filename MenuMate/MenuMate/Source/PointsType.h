//---------------------------------------------------------------------------

#ifndef PointsTypeH
#define PointsTypeH
//---------------------------------------------------------------------------
#include "enumPoints.h"
#include <utility>
#include <System.hpp>


class TPointsType
{
    public :
    TPointsAssignedSource Source;
    TPointsTypePair AccType;
    TPointsExportStatus ExportStatus;
    /* The id is used only for tracking a particalur points adjustment.
     For example adjustments made by the discounts system
    UnicodeString id;
*/
    TPointsType(){};
    TPointsType(TPointsAssignedSource Souce, TPointsTypePair inAccType, TPointsExportStatus inExportStatus);

    UnicodeString Name() const ;
	static UnicodeString GetPointsTypeName(TPointsType PtsType);

    friend bool operator==(const TPointsType &f1, const TPointsType &f2);
    friend bool operator < (const TPointsType &f1,const TPointsType &f2);
};
#endif
