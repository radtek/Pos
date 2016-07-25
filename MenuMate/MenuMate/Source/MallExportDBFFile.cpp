//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportDBFFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

MallExportDBFFile::MallExportDBFFile()
{
    m_pFileHandle = NULL;
    m_nNumFields = 0;
    m_bAllowWrite = false;
    m_bStructSizesOK = true;
    m_pRecord = new char[MAX_RECORD_SIZE];
}
//---------------------------------------------------------------------------

MallExportDBFFile::~MallExportDBFFile()
{
    if( m_pFileHandle != NULL )
    {
        fclose(m_pFileHandle);
    }

    m_pFileHandle = NULL;
    delete [] m_pRecord;
}
//---------------------------------------------------------------------------

// This method opens a dbf file for reading only
int MallExportDBFFile::open(string sFileName,bool bAllowWrite)
{
    m_sFileName = sFileName;
    if( bAllowWrite && !m_bStructSizesOK )
    {
        bAllowWrite = false; // DO NOT WRITE IF ENGINE IS NOT COMPILED PROPERLY!
    }
    m_bAllowWrite = bAllowWrite;

    char cMode[10] = "rb"; // For windows we MUST open in binary mode ALWAYS!!!  Linux does not care
    if( m_bAllowWrite )
    {
        strncpy(cMode,"rb+",3); // Change to read write mode
    }

    m_pFileHandle = fopen(sFileName.c_str(),cMode);

    // Unable to open file Error
    if( m_pFileHandle == NULL )
    {
       return errno;
    }

    // If open is ok, so read in the File Header
    int nBytesRead = fread (&m_FileHeader,1,32,m_pFileHandle);

    // Bad Read for Hearder. For DBF it should be 32
    if( nBytesRead != 32 )
    {
        return 1;
    }

    m_nNumFields = 0;

    do
    {
        int nBytesRead = fread(&(m_FieldDefinitions[m_nNumFields]),1,32,m_pFileHandle);
        // Bad Read for Hearder. For DBF it should be 32
        if( nBytesRead != 32 )
        {
            return 1;
        }

        if( m_FieldDefinitions[m_nNumFields].cFieldName[0] == 0x0D || strlen(m_FieldDefinitions[m_nNumFields].cFieldName) <= 1 )
        {
            break;
        }

        m_nNumFields++;
    }
    while(!feof(m_pFileHandle));

    // Move to start of first record
    int nFilePosForRec0 = 32+32*m_nNumFields+264;

    // Bad Rec0 position Error
    if( m_FileHeader.uPositionOfFirstRecord != nFilePosForRec0 )
    {
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

int MallExportDBFFile::close()
{
    int nRet = fclose(m_pFileHandle);
    m_pFileHandle = NULL;
    m_sFileName = "";
    m_nNumFields = 0;
    m_bAllowWrite = false;
    m_FileHeader.u8FileType = 0;
    return nRet;
}
//---------------------------------------------------------------------------

int MallExportDBFFile::create(string sFileName,int nNumFields)
{
    // Unable to create new DBF because engine is compiled incorrectly
    if( !m_bStructSizesOK )
    {
        return 1;
    }

    // Create a new dbf file with space for nNumFields
    if( m_pFileHandle != NULL )
    {
        close();
    }
    m_sFileName = sFileName;
    m_bAllowWrite = true;
    m_nNumFields = nNumFields;

    m_pFileHandle = fopen(sFileName.c_str(),"wb+"); // create a new empty file for binary writing

    // Unable to Create file Error
    if( m_pFileHandle == NULL )
    {
       return errno;
    }

    // Setup the file header
    m_FileHeader.u8FileType = 0x43;
//    m_FileHeader.u8FileType = 0x63;
//    m_FileHeader.u8FileType = 0x8B;
//    m_FileHeader.u8FileType = 0xCB;

//    m_FileHeader.u8FileType = 0x03;
//    m_FileHeader.u8FileType = 0x83;
//    m_FileHeader.u8FileType = 0xF5;

    for( int i=0; i<16;i++)
    {
        m_FileHeader.Reserved16[i] = 0;
    }
    for( int i=0; i<2;i++)
    {
        m_FileHeader.Reserved2[i] = 0;
    }

    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    int nYear = timePtr->tm_year % 100; // convert yr to 2 digits
    m_FileHeader.u8LastUpdateDay = timePtr->tm_mday;
    m_FileHeader.u8LastUpdateMonth = timePtr->tm_mon+1;
    m_FileHeader.u8LastUpdateYear = nYear;
    m_FileHeader.uCodePage = 0; // copied from another db, no idea what this corresponds to
    m_FileHeader.uPositionOfFirstRecord = 32+32*nNumFields+264; // calculated based on the file header size plus the n*FieldDef size + 1 term char + 263 zeros
    m_FileHeader.uRecordLength = 0;
    m_FileHeader.uRecordsInFile = 0;
    m_FileHeader.uTableFlags = 0; // bit fields, copied from another db , 0x01=has a .cdx?, 0x02=Has Memo Fields, 0x04=is a .dbc?

    // write the File Header for the first time!
    fwrite(&m_FileHeader,1,sizeof(m_FileHeader),m_pFileHandle);

    // now write dummy field definition records until the real ones can be specified
    for( int i = 0; i < nNumFields ; i++ )
    {
        for( int j = 0; j < 11 ; j++ )
        {
            m_FieldDefinitions[i].cFieldName[j]=0; // clear entire name
        }
        m_FieldDefinitions[i].cFieldType='C';
        m_FieldDefinitions[i].FieldFlags=0;
        m_FieldDefinitions[i].uAutoIncrementStep=0;
        m_FieldDefinitions[i].uNextAutoIncrementValue[0]=0;
        m_FieldDefinitions[i].uNextAutoIncrementValue[1]=0;
        m_FieldDefinitions[i].uNextAutoIncrementValue[2]=0;
        m_FieldDefinitions[i].uNextAutoIncrementValue[3]=0;
        m_FieldDefinitions[i].uLength=0;
        m_FieldDefinitions[i].uNumberOfDecimalPlaces=0;
        m_FieldDefinitions[i].Reserved8[i]=0;

        // write the definitions
        fwrite(&m_FieldDefinitions[i],1,sizeof(fieldDefinition),m_pFileHandle);
    }
    // write the field definition termination character
    char FieldDefTermination[2];
    FieldDefTermination[0] = 0x0D;
    FieldDefTermination[1] = 0;

    fwrite(FieldDefTermination,1,1,m_pFileHandle);
    // write the 263 bytes of 0
    char cZero[263];
    for( int j=0; j<263;j++)
    {
        cZero[j]=0;
    }

    fwrite(&cZero[0],1,263,m_pFileHandle);
    // this is now the starting point for the first record
    // ready to assign the field definitions!

    // make sure change is made permanent, we are not looking for speed, just reliability and compatibility
    fflush(m_pFileHandle);

    return 0;
}
//---------------------------------------------------------------------------

int MallExportDBFFile::updateFileHeader()
{
    // move to file start
    int nRes = fseek(m_pFileHandle,0,SEEK_SET);
    if( nRes != 0)
    {
        return 1; //fail
    }

    // update the last modified date
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    int nYear = timePtr->tm_year % 100; // convert yr to 2 digits
    m_FileHeader.u8LastUpdateDay = timePtr->tm_mday;
    m_FileHeader.u8LastUpdateMonth = timePtr->tm_mon+1;
    m_FileHeader.u8LastUpdateYear = nYear;

    // Write the current header info
    int nBytesWritten = fwrite(&m_FileHeader,1,sizeof(m_FileHeader),m_pFileHandle);

    // Failt ot update Header Error
    if( nBytesWritten != sizeof(m_FileHeader) )
    {
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------

int MallExportDBFFile::assignField(fieldDefinition fd,int nField)
{
    // Used to assign the field info ONLY if num records in file = 0 !!!
    // Failed to AssignField Can not change Fields once the File has records in it Error
    if( m_FileHeader.uRecordsInFile != 0)
    {
        return 1; // fail
    }

    // Set the unused characters for the field name to zero
    int nPos = strlen(fd.cFieldName);
    for( int i=nPos; i < 11 ; i++ )
    {
        fd.cFieldName[i]=0;
    }

    // This engine does not support auto increment, set it to zero
    fd.uAutoIncrementStep=0;
    fd.uNextAutoIncrementValue[0]=0;
    fd.uNextAutoIncrementValue[1]=0;
    fd.uNextAutoIncrementValue[2]=0;
    fd.uNextAutoIncrementValue[3]=0;

    for( int i=0; i<8;i++)
    {
        fd.Reserved8[i] = 0; // must always be set to zeros!
    }

    // Add some rules to prevent creation of invalid db.
    if( fd.cFieldType=='I' )
    {
        fd.uLength = 4;
    }
    else if( fd.cFieldType=='B' )
    {
        fd.uLength = 8; // actual double, not text!
    }
    else if( fd.cFieldType=='L' )
    {
        fd.uLength = 1;
    }
    else
    {
        //default case
        if( fd.uLength < 1 )
        {
            fd.uLength=1;
        }
    }

    // calculate the proper field offset based on corrected data
    if( nField == 0 )
    {
        fd.uFieldOffset = 1;
    }
    else
    {
        fd.uFieldOffset = 1;
        for( int i=0;i<nField;i++ )
        {
            fd.uFieldOffset += m_FieldDefinitions[i].uLength;
        }
    }

    int nPosOfFieldDef = 32+nField*32;
    int nRes = fseek(m_pFileHandle,nPosOfFieldDef,SEEK_SET);
    if( nRes != 0)
    {
        return 1; //fail
    }
    int nBytesWritten = fwrite(&fd,1,sizeof(fieldDefinition),m_pFileHandle);

    // Failed to update field definition error
    if( nBytesWritten != sizeof(m_FileHeader) )
    {
        return 1;
    }
    // Update the in memory definition too
    m_FieldDefinitions[nField] = fd;

    // Update the total record length, and the header record!
    m_FileHeader.uRecordLength = 1; // 1 byte for delete flag
    for( int i=0;i<= nField ;i++ )
    {
        m_FileHeader.uRecordLength += m_FieldDefinitions[i].uLength;
    }

    updateFileHeader();

    return 0;
}
//---------------------------------------------------------------------------

int MallExportDBFFile::appendRecord(string *sValues,int nNumValues)
{
    // Used to add records to the dbf file (append to end of file only)
    // Cannot add new record, wrong number of Values Given error
    if( nNumValues != m_nNumFields )
    {
        return 1;
    }

    // Calculate the proper location for this record
    int nRecPos = 32 + 32*m_nNumFields + 264 + m_FileHeader.uRecordLength * m_FileHeader.uRecordsInFile;
    int nRes = fseek(m_pFileHandle,nRecPos,SEEK_SET);

    // Error seeking to new record position
    if (nRes != 0 )
    {
        return 1; //fail
    }

    // Clear record
    for( int i = 0 ; i < m_FileHeader.uRecordLength ; i++ )
    {
        m_pRecord[i] = 0;
    }
    m_pRecord[0] = ' '; // clear the deleted flag for the new record

    // File position is now at end of file

    for( int f=0;f<m_nNumFields;f++)
    {
        // Pull field value out of string record
        string sFieldValue = sValues[f];
        char cType = m_FieldDefinitions[f].cFieldType;
        if( cType == 'I' )
        {
            // Convert string version of INT, into actual int, and save into the record
            int res = ConvertStringToInt(sFieldValue,m_FieldDefinitions[f].uLength,&m_pRecord[m_FieldDefinitions[f].uFieldOffset]);
        }
        else if( cType== 'B' )
        {
            // float or double
            int res = ConvertStringToFloat(sFieldValue,m_FieldDefinitions[f].uLength,&m_pRecord[m_FieldDefinitions[f].uFieldOffset]);
        }
        else if( cType== 'L' )
        {
            // logical
            if( sFieldValue=="T" || sFieldValue=="TRUE" )
            {
                m_pRecord[m_FieldDefinitions[f].uFieldOffset] = 'T';
            }
            else if( sFieldValue=="?")
            {
                m_pRecord[m_FieldDefinitions[f].uFieldOffset] = '?';
            }
            else
            {
                m_pRecord[m_FieldDefinitions[f].uFieldOffset] = 'F';
            }
        }
        else
        {
            // default for character type fields (and all unhandled field types)
            for( unsigned int j=0;j<m_FieldDefinitions[f].uLength;j++)
            {
                int n = m_FieldDefinitions[f].uFieldOffset + j;
                if( j < sFieldValue.length() )
                {
                    m_pRecord[n] = sFieldValue[j];
                }
                else
                {
                    m_pRecord[n] = 0; // zero fill remainder of field
                }
            }
        }
    }
    // Write the record at the end of the file
    int nBytesWritten = fwrite(&m_pRecord[0],1,m_FileHeader.uRecordLength,m_pFileHandle);

    // Failed to write new record error
    if( nBytesWritten != m_FileHeader.uRecordLength )
    {
        return 1;
    }

    // Update the header to reflect the New record count
    m_FileHeader.uRecordsInFile++;
    updateFileHeader();

    // Make sure change is made permanent, we are not looking for speed, just reliability and compatibility
    fflush(m_pFileHandle);
    return 0;
}
//---------------------------------------------------------------------------
