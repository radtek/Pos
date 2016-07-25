//---------------------------------------------------------------------------

#ifndef PointsTransactionH
#define PointsTransactionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "blowfish.h"
#include <memory>
#include <vector>
#include "enumPoints.h"
#include "PointsType.h"
//---------------------------------------------------------------------------

class TPointsTransaction
{
   private :
      std::auto_ptr<TMemoryStream> InternalStream;
      void EncryptStream(TMemoryStream *Data);
      void DecryptStream(TMemoryStream *Data);
   public :
		void operator = (TPointsTransaction rhs);

		int PointsTransactionsKey;
		int ContactKey;
		TDateTime TimeStamp;
		TDateTime TimeStampExported;
		TPointsTransactionType PointsTransactionType;
		TPointsTransactionAccountType PointsTransactionAccountType;
		Currency Adjustment;
		TPointsExportStatus ExportStatus;
		UnicodeString InvoiceNumber;

		TPointsTransaction();
		TPointsTransaction(const TPointsTransaction &PointsTransaction);
		TMemoryStream *GetStream() const;
		TMemoryStream *GetAsStream();
		TMemoryStream &GetStreamRef();

		void SaveToStream(TMemoryStream *Stream);
		bool Validate();

        TPointsTransaction::Build(int inContactKey,
                                  std::pair<TPointsType, Currency> Detials,
                                  TPointsExportStatus inExportStatus,
                                  UnicodeString InvoiceNo,
                                  TDateTime inTimeStamp,
                                  TDateTime inTimeStampExported,
                                  bool isRefundingItems = false);
};

typedef std::vector<TPointsTransaction> TPointsTransactionContainer;

#endif
