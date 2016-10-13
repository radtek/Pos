// ---------------------------------------------------------------------------

#pragma hdrstop

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "ReqPrintJob.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall SortFunc(void *Item1, void *Item2)
{
	TItemComplete *Order1 = (TItemComplete*)Item1;
	TItemComplete *Order2 = (TItemComplete*)Item2;
	if (Order1->ContainerTabType < Order2->ContainerTabType)
	{
		return -1;
	}
	else if (Order1->ContainerTabType == Order2->ContainerTabType)
	{
		if (Order1->InvoiceNumber < Order2->InvoiceNumber)
		{
			return -1;
		}
		else if (Order1->InvoiceNumber == Order2->InvoiceNumber)
		{
			// On consolidate items for Tables.
			if (Order1->TabType == TabTableSeat || Order2->TabType == TabTableSeat)
			{
				if (Order1->TabType < Order2->TabType)
				{
					return -1;
				}
				else if (Order1->TabType == Order2->TabType)
				{
					if (Order1->RoomNo < Order2->RoomNo)
					{
						return -1;
					}
					else if (Order1->RoomNo == Order2->RoomNo)
					{
						if (Order1->TableNo < Order2->TableNo)
						{
							return -1;
						}
						else if (Order1->TableNo == Order2->TableNo)
						{
							if ((Order1->SeatNo < Order2->SeatNo) && (!Receipt->ConsolidateReceipt))
							{
								return -1;
							}
							else if ((Order1->SeatNo == Order2->SeatNo) || (Receipt->ConsolidateReceipt))
							{
								if (Order1->TabName < Order2->TabName && (!Receipt->ConsolidateReceipt))
								{
									return -1;
								}
								else if (Order1->TabName == Order2->TabName || (Receipt->ConsolidateReceipt))
								{
									if ((Order1->TabKey < Order2->TabKey) && (!Receipt->ConsolidateReceipt))
									{
										return -1;
									}
									else if ((Order1->TabKey == Order2->TabKey) || (Receipt->ConsolidateReceipt))
									{
										if (Order1->Item < Order2->Item)
										{
											return -1;
										}
										else if (Order1->Item == Order2->Item)
										{
											if (Order1->PriceEach() < Order2->PriceEach())
											{
												return -1;
											}
											else if (Order1->PriceEach() == Order2->PriceEach())
											{
												if (Order1->TotalDiscount() < Order2->TotalDiscount())
												{
													return -1;
												}
												else if (Order1->TotalDiscount() == Order2->TotalDiscount())
												{
													if (Order1->DiscountReason < Order2->DiscountReason)
													{
														return -1;
													}
													else if (Order1->DiscountReason == Order2->DiscountReason)
													{
														if (Order1->OrderType < Order2->OrderType)
														{
															return -1;
														}
														else if (Order1->OrderType == Order2->OrderType)
														{
															return 0;
														}
														else
														{
															return 1;
														}
													}
													else
													{
														return 1;
													}
												}
												else
												{
													return 1;
												}
											}
											else
											{
												return 1;
											}
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
								}
								else
								{
									return 1;
								}
							}
							else
							{
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if (Order1->TabType < Order2->TabType)
				{
					return -1;
				}
				else if (Order1->TabType == Order2->TabType)
				{
					if (Order1->RoomNo < Order2->RoomNo)
					{
						return -1;
					}
					else if (Order1->RoomNo == Order2->RoomNo)
					{
						if (Order1->TableNo < Order2->TableNo)
						{
							return -1;
						}
						else if (Order1->TableNo == Order2->TableNo)
						{
							if ((Order1->SeatNo < Order2->SeatNo))
							{
								return -1;
							}
							else if ((Order1->SeatNo == Order2->SeatNo))
							{
								if (Order1->TabName < Order2->TabName)
								{
									return -1;
								}
								else if (Order1->TabName == Order2->TabName)
								{
									if ((Order1->TabKey < Order2->TabKey))
									{
										return -1;
									}
									else if ((Order1->TabKey == Order2->TabKey))
									{
										if (Order1->Item < Order2->Item)
										{
											return -1;
										}
										else if (Order1->Item == Order2->Item)
										{
											if (Order1->Price() < Order2->Price())
											{
												return -1;
											}
											else if (Order1->Price() == Order2->Price())
											{
												if (Order1->TotalDiscount() < Order2->TotalDiscount())
												{
													return -1;
												}
												else if (Order1->TotalDiscount() == Order2->TotalDiscount())
												{
													if (Order1->DiscountReason < Order2->DiscountReason)
													{
														return -1;
													}
													else if (Order1->DiscountReason == Order2->DiscountReason)
													{
														if (Order1->OrderType < Order2->OrderType)
														{
															return -1;
														}
														else if (Order1->OrderType == Order2->OrderType)
														{
															return 0;
														}
														else
														{
															return 1;
														}
													}
													else
													{
														return 1;
													}
												}
												else
												{
													return 1;
												}
											}
											else
											{
												return 1;
											}
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
								}
								else
								{
									return 1;
								}
							}
							else
							{
								return 1;
							}
						}
						else
						{
							return 1;
						}
					}
					else
					{
						return 1;
					}
				}
				else
				{
					return 1;
				}
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}
// ---------------------------------------------------------------------------

_fastcall TReqPrintJob::TReqPrintJob(TDeviceDB *Device) : TRequest(Device)
{
	if (Device == NULL)
	{
		Header.RequestingDevice->Copy(&TDeviceRealTerminal::Instance());
		Header.Device = &TDeviceRealTerminal::Instance();
	}
	else
	{
		Header.RequestingDevice->Copy(Device);
		Header.Device = Device;
	}
	Header.Error = proA_Ok;
	Header.ErrorMsg = "";

	OrderLocalKeys.clear();
	PrintJobs = new TList;
	Printouts = new TPrintouts;
	ExtraInfo = new TStringList;
	DeliveryInfo = new TStringList;
	PaymentInfo = new TStringList;
	TabHistory = new TStringList;
    OrderComments = new TStringList;

	ReceiptHeader = new TStringList;
	ReceiptPHeader = new TStringList;
	ReceiptFooter = new TStringList;
    ReceiptDetails = new TStringList;

	Echo = false;
	PaymentType = ptPreliminary;
	DocketNumber = "";
	ptrSortFunc = SortFunc; // Function ptr.
	TimeStamp = Now();

	Sender = devUnknown;
	Waiter = "";
	// Loyalty = false;
	JobType = pjInit;
	SignReceipt = false;
	AccountPayment = false;
	AccountInvoiceNumber = "";
	WaitTime = 0;
	BarCodeData = 0;
	Transaction = NULL;
    ReceiptVoidFooter = new TStringList;
}
// ---------------------------------------------------------------------------

_fastcall TReqPrintJob::~TReqPrintJob()
{
	while (PrintJobs->Count != 0)
	{
		TPrintJob *PrintJob = (TPrintJob*)PrintJobs->Items[0];
		delete PrintJob;
		PrintJobs->Delete(0);
	}
	delete PrintJobs;
	while (Printouts->Count != 0)
	{
		TPrintout *Printout = (TPrintout*)Printouts->Items[0];
		delete Printout;
		Printouts->Delete(0);
	}
	delete Printouts;
	delete ExtraInfo;
	delete DeliveryInfo;
	delete PaymentInfo;
    delete OrderComments;
	delete TabHistory;
	delete ReceiptHeader;
	delete ReceiptPHeader;
	delete ReceiptFooter;
    delete ReceiptDetails;
    delete ReceiptVoidFooter;
}
