//---------------------------------------------------------------------------

#ifndef PSectionInstructionH
#define PSectionInstructionH
//---------------------------------------------------------------------------
#include <vector>
#include <IBSQL.hpp>

#include "enumPrintOutFormatInstructions.h"
#include "PrintInfo.h"


class TPSectionInstruction;

typedef std::vector<TPSectionInstruction> TSectionInstructStorage;
typedef std::vector< TPSectionInstruction * > TPtrSectionInstructStorage;

class TPSectionInstruction
{
	private :
	int ProfileKey;
	UnicodeString InternalProfileName;
	public :

	ePrintOutFormatInstructions Instruction;

   TPtrSectionInstructStorage SubInstructions;
	int OrderNumber;
	int GroupNo;
	UnicodeString Caption;
   UnicodeString InstructionCaption;

	bool LineAbove;
	bool LineBelow;
	bool DrawLineAbove;
	bool DrawLineBelow;
	bool Cut;
	bool Visible;
	TPrintInfo FontInfo;
	void Load(TIBSQL *Query);
	void Save(TIBSQL *Query);
	TPSectionInstruction();
	TPSectionInstruction(ePrintOutFormatInstructions inInstruction);
};

#endif
