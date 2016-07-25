#pragma hdrstop

#include "TCPStreamPrepare.h"
//#include <codecvt>
#include "vector.h"
#include "MMMessageBox.h"
#include <system.hpp>
#include <memory>
#include <MMMessageBox.h>
#include "DrinkCommandData.h"
#include "SelectDish.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TTCPStreamPrepare::TTCPStreamPrepare( AnsiString inTcpIPAddress, __int32 inTcpIPPort )
{
	SetIPAddr( inTcpIPAddress );
	SetIPPort( inTcpIPPort    );
}
//............................................................................

TTCPStreamPrepare::~TTCPStreamPrepare()
{
}
//............................................................................

void __fastcall TTCPStreamPrepare::sendThreadTerminated( TObject *Sender )
{
}
//............................................................................


#pragma mark ::::::::::::::::::::::::::::::
#pragma mark PRIVATE SECTION
#pragma mark ::::::::::::::::::::::::::::::

TIdTCPClient* TTCPStreamPrepare::createTCPClient()
{
	TIdTCPClient* result = new TIdTCPClient( NULL );

	//:::::::::::::::::::::::::::::::::::::::::::::::

	result->Host           = FIPAddr;
	result->Port           = FIPPort;
	result->BoundPort      = 0;
	result->ReadTimeout    = _READ_TIME_OUT;
	result->ConnectTimeout = _CONNECT_TIME_OUT;
	//:::::::::::::::::::::::::::::::::::::::::::::::

	return result;
}
//............................................................................

#pragma mark :::::::::::::::::::::::::::::::::::::::::::::
#pragma mark Setters & Getters
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::

void TTCPStreamPrepare::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TTCPStreamPrepare::SetIPPort( __int32 inIPPort )
{

	FIPPort = inIPPort;
}
//............................................................................


TTCPClient::TTCPClient(TIdTCPClient*  inTcpClient,UnicodeString request , int _requestType                                                            ,UnicodeString session_uuid , AnsiString _memberNo , UnicodeString _lineitemid)
{

	//FreeOnTerminate = true;
	_tcpClient   = inTcpClient;
    RequestType = _requestType;
	Setrequestdc( request  );
    session_id = session_uuid;
    memberNo = _memberNo;
    line_item = _lineitemid ;
}
TTCPClient::TTCPClient()
{
}
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::
#pragma mark Setters & Getters
#pragma mark :::::::::::::::::::::::::::::::::::::::::::::

TTCPClient::~TTCPClient()
{
}
void TTCPClient::Setrequestdc( AnsiString request )
{

	requestDc = request;
}

void TTCPStreamPrepare::Connect(UnicodeString str1 , int _requestType,UnicodeString session_uuid , AnsiString memberNo , UnicodeString lineitemid)
{
    try
    {
    TTCPClient *sendThread = new TTCPClient(createTCPClient(),str1 , _requestType,session_uuid,memberNo,lineitemid);
	sendThread->StartProcess();
    }
    catch( Exception& E)
	{
       MessageBox("Exception thrown in connect.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}

void TTCPClient::StartProcess()
{
    if( _tcpClient != NULL )
   {
         makeConnection();
      try
      {
         delete _tcpClient;
      }
      catch( ...)
      {
      }
   }
}

void TTCPClient::makeConnection()                                  // for setting up session{
	try
	{

	   _tcpClient->Connect();
	   if( _tcpClient->Connected() )
	   {
          requestDC(_tcpClient , requestDc);
          requestDCResponse(_tcpClient);
	   }
	}
	catch( Exception& E)
	{
        if(RequestType == eSessionBill)
        {
           Time_Stamp = "";
           TDrinkCommandData::Instance().UpdateTimeStamp(session_id , Time_Stamp);
        }
       	MessageBox(E.Message, "Abort", MB_OK + MB_ICONERROR);
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

}void TTCPClient::sendStream(TIdTCPClient* inTcpClient , UnicodeString str)
{
     UTF8String dest = str;
     std::string data  = dest.c_str();
     __int32 dataSize         = data.size();
 	 __int32 remainedDataSize = dataSize;
     __int32 maxBufferSize =    data.size(); // It can be assigned a lower value
	 __int32 itPos = 0;

     char   *buffer = new char[maxBufferSize];
	// Send Data
	while( remainedDataSize > 0 )
	{
	   __int32 bufferSize = ( remainedDataSize >= maxBufferSize ) ? maxBufferSize : remainedDataSize;
	   data.copy( buffer, bufferSize, itPos );
	   inTcpClient->IOHandler->Write( createByteArray( buffer, bufferSize ), bufferSize );
	   itPos += bufferSize;
	   remainedDataSize -= bufferSize;
	}

	delete[] buffer;
}

void TTCPClient::requestDC(TIdTCPClient* inTcpClient , UnicodeString str)
{
      makeLogFile(str);

      sendStream( inTcpClient , str);
}
void TTCPClient::requestDCResponse(TIdTCPClient* inTcpClient)
{
    int i = 0;
    AnsiString data = "";
	if( inTcpClient->IOHandler->Readable( _READ_TIME_OUT ) )
	{
       if(RequestType == eProductList)
       {
           inTcpClient->IOHandler->CheckForDataOnSource(3000);
           inTcpClient->IOHandler->ReadTimeout = 6000;
           TByteDynArray buffer;
           Sleep(1000);
           inTcpClient->IOHandler->ReadBytes( buffer, -1, true );
           data = AnsiString((char*) & buffer[0], buffer.Length);
           makeLogFile(data);
       }
       else
       {
           inTcpClient->IOHandler->CheckForDataOnSource(1500);
           inTcpClient->IOHandler->ReadTimeout = 3000;
           TByteDynArray buffer;
           inTcpClient->IOHandler->ReadBytes( buffer, -1, true );
           data = AnsiString((char*) & buffer[0], buffer.Length);
           makeLogFile(data);
       }
     }
       if(RequestType == 6)
       {
         writeToProductFile(data)  ;
       }

       AnsiString message = "";
       message = ReadResponse(data , RequestType);
       if (message.Length() == 0 && RequestType == eStartSession)
       {
           AnsiString id = session_id;
           int session_uuid = atoi(id.c_str());
           TDrinkCommandData::Instance().InsertDataToTable(memberNo , session_uuid);
       }
       else if (message.Length() == 0 && RequestType == eSendCash)
       {
           AnsiString id = line_item;
           int line_itemId = atoi(id.c_str());
           TDrinkCommandData::Instance().UpdateLineItemID(session_id , line_itemId);
       }
       else if (message.Length() == 0 && RequestType == eSessionSummary)
       {
            //        Check if item has been fetched earlier or not.
           Stamp = TDrinkCommandData::Instance().FetchTimeStamp(session_id);
            if(Stamp != Time_Stamp )
            {
                TDrinkCommandData::Instance().UpdateTimeStamp(session_id, Time_Stamp);
                UnicodeString str2 = "sessions.bill\n";
                str2 += ",session_uuid,";
                str2 += session_id;
                str2 += "\n" ;
                str2 += ".\n";
                int requestType = eSessionBill;
                UnicodeString session_uuid = session_id;
                AnsiString memberNo = "";
                UnicodeString lineitemid = "";
                AnsiString path = TGlobalSettings::Instance().DrinkCommandServerPath;
                int port = TGlobalSettings::Instance().DrinkCommandServerPort;
                std::auto_ptr<TTCPStreamPrepare> streamPrepare(
                                                            new TTCPStreamPrepare(
                                                                path,
                                                                port ) );
                streamPrepare->Connect( str2 , requestType, session_uuid , memberNo , lineitemid);
            }
       }
       else if (message.Length() == 0 && RequestType == eCloseSession)
       {
          AnsiString session_uuid = "";
          if(session_id.Pos(",") != 0)
          {
             AnsiString id = session_id;
             char* session_id_temp = new char[id.Length()+1];
             strcpy(session_id_temp,id.c_str());
             int prev = 0;
             for(int i = 0 ; i <= session_id.Length(); i++ )
             {
                if(session_id_temp[i] == '\n' || session_id_temp[i] == '\0')
                {
                    if(prev != 0)
                    {
                        session_uuid += id.SubString(prev , i - prev + 1);
                        prev = i + 2;
                    }
                    else
                    {
                        session_uuid += id.SubString(prev , i - prev);
                        prev = i + 2;
                    }
                }
             }

          }
          else
          {
              session_uuid = session_id;
          }
          TDrinkCommandData::Instance().UpdateClose(session_uuid);
       }
       else if (message.Length() == 0 && RequestType == eSessionBill)
       {

       }
       else if (message.Length() == 0 && RequestType == eProductList)
       {

       }
       else
       {
          throw new Exception(message);
       }
}
//...........................................................................



TBytes TTCPClient::createByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;

	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];
	}
	return result;
}
AnsiString TTCPClient::ReadResponse(AnsiString data , int type)
{
    char* data_temp = new char[data.Length() + 1];
    strcpy(data_temp , data.c_str());
    UnicodeString data_c = data;
    AnsiString message = "";
    AnsiString data_result = "";
    AnsiString data_result_expected = "";
    AnsiString thirdPartyCode = "";
    int rownum = 0;
    AnsiString qtyValue = "";
    AnsiString billValue ="";
    switch (type)
    {
        case eStartSession:
            {
           // processing for successful creation
             AnsiString data1 =  UpperCase(data);
             int i = data1.Pos("SESSION CREATED");

             if (i != 0)
             {
               message = "";
             }
               else
               {
                 message = "Session could not be created";
               }
             }

           break;

        case eSendCash:
            {
           // processing for sending cash info to DC

             AnsiString data1 = UpperCase(data);
             int i = data1.Pos("CASH ADDED");
             if (i != 0)
             {
               message = "";
             }
             else
             {
               message = "Cash could not be added.";
             }
            }
             break;

        case eSessionSummary:
            // processing for generating bill
             {
                int findProductRow =  data.Pos(",PRODUCT");
                int findCreatedAtLoc = 0;

                int countOfCommas = 0;
                int countOfCommasAux = 0;
                if(findProductRow != 0)                                          // Product had been poured so we will go for sessions.bill request
                {
                    //Find Location for created_at
                    findCreatedAtLoc =  data.Pos(",created_at");
                    int i = findCreatedAtLoc - 1;
                    // Count the number of commas before "created_at" in the row
                    while(data_temp[i] != '\n')
                    {
                        if (data_temp[i] == ',')
                        {
                           countOfCommas++;
                        }
                        i = i-1;
                    }
                    // Go to next Line
                    i = findCreatedAtLoc;
                    while(data_temp[i] != '\n')
                    {
                        i++;
                    }
                    //Switch to new line
                     i = i+1;
                    //FETCH time_stamp by going to next line
                    while(countOfCommasAux != countOfCommas)
                    {
                        if(data_temp[i] == ',')
                        {
                            countOfCommasAux++;
                        }
                        i++;
                    }
                    Time_Stamp = data.SubString(i+1, 19);

                }
                else
                {
                   Time_Stamp = "";
                }
             }
           break;

           case eCloseSession:
            {
             AnsiString data1 = UpperCase(data);
             int i = data1.Pos("SESSION CLOSED");
             if (i != 0)
             {
               message = "";
             }
             else
             {
               message = "Session could not be closed";
             }
            }
             break;
        case eSessionBill:
            // processing for generating bill
             {
                AnsiString data1 = UpperCase(data);
                int length = data.Length();
                int checkForZeroPour =  data1.Pos(",PRODUCT,0");
                if((checkForZeroPour != 0)||(length == 0))                          // Case Of Zero Pour , TimeStamp needs to be set to Null
                {
                   //std::auto_ptr<TDrinkCommandData> dcData ( new TDrinkCommandData());
                   Time_Stamp = "";
                   //dcData->UpdateTimeStamp(session_id , Time_Stamp);              // Put Stamp back to zero
                   TDrinkCommandData::Instance().UpdateTimeStamp(session_id , Time_Stamp);
                }
                else
                {
                   int locVolume_units =  data1.Pos(",VOLUME_UNITS");
				   int locMoney_units = data1.Pos(",MONEY_UNITS");
                   int locMoney_units1 = data1.SubString(locMoney_units + 1,length - locMoney_units).Pos(",MONEY_UNITS");
                   locMoney_units = locMoney_units + locMoney_units1;
				   int locProduct_code = data1.Pos(",PRODUCT_CODE");
				   int i = 0;
				   int countOfCommas = 0;
				   int CommasBefVol = 0;
				   int CommasBefMon = 0;
				   int CommasBefProd = 0;
				   int endOfLine = 0;
                   int startOfLine = 0;
				   int noOfRowsOfConsumption = 0;
                   AnsiString moneyUnits = "";
                   AnsiString volumeUnits = "";
                   AnsiString productCode = "";
                   

				   // finding out Commas before every requirement
                   for(int j = 0 ; j < 3 ; j++)
				   {
					   if(j == 0)
					   {
					        i = locVolume_units - 1;
						   while(data_temp[i] != '\n')
						   {
								if (data_temp[i] == ',')
								{
								   countOfCommas++;
								}
								i = i-1;
						   }
                           CommasBefVol = countOfCommas;
                           countOfCommas = 0;
					   }
					   else if (j == 1)
					   {
					        i = locMoney_units -1;
						   while(data_temp[i] != '\n')
						   {
								if (data_temp[i] == ',')
								{
								   countOfCommas++;
								}
								i = i-1;
						   }
                           CommasBefMon = countOfCommas;
                           countOfCommas = 0;
					   }
					   else
					   {
					        i = locProduct_code -1;
						   while(data_temp[i] != '\n')
						   {
								if (data_temp[i] == ',')
								{
								   countOfCommas++;
								}
								i = i-1;
						   }
                           CommasBefProd = countOfCommas;
                           countOfCommas = 0;
					   }
				   }
				   // Reach the end of line
				   endOfLine = locProduct_code;
				   while(data_temp[endOfLine] != '\n')
				   {
					    endOfLine++;
				   }
				   startOfLine = endOfLine + 1;                       // beginning of new line

				   //Find number of rows representing consumption
				   int k = locProduct_code;
				   while (data_temp[k] != '.')
				   {
					    if(data_temp[k] == '\n')
						{
							noOfRowsOfConsumption++;
						}
                        k++;
				   }

				   // Retrieve the requirements
				   for(int k = 1 ; k <= noOfRowsOfConsumption ; k++)
				   {
						//find product code
						int commaProd = 0;
						int traverseAux = 0;
						int traverse = startOfLine;
						while(commaProd <= CommasBefProd)
						{
							if(data_temp[traverse] == ',' || data_temp[traverse] == '\r')
							{
								commaProd++;
								if(commaProd == CommasBefProd)
								{
									traverseAux = traverse;
								}
							}
							traverse++;
						}
                        productCode = data.SubString(traverseAux+2,traverse-traverseAux-2);

						//find volume units
						int commaVol = 0;
						int traverseAux1 = 0;
						int traverse1 = startOfLine;
						while(commaVol <= CommasBefVol)
						{
							if(data_temp[traverse1] == ',' || data_temp[traverse1] == '\r')
							{
								commaVol++;
								if(commaVol == CommasBefVol)
								{
									traverseAux1 = traverse1;
								}
							}
							traverse1++;
						}
                        volumeUnits = data.SubString(traverseAux1+2,traverse1-traverseAux1-2);


						//find money units
						int commaMon = 0;
						int traverseAux2 = 0;
						int traverse2 = startOfLine;
						while(commaMon <= CommasBefMon)
						{
							if(data_temp[traverse2] == ',' || data_temp[traverse2] == '\r')
							{
								commaMon++;
								if(commaMon == CommasBefMon)
								{
									traverseAux2 = traverse2;
								}
							}
							traverse2++;
						}
                        moneyUnits = data.SubString(traverseAux2+3,traverse2-traverseAux2-3);
                        bool lastProduct = false;
                        if(productCode.Length() > 0)
                        {
                            if(k == noOfRowsOfConsumption)
                            {
                               lastProduct = true;
                            }
                            TDrinkCommandData::Instance().getItemFromThirdPartyCode(productCode , volumeUnits , moneyUnits,lastProduct);
                        }
                        //go to next line if required
                        if(k < noOfRowsOfConsumption)
                        {
                            int end = startOfLine;
                            while(data_temp[end] != '\n')
                            {
                                end++;
                            }
                            startOfLine = end + 1;
                        }
                        if((data_temp[startOfLine] == '.') && (data_temp[startOfLine+1] == '\r'))
                        {
                           break;
                        }
				   }
//                   AnsiString str = "Complete";
//                   OrderComplete.Occured(str);
                }
             }
           break;
         case eProductList:
           {
              message = "";
           }

         default:
             break;
    }
      return message;
}


void TTCPClient::makeLogFile(AnsiString str)
{
     AnsiString fileName = ExtractFilePath(Application->ExeName) + "DCLogs.txt" ;



            TStrings * List = new TStringList();
            if (FileExists(fileName) )
            {
              List->LoadFromFile(fileName);
            }


            List->Add("Request/Response:- "+ str +  "\n");


            List->SaveToFile(fileName );
}

void TTCPClient::writeToProductFile(AnsiString str)
{
     AnsiString fileName = ExtractFilePath(Application->ExeName) + "ProductsList.txt" ;
     std::auto_ptr<TMemoryStream>Stream(new TMemoryStream);
     Stream->Write(str.c_str(), str.Length());
     Stream->SaveToFile(fileName);
}
