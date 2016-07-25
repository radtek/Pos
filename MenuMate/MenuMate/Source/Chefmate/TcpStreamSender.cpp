//---------------------------------------------------------------------------

#pragma hdrstop

#include "TcpStreamSender.h"

#include "TcpStreamSenderSettings.h"
#include "vector.h"

#include <system.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

ETcpStreamSenderInvalidIPPort::ETcpStreamSenderInvalidIPPort( UnicodeString inMsg ) :
								   Exception( inMsg )
{
}
//......................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::

TTcpStreamSender::TTcpStreamSender()
{
	TTcpStreamSenderSettings senderSettings = TTcpStreamSenderSettings::Instance();

	SetIPAddr( senderSettings.IPAddress );
	SetIPPort( senderSettings.IPPort    );
}
//............................................................................

TTcpStreamSender::TTcpStreamSender( AnsiString inTcpIPAddress, __int32 inTcpIPPort )
{
	SetIPAddr( inTcpIPAddress );
	SetIPPort( inTcpIPPort    );
}
//............................................................................

TTcpStreamSender::~TTcpStreamSender()
{
}
//............................................................................

void TTcpStreamSender::Send( TiXmlDocument* inXMLOrderDoc )
{
	TTcpStreamSenderThread *senderThread = new TTcpStreamSenderThread(
												createTcpClient(),
												inXMLOrderDoc );

	senderThread->OnTerminate = &sendThreadTerminated;
	senderThread->Resume();

	//setResponse( result, "0", false, "Invalid Receiver IP address.Empty IP address");
}
//............................................................................

void __fastcall TTcpStreamSender::sendThreadTerminated( TObject *Sender )
{
}
//............................................................................


#pragma mark ::::::::::::::::::::::::::::::
#pragma mark PRIVETE SECTION
#pragma mark ::::::::::::::::::::::::::::::

TIdTCPClient* TTcpStreamSender::createTcpClient()
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

void TTcpStreamSender::SetIPAddr( AnsiString inIPAddr )
{
	FIPAddr = inIPAddr;
}
//............................................................................

void TTcpStreamSender::SetIPPort( __int32 inIPPort )
{
	if( inIPPort < 56001 )
	{
		throw new ETcpStreamSenderInvalidIPPort( "Invalid TCP/IP Port number.  Port number must be equals or hihger than 600000" );
	}

	FIPPort = inIPPort;
}
//............................................................................

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  TTcpStreamSenderThread
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

__fastcall TTcpStreamSenderThread::TTcpStreamSenderThread(
									TIdTCPClient*  inTcpClient,
									TiXmlDocument* inXMLOrderDoc ) : TThread( true )
{
	FreeOnTerminate = true;

	_tcpClient   = inTcpClient;
	_xmlOrderDoc = inXMLOrderDoc;
}
//............................................................................

__fastcall TTcpStreamSenderThread::~TTcpStreamSenderThread()
{
   Terminate();
}

void __fastcall TTcpStreamSenderThread::Execute()
{
	if( _tcpClient != NULL )
   {
      while( !Terminated )
      {
         FResponse = SendAndFetch( _xmlOrderDoc );

         Terminate();
      }

      try
      {
         delete _tcpClient;
      }
      catch( ...)
      {
      }
   }
}
//............................................................................

TiXmlDocument* TTcpStreamSenderThread::SendAndFetch( TiXmlDocument* inXMLOrderDoc )
{

	TiXmlDocument* result = buildResponse( "0", false, "" );

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	try
	{
		bool retry      = false;
		int  retryCount = 0;
		do
		{
			_tcpClient->Connect();
			if( _tcpClient->Connected() )
			{
				sendData( _tcpClient, inXMLOrderDoc );
				fetchResponse( _tcpClient, result );
			}
			else
			{
				setResponse( result, "0", false, "Receiver not open");
			}
		}
		while( retry == true && retryCount < _DEFAULT_RETRY_COUNT );
	}
	catch( Exception& exc)
	{
		setResponse( result, "0", false, "Receiver not open");
	}

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//............................................................................

void TTcpStreamSenderThread::sendData( TIdTCPClient* inTcpClient, TiXmlDocument* inXMLOrderDoc )
{

      std::string data         = SerializeOut( inXMLOrderDoc ).c_str();
	__int32 dataSize         = data.size();
 	__int32 remainedDataSize = dataSize;
 //   __int32 remainedDataSize = data.size();

     __int32 maxBufferSize = data.size(); // It can be assigned a lower value
	__int32 itPos = 0;

	char   *buffer = new char[maxBufferSize];



	// Send Data Size
	inTcpClient->IOHandler->Write( maxBufferSize );

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

	//inTcpClient->IOHandler->WriteLn( data );
}
//............................................................................

void TTcpStreamSenderThread::fetchResponse( TIdTCPClient* inTcpClient, TiXmlDocument* outResponse )
{
	std::string data;

	if( inTcpClient->IOHandler->Readable( _READ_TIME_OUT ) )
	{
		__int32 dataSize  = inTcpClient->IOHandler->ReadLongInt();
		__int32 bytesRead = 0;

		while( bytesRead < dataSize )
		{
			TBytes buffer;
			inTcpClient->IOHandler->ReadBytes( buffer, dataSize, false );

			data +=  std::string( AnsiString( ( char* )&buffer[0], buffer.Length ).c_str() );

			bytesRead += buffer.Length;
		}

		if( data.size() )
		{
			SerializeIn( outResponse, AnsiString( data.c_str() ) );
		}
	}
}
//............................................................................

TBytes TTcpStreamSenderThread::createByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;
   
	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];

	}

	return result;
}
//............................................................................

TiXmlDocument* TTcpStreamSenderThread::buildResponse( AnsiString inOrderNo,
												bool inSuccess,
												AnsiString inMsg )
{
	TiXmlDocument* result = new TiXmlDocument();

	//::::::::::::::::::::::::::::::

    // add declaration
	TiXmlDeclaration *decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	result->LinkEndChild( decl );

	TiXmlElement* rootElem;
	addElement( ( TiXmlElement* )result, "Order", rootElem );

	setNodeAttr( rootElem, "action",      "orderResponse" );
	setNodeAttr( rootElem, "orderNumber", inOrderNo );
	setNodeAttr( rootElem, "result",      inSuccess ? "OK" : "FAILED" );
	setNodeAttr( rootElem, "message",     inMsg );

    //::::::::::::::::::::::::::::::

    return result;
}
void TTcpStreamSenderThread::setResponse( TiXmlDocument* inXmlDoc,
									AnsiString inOrderNo,
									bool inSuccess,
									AnsiString inMsg )
{
	TiXmlElement* rootElem = ( TiXmlElement* )inXmlDoc->RootElement();

	setNodeAttr( rootElem, "action",      "orderResponse" );
	setNodeAttr( rootElem, "orderNumber", inOrderNo );
	setNodeAttr( rootElem, "result",      inSuccess ? "OK" : "FAILED" );
	setNodeAttr( rootElem, "message",     inMsg );
}
//............................................................................

void TTcpStreamSenderThread::addElement( TiXmlElement* inParentElem, AnsiString inName, TiXmlElement* &inElem )
{
	inElem = new TiXmlElement(  inName.c_str() );
	inParentElem->LinkEndChild( inElem );
}
//----------------------------------------------------------------------------

void TTcpStreamSenderThread::setNodeAttr( TiXmlElement* inElem, AnsiString inAttrName, AnsiString inAttrValue )
{
    inElem->SetAttribute( inAttrName.c_str(), inAttrValue.c_str() );
}
//----------------------------------------------------------------------------

AnsiString TTcpStreamSenderThread::SerializeOut( TiXmlDocument* inXMLOrderDoc )
{
   AnsiString result = "";

   //::::::::::::::::::::::::::::::::::::::::::::

   TiXmlPrinter printer;

   printer.SetIndent( _T("\t") );
   inXMLOrderDoc->Accept( &printer );

   result = AnsiString( printer.CStr() );

   //::::::::::::::::::::::::::::::::::::::::::::

   return result;
}
//............................................................................

void TTcpStreamSenderThread::SerializeIn( TiXmlDocument* inXMLOrderDoc, AnsiString inData )
{
   //load it into the Doc
   inXMLOrderDoc->Parse( inData.c_str() );

   if( inXMLOrderDoc->Error() )
   {
   }
}
//............................................................................


