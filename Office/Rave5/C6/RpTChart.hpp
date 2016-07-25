// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpTChart.pas' rev: 6.00

#ifndef RpTChartHPP
#define RpTChartHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpCon.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <Chart.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rptchart
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall WriteChartData(Rpcon::TRvCustomConnection* Connection, Chart::TCustomChart* Chart);
extern PACKAGE void __fastcall PrintTeeChart(Rpbase::TBaseReport* Report, double X1, double Y1, double X2, double Y2, Chart::TCustomChart* Chart);
extern PACKAGE double __fastcall CalcTeeChartWidth(double Height, Chart::TCustomChart* Chart);
extern PACKAGE double __fastcall CalcTeeChartHeight(double Width, Chart::TCustomChart* Chart);

}	/* namespace Rptchart */
using namespace Rptchart;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpTChart
