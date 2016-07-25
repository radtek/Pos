//---------------------------------------------------------------------------

#ifndef CSVH
#define CSVH
//---------------------------------------------------------------------------
class TCsv       
{
private:
	TStringList *fStrings;
	AnsiString GetCells(int ACol, int ARow);
	AnsiString GetLine(int ARow);
	void SetCells(int ACol, int ARow, const AnsiString Value);
	AnsiString ColumnText(AnsiString Row, int ACol);
	int GetRowCount();
	int GetColCount(int ARow);
public:
	TCsv();
	~TCsv();
	void LoadFromFile(const AnsiString FileName);
	void SaveToFile(const AnsiString FileName);
	__property AnsiString Cells[int ACol][int ARow] = {read=GetCells, write=SetCells};
	__property AnsiString Line[int ARow] = {read=GetLine};
	void DeleteRow(int Row);
	int Add(const AnsiString Value);		// Be carfull using this. You are better off adding a blank string,
													// then use Cells[][] to fill it in. Then you don't need to worry about
													// Escape chars etc.
													// e.g. Csv.Cells[3][Csv.Add(",,,,")] = "Data"; (Not sure about the syntax!!)
	void Clear();
	__property int RowCount = {read=GetRowCount};
	__property int ColCount[int ARow] = {read=GetColCount};

	char Delimiter;		// Default: ",".	Used to separate values in a row.
	char Qualifier;		// Default: None.	Used to surround text containg the delimiter character.
	char EscapeChar;		// Preceed delimiter to include as text. Don't use the same as the delimiter.
};
//---------------------------------------------------------------------------
#endif

