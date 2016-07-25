//---------------------------------------------------------------------------

#ifndef CSVH
#define CSVH
//---------------------------------------------------------------------------
class TCsv
{
private:
	TStringList *fStrings;
	UnicodeString GetCells(int ACol, int ARow);
	void SetCells(int ACol, int ARow, const UnicodeString Value);
	UnicodeString ColumnText(UnicodeString Row, int ACol);
	int GetRowCount();
	int GetColCount(int ARow);
public:
	TCsv();
	~TCsv();
	void LoadFromFile(const UnicodeString FileName);
	void SaveToFile(const UnicodeString FileName);
	bool AppendToFile(const UnicodeString FileName);
	__property UnicodeString Cells[int ACol][int ARow] = {read=GetCells, write=SetCells};
	void DeleteRow(int Row);
	UnicodeString GetRow(int Value);
	void Assign(TCsv * inData);	
	int Add(const UnicodeString Value);		// Be carfull using this. You are better off adding a blank string,
													// then use Cells[][] to fill it in. Then you don't need to worry about
													// Escape chars etc.
													// e.g. Csv.Cells[3][Csv.Add()] = "Data"; (Not sure about the syntax!!)
	void Clear();
	__property int RowCount = {read=GetRowCount};
	__property int ColCount[int ARow] = {read=GetColCount};

	char Delimiter;		// Default: ','.	Used to separate values in a row.
//	char Qualifier;		// Default: None.	Used to surround text containg the delimiter character.
	char EscapeChar;		// Default: '\'
   int FindFirst(int ACol, UnicodeString Val);		// Preceed delimiter to include as text. Don't use the same as the delimiter.
};
//---------------------------------------------------------------------------
#endif

