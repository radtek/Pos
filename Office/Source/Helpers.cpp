//---------------------------------------------------------------------------


#pragma hdrstop

#include "Helpers.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
bool Helpers::SendInvoiceToXero(const char *invoiceData, size_t size)
{
   bool result = false;

   HANDLE xeroPipe = INVALID_HANDLE_VALUE;

    if (WaitNamedPipe("\\\\.\\pipe\\Xero", 25000))
    {

      xeroPipe = CreateFile(
                        "\\\\.\\pipe\\Xero",
                        GENERIC_WRITE,
							   0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL
      );

		if (xeroPipe == INVALID_HANDLE_VALUE)
        {
            AnsiString Message = "Error opening connection to XeroMate!";
		    Application->MessageBox(Message.c_str(), "Connection Error!", MB_OK);
		}
		else
        {
			char* buf;

			try
            {
				buf = (char *)malloc(size);

				sprintf(buf, "%s", invoiceData);
				DWORD bytesWritten;

				if (WriteFile(xeroPipe, (void*)buf, size, &bytesWritten, NULL))
                {
                    result = true;
                }
                else
                {
            	    AnsiString Message = "Error writing data to XeroMate!";
         	        Application->MessageBox(Message.c_str(), "Transmission Error!", MB_OK);
                }
			}
			__finally
            {
				free(buf);
                CloseHandle(xeroPipe);
			}
		}
	}
	else
    {
		AnsiString Message = "Error opening connection to XeroMate!";
	    Application->MessageBox(Message.c_str(), "Connection Error!", MB_OK);
	}

    return result;
}

