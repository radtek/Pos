#include <vcl.h>
#include "PlanController.h"
#include "GlobalSettings.h"
#include "MMLogging.h"
#include <jpeg.hpp>
#include "DBTables.h"

using namespace TablePlan;

// ---------------------------------------------------------------------------
PlanController::PlanController()
{
	ScaleFactor = 1;
}

// ---------------------------------------------------------------------------
bool PlanController::Init()
{

	try
	{
		UnicodeString ReservationsURL = TGlobalSettings::Instance().ReservationsAddress;
		_client = GetIPosIntegration(false, ReservationsURL, NULL);

		ArrayOfDTOLocation _locations = _client->GetAllLocations();
		for (int i = 0; i < _locations.Length; i++) {
			DTOLocation *Loc = _locations[i];
			locations[Loc->Id] = Loc;
		}

		/*
		 * We're checking whether there're elements in the _locations array after
		 * the loop above as it's not the common case for there to be zero
	    * locations. The loop is safe as it won't iterate with zero locations.
		 */
		if (!_locations.Length)
			return false;

		_connected = true;

		std::map<int, DTOLocation*>::iterator itLocations = locations.begin();
		if (itLocations != locations.end())
		{
			_idxCurrentPlan = itLocations->first;
		}
		else
		{
			_idxCurrentPlan = -1;
		}

	}
	catch(...)
	{
		_connected = false;
	}

	return _connected;
}

// ---------------------------------------------------------------------------
void PlanController::SetView(TImage *v)
{
	image = v;
}

// ---------------------------------------------------------------------------
PlanController::~PlanController()
{
	// unitialise com subsystem
	//CoUninitialize();
}

// -----------------------------------------------------------------------------------------------
bool PlanController::IsInitOk()
{
	return _connected;
}

// -----------------------------------------------------------------------------------------------
void PlanController::DrawCurrentPlan(Database::TDBTransaction &DBTransaction, bool isNormalArea)
{
	// save canvas brush
	TBrushRecall *savedBrush = new TBrushRecall(image->Canvas->Brush);
	savedBrush->Store();
	ClearView();
	try
	{
		try
		{
			if (locations.size() > 0)
			{
				double XScaleFactor = 1;
				double YScaleFactor = 1;
				TByteDynArray Data = _client->GetBackgroundImgDataForLocation(_idxCurrentPlan);
				if (Data.Length > 0)
				{
					try
					{
						std::auto_ptr<TMemoryStream>BitMapStream(new TMemoryStream());
						BitMapStream->WriteBuffer(&Data[0], Data.Length);
						BitMapStream->Position = 0;
						std::auto_ptr<TJPEGImage>BckGnd(new TJPEGImage());
						BckGnd->LoadFromStream(BitMapStream.get());

						BackGndRect = TRect(0, 0, BckGnd->Width, BckGnd->Height); // left , top, right , bottom
						TRect ScreenRect = TRect(0, 0, image->Width, image->Height);

						if (ScreenRect.Width() > 0 && BackGndRect.Width() > 0)
							XScaleFactor = double(ScreenRect.Width()) / double(BackGndRect.Width());

						if (ScreenRect.Height() > 0 && BackGndRect.Height() > 0)
							YScaleFactor = double(ScreenRect.Height()) / double(BackGndRect.Height());

                        if(XScaleFactor < YScaleFactor)
                        {
                            ScaleFactor = XScaleFactor;
                        }
                        else
                        {
                            ScaleFactor = YScaleFactor;
                        }

						TRect ScaledRect = TRect(0, 0, BckGnd->Width * ScaleFactor, BckGnd->Height * ScaleFactor);
						image->Canvas->StretchDraw(ScaledRect, BckGnd.get());
					}
					catch(Exception & Err)
					{
						TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Table Plan Background Error " + Err.Message);
					}
				}

		        ArrayOfDTOReservable tables = _client->GetTablesForLocation(_idxCurrentPlan);
                std::map<int,UnicodeString> PartyNames;
                TDBTables::GetPartyNames(DBTransaction, PartyNames);
                std::map<int,UnicodeString> TableStatus;
                TDBTables::GetTableStatus(DBTransaction, TableStatus);

                // render each table
                for (int i = 0; i < tables.Length; i++)
                {
                    int OrigX = tables[i]->X;
                    int OrigY = tables[i]->Y;
                    int OrigHeight = tables[i]->Height;
                    int OrigWidth  = tables[i]->Width;
                    OrigX = ((double)OrigX) * ScaleFactor;
                    OrigWidth = ((double)OrigWidth) * ScaleFactor;
                    OrigY = ((double)OrigY) * ScaleFactor;
                    OrigHeight = ((double)OrigHeight) * ScaleFactor;
                    TRect rect(OrigX, OrigY, OrigX + OrigWidth , OrigY + OrigHeight );

                    if(!TGlobalSettings::Instance().FloorPlanTransparentTables )
                    {
                        image->Canvas->Brush->Style = bsClear;
                        if(TGlobalSettings::Instance().UpdateTableGUIOnOrderStatus && isNormalArea)
                        {
                          int status = TDBTables::GetTableBillingStatus(DBTransaction,tables[i]->Number);
                           switch(status)
                            {
                               // prelim
                               case ePrelim:
                               image->Canvas->Brush->Color = clGreen;
                               image->Canvas->Font->Color =  clWhite;
                               break;
                               //call away
                               case eCallAwayStatus:
                               image->Canvas->Brush->Color = clYellow;
                               image->Canvas->Font->Color =  clBlack;
                               break;
                               case ePartialSplit:
                               //split and partial
                               image->Canvas->Brush->Color = (TColor)RGB(255,153,0);
                               image->Canvas->Font->Color = clBlack;
                               break;
                               default :
                                    if(TDBTables::IsEmpty(DBTransaction,tables[i]->Number))
                                     {
                                        image->Canvas->Brush->Color = clSilver;
                                        image->Canvas->Font->Color =  clBlack;
                                     }
                                     else
                                     {
                                        SetColorAsPerNoServiceTime(DBTransaction,tables[i]->Number);
                                     }
                                break;
                            }
                        }
                        else
                        {
                            if(TDBTables::IsEmpty(DBTransaction,tables[i]->Number) || (!isNormalArea))
                            {
                                image->Canvas->Brush->Color = clSilver;
                                image->Canvas->Font->Color =  clBlack;
                            }
                            else
                            {
                               SetColorAsPerNoServiceTime(DBTransaction,tables[i]->Number);
                            }
                        }
                        if (tables[i]->Shape == "r")
                        {
                            image->Canvas->FillRect(rect);
                        }
                        else
                        {
                            image->Canvas->Ellipse(rect);
                        }
                    }
					// table label
                    std::auto_ptr <TStringList> TableText(new TStringList);
                    TableText->Add(tables[i]->Name);
        		    int LineHeight = image->Canvas->TextHeight(tables[i]->Name.t_str());
                    UnicodeString PartyName = PartyNames[tables[i]->Number];

                    if(TDBTables::IsTableMarked(DBTransaction, tables[i]->Number))
                        PartyName = PartyName + " (S)";

                    if(PartyName != "")
                    {
	                    TableText->Add(PartyName);
                    }

                    UnicodeString Status = TableStatus[tables[i]->Number];
                    if(Status != "")
                    {
	                    TableText->Add(Status);
                    }

                    int TotalTxtHeight = LineHeight * TableText->Count;

                    int Top = rect.Top + (rect.Height() / 2) - (TotalTxtHeight / 2);
                    for(int i = 0; i < TableText->Count; i++)
                    {
            			int txtWidth = image->Canvas->TextWidth(TableText->Strings[i]);
	                    int Left = rect.Left + (rect.Width() / 2) - (txtWidth / 2);
                    	image->Canvas->TextOutA(Left,Top , TableText->Strings[i]);
                        Top += LineHeight;
                    }
		            image->Canvas->Font->Color = clBlack;
                    // Write out Table Status.
    	       }

          }
    }
		catch(Exception & Err)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Table Plan Error " + Err.Message);
			_connected = false;
		}
	}
	__finally
	{
		// this will restore the canvas brush
		delete savedBrush;
	}
}

void PlanController::SetColorAsPerNoServiceTime(Database::TDBTransaction &DBTransaction,int TableNumber)
{
    TDateTime OrderTime =  TDBTables::GetMaxTableBillingTimeStamp(DBTransaction,TableNumber);
    TDateTime PeriodTillBadAsItGets = double(1.0 / 24.0 / 60.0) * double(TGlobalSettings::Instance().NonServicePeriodMinutes); // An Hour.
    if(double(PeriodTillBadAsItGets) == 0)
    {
        PeriodTillBadAsItGets = 1;
    }
    TDateTime Difference 	= Now() - OrderTime;
    float PercentageColour 	= double(Difference) / double(PeriodTillBadAsItGets);
    if(PercentageColour > 1) PercentageColour = 1;
    image->Canvas->Brush->Color = (TColor)RGB((192 * PercentageColour),0,256 - (256 * PercentageColour));
    image->Canvas->Font->Color =  clWhite;
}


DTOReservable *PlanController::GetTable(int LocationID, int TableNo)
{
	try
	{
		ArrayOfDTOReservable tables = _client->GetTablesForLocation(LocationID);
		// render each table
		for (int i = 0; i < tables.Length; i++)
		{
			if (tables[i]->Number == TableNo)
			{
				return tables[i];
			}
		}
	}
	catch(...)
	{
		//
		_connected = false;

	}
}

// -----------------------------------------------------------------------------------------------
void PlanController::FirstPlan()
{
	_idxCurrentPlan = 0;
	ClearView();
}

void PlanController::SetLocation(int LocationID)
{
	std::map<int, DTOLocation*>::iterator itLocations = locations.find(LocationID);
	if (itLocations != locations.end())
	{
		_idxCurrentPlan = itLocations->first;
	}
	ClearView();
}

std::vector<DTOLocation*>PlanController::getLocations()
{
	std::vector<DTOLocation*>LocArray;
	if (_connected)
	{
		std::map<int, DTOLocation*>::iterator itLocations = locations.begin();
		for (itLocations = locations.begin(); itLocations != locations.end(); itLocations++)
		{
			LocArray.push_back(itLocations->second);
		}
	}
	return LocArray;
}

int PlanController::PlanCount()
{
	return locations.size();
}

// -----------------------------------------------------------------------------------------------
void PlanController::UpdateMousePos(int x, int y)
{
	_currentMouseLocation.x = x;
	_currentMouseLocation.y = y;
}

// -----------------------------------------------------------------------------------------------
TPoint PlanController::CurrentMouseLocation()
{
	return _currentMouseLocation;
}

// -----------------------------------------------------------------------------------------------
// Get table description at current mouse position.
//
// -----------------------------------------------------------------------------------------------
DTOReservable *PlanController::GetCurrentTable()
{
	DTOReservable *CurrentTable = NULL;
	if (_connected)
	{
		try
		{
			if (locations.size() > 0)
			{
				ArrayOfDTOReservable tables = _client->GetTablesForLocation(_idxCurrentPlan);
				for (int i = 0; i < tables.Length; i++)
				{
					if (_currentMouseLocation.x >= (tables[i]->X * ScaleFactor) &&
                    	_currentMouseLocation.y >= (tables[i]->Y * ScaleFactor) &&
                        _currentMouseLocation.x < ((tables[i]->X * ScaleFactor) + (tables[i]->Width * ScaleFactor)) &&
                        _currentMouseLocation.y < ((tables[i]->Y * ScaleFactor) + (tables[i]->Height * ScaleFactor)) )
					{
						CurrentTable = tables[i];
					}
				}
			}
		}
		catch(...)
		{
		}
	}
	return CurrentTable;
}

AnsiString PlanController::GetTableDesc()
{

	AnsiString desc;
	if (_connected)
	{
		try
		{

			if (locations.size() > 0)
			{
				ArrayOfDTOReservable tables = _client->GetTablesForLocation(_idxCurrentPlan);
				for (int i = 0; i < tables.Length; i++)
				{
					if (_currentMouseLocation.x >= tables[i]->X && _currentMouseLocation.y >= tables[i]->Y && _currentMouseLocation.x < (tables[i]->X + tables[i]->Width) && _currentMouseLocation.y <
						(tables[i]->Y + tables[i]->Height))
					{
						desc = tables[i]->Name;
						break;
					}
				}
			}
		}
		catch(...)
		{
		}
	}
	return desc;
}

// -----------------------------------------------------------------------------------------------
AnsiString PlanController::GetCurrentPlanName()
{
	AnsiString name;
	if (_connected)
	{
		name = locations[_idxCurrentPlan]->Name;
	}
	return name;
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
void PlanController::ClearView()
{
	TRect cr(0, 0, image->Width, image->Height);
	image->Canvas->Brush->Style = bsSolid;
	image->Canvas->Brush->Color = clWhite;
	image->Canvas->FillRect(cr);
}

int PlanController::CurrentID()
{
	return _idxCurrentPlan;
}
