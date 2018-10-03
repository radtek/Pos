using Microsoft.PointOfService;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;
//using System.Threading.Tasks;
using FiscalLibraries.Domain;

namespace FiscalLibraries
{
    [Guid("b99565f0-9f3e-4719-bb57-4f4a941adb68")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("FiscalLibraries.FiscalLibraryClass")]
    public class FiscalLibraryClass : IFiscalLibrary
    { 

        private List<FiscalService> ItemList;
        private List<FiscalPayment> PaymentList;
        private List<DiscountDetails> DiscountList;

        private FiscalService ItemComplete;
        private FiscalPayment PaymentInfo;
        private DiscountDetails DiscountInfo;

        PosExplorer posExplorer;
        PosCommon posCommonFP;

        public string InvoiceNumber { get; set; }

        public string Date { get; set; }

        public string Time { get; set; }

        public string Billno { get; set; }

        public string Cashno { get; set; }

        public string Cashier { get; set; }

        public string Source { get; set; }

        public string TerminalName { get; set; }

        public string PointPurchased { get; set; }
        public string TabCredit { get; set; }
        public string PrinterType { get; set; }
        public string PrinterLogicalName { get; set; }
        public string Saletype { get; set; }
        public bool OpenCD { get; set; }//CD stands for cashdrawer.

        List<string> stringList;

        public FiscalLibraryClass()
        {
            stringList = new List<string>();
            ItemList = new List<FiscalService>();
            PaymentList = new List<FiscalPayment>();
            DiscountList = new List<DiscountDetails>();
            stringList.Add("FiscalLibraryClass consructor");
        }

        public void InitializeItemProperties()
        {
            stringList.Add("InitializeItemProperties");
            ItemComplete = new FiscalService();
        }

        public void InitializePaymentProperties()
        {
            stringList.Add("InitializePaymentProperties");
            PaymentInfo = new FiscalPayment();
        }

        public void InitializeDiscountProperties()
        {
            stringList.Add("InitializeDiscountProperties");
            DiscountInfo = new DiscountDetails();
        }

        public void LoadReceiptItemInfo(int identifier, string fieldValue)
        {
            try
            {
                switch (identifier)
                {
                    case 0:
                        ItemComplete.ChitNumber = fieldValue;
                        break;
                    case 1:
                        ItemComplete.GuestName = fieldValue;
                        break;
                    case 2:
                        ItemComplete.ItemCategory = fieldValue;
                        break;
                    case 3:
                        ItemComplete.ItemDescription = fieldValue;
                        break;
                    case 4:
                        ItemComplete.MemberName = fieldValue;
                        break;
                    case 5:
                        ItemComplete.PartyName = fieldValue;
                        break;
                    case 6:
                        ItemComplete.PricePerUnit = fieldValue;
                        break;
                    case 7:
                        ItemComplete.PriceTotal = fieldValue;
                        break;
                    case 8:
                        ItemComplete.Quantity = fieldValue;
                        break;
                    case 9:
                        ItemComplete.SizeName = fieldValue;
                        break;
                    case 10:
                        ItemComplete.TableNo = fieldValue;
                        break;
                    case 11:
                        ItemComplete.VATPercentage = fieldValue;
                        break;
                    default:
                        break;
                }
            }
            catch (Exception Ex)
            {
                stringList.Add("exception in LoadReceiptItemInfo switch  " + Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();
            }
        }

        public void LoadReceiptPaymentInfo(int identifier, string fieldValue)
        {
            try
            {
                switch (identifier)
                {
                    case 0:
                        PaymentInfo.Type = fieldValue;
                        break;
                    case 1:
                        PaymentInfo.Amount = fieldValue;
                        break;
                    case 2:
                        PaymentInfo.Billno = fieldValue;
                        break;
                    case 3:
                        PaymentInfo.Cashno = fieldValue;
                        break;
                    case 4:
                        PaymentInfo.Cashier = fieldValue;
                        break;
                    case 5:
                        PaymentInfo.Source = fieldValue;
                        break;
                    case 6:
                        PaymentInfo.Description = fieldValue;
                        break;
                    case 7:
                        PaymentInfo.TipAmount = fieldValue;
                        break;
                    case 8:
                        PaymentInfo.PaymentSurcharge = fieldValue;
                        break;
                    case 9:
                        PaymentInfo.ChangeAmount = fieldValue;
                        break;
                    case 10:
                        PaymentInfo.IsTipAppliedFromPOS = fieldValue;
                        break;
                    default:
                        break;
                }
            }
            catch (Exception Ex)
            {
                stringList.Add("exception in LoadReceiptPaymentInfo switch  " + Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();
            }
        }

        public void LoadReceiptDiscountInfo(int identifier, string fieldValue)
        {
            try
            {
                switch (identifier)
                {
                    case 0:
                        DiscountInfo.Amount = fieldValue;
                        break;
                    case 1:
                        DiscountInfo.Description = fieldValue;
                        break;
                    case 2:
                        DiscountInfo.DiscountGroup = fieldValue;
                        break;
                    case 3:
                        DiscountInfo.DiscountMode = fieldValue;
                        break;
                    case 4:
                        DiscountInfo.Type = fieldValue;
                        break;
                    default:
                        break;
                }
            }
            catch (Exception Ex)
            {
                stringList.Add("exception in LoadReceiptDiscountInfo switch  " + Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();
            }
        }

        public void AddItemToList()
        {
            ItemList.Add(ItemComplete);
            stringList.Add("Item List size" + ItemList.Count);
        }

        public void AddPaymentInfoToList()
        {
            PaymentList.Add(PaymentInfo);
            stringList.Add("PaymentList List size" + PaymentList.Count);
        }

        public void AddDiscountInfoToList()
        {
            DiscountList.Add(DiscountInfo);
            stringList.Add("DiscountList After adding List size" + DiscountList.Count);
        }

        /*****************************Code for port Open******************************************/
        public bool PortOpen()
        {
            try
            {
                stringList.Add("Trying To Open Port " + DateTime.Now.ToString("hh:mm:ss tt"));
                posExplorer = new PosExplorer();
                //DeviceInfo fp = posExplorer.GetDevice(PrinterType, PrinterLogicalName);
               DeviceInfo fp = posExplorer.GetDevice("FiscalPrinter", "EpsonFP1");
                posCommonFP = (PosCommon)posExplorer.CreateInstance(fp);
                 // posCommonFP.StatusUpdateEvent += new StatusUpdateEventHandler(co_OnStatusUpdateEvent);
                fiscalprinter = (FiscalPrinter)posCommonFP;
                stringList.Add("Open");
                fiscalprinter.Open();

                stringList.Add("Claim");
                fiscalprinter.Claim(5000);

                stringList.Add("PowerNotify");
                fiscalprinter.PowerNotify = PowerNotification.Enabled;

                stringList.Add("DeviceEnabled");
                fiscalprinter.DeviceEnabled = true;
                stringList.Add("DeviceEnabled Complete");

            }       //stringList.Clear();

            catch (Exception Ex)
            {
                stringList.Add("Exception in PortOpen() " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception  " + Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();
                fiscalprinter.Close();
                return false;
            }
            WriteToFile(stringList);
            stringList.Clear();
            return true;
        }
    
        /*************************Code for open port and retry**********************************************/
        FiscalPrinter fiscalprinter;
        public bool CheckPortOpenAndRetry()
        {
            bool checkPortStatus =false;

            try
            {
                //  stringList.Add(fiscalprinter.State.ToString());
                checkPortStatus = PortOpen();
                for (int i = 0; i < 3; i++)
                {
                    stringList.Add("Going to retry" + i);
                    if (!checkPortStatus)
                    {

                        System.Threading.Thread.Sleep(3000);
                        checkPortStatus = PortOpen(); ;
                    }
                    else { break; }
                }
            }
            catch (Exception Ex)
            {

                stringList.Add("catch block of second Try block" + Ex.Message);
                stringList.Add(DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add(Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();

              
                fiscalprinter.Close();
            }
            WriteToFile(stringList);
            stringList.Clear();
            return checkPortStatus;
        }
        /***********************************************************************/
        public string PrintReceipt()
        {
            string printResponseString = "";

            try
            {
                string ReceiptNo = "        Receipt No. " + InvoiceNumber;
                bool isNormalSale = true;

                if(!CheckPortOpenAndRetry())
                { return "Printer is not responding or might be busy"; }
                fiscalprinter.ResetPrinter();
                stringList.Add("Printer reset ");

                isNormalSale = Convert.ToBoolean(Convert.ToInt32(Saletype));
                if (fiscalprinter.CapFiscalReceiptType)
                {
                    if (isNormalSale)
                        fiscalprinter.FiscalReceiptType = FiscalReceiptType.Sales;
                    else
                        fiscalprinter.FiscalReceiptType = FiscalReceiptType.Refund;
                }
                 
                stringList.Add("receipt type " + Saletype);
                stringList.Add("Invoice Number is  " + InvoiceNumber);
                stringList.Add("Before beginning fiscal receipt ");

                fiscalprinter.BeginFiscalReceipt(true);
                stringList.Add("BeginFiscalReceipt ");

                decimal multiplier = Convert.ToDecimal(10000);
                try
                {
                    decimal totalItemprice = 0, totalPaymentcollected = 0, totalTipAmount = 0, totalPaymentSurcharge = 0, totalChange = 0,
                            totalDiscount = 0, totalSurcharge = 0, pointsPurchased = 0, tabCredit = 0;
                    fiscalprinter.PrintRecMessage(ReceiptNo);

                    stringList.Add("Before iterating item total loop ");
                    foreach (FiscalService fiscalItem in ItemList)
                    {
                        //decimal total = (Convert.ToDecimal(fiscalItem.PriceTotal)) * multiplier;
                        decimal total = (Convert.ToDecimal(fiscalItem.PricePerUnit) * Convert.ToDecimal(fiscalItem.Quantity));
                        total = Math.Round(total, 2) * multiplier;

                        decimal qtyDecimal = (Convert.ToDecimal(fiscalItem.Quantity)) * 1000;
                        int qty = Convert.ToInt32(qtyDecimal);
                        int vat = 10;//(Convert.ToInt32(fiscalItem.VATPercentage)) * 1000;
                        decimal ppu = (Convert.ToDecimal(fiscalItem.PricePerUnit)) * multiplier;
                        string sizeName = fiscalItem.SizeName;
                        totalItemprice += total;
                        fiscalItem.ItemDescription = fiscalItem.SizeName + " " + fiscalItem.ItemDescription;
                        int size = fiscalItem.ItemDescription.Length > 30 ? 30 : fiscalItem.ItemDescription.Length;
                        fiscalItem.ItemDescription = fiscalItem.ItemDescription.Substring(0, size);
                        stringList.Add("Iteam iterated is: " + fiscalItem.ItemDescription);
                        if (isNormalSale)
                            fiscalprinter.PrintRecItem(fiscalItem.ItemDescription, (decimal)total, (int)qty, (int)vat, (decimal)ppu, fiscalItem.ItemDescription);
                        else
                            fiscalprinter.PrintRecRefund(fiscalItem.ItemDescription, (decimal)(Math.Abs(total)), (int)vat);
                    }
                    stringList.Add("After iterating item total loop ");
                    pointsPurchased = Convert.ToDecimal(PointPurchased) * multiplier;
                    tabCredit = (Convert.ToDecimal(TabCredit)) * multiplier;

                    if (pointsPurchased > 0)
                        fiscalprinter.PrintRecItem("Purchased Points", (decimal)pointsPurchased, (int)1000, (int)1, (decimal)pointsPurchased, "Points");
                    else if (pointsPurchased < 0)
                        fiscalprinter.PrintRecRefund("Refund Points", (decimal)(Math.Abs(pointsPurchased)), (int)10);


                    if (tabCredit > 0)
                        fiscalprinter.PrintRecRefund("Tab Credit Refund", (decimal)(Math.Abs(tabCredit)), (int)10);
                    else if (tabCredit < 0)
                    {
                        tabCredit = Math.Abs(tabCredit);
                        fiscalprinter.PrintRecItem("Tab Credit ", (decimal)pointsPurchased, (int)1000, (int)1, (decimal)(tabCredit), "Credit");
                    }

                    stringList.Add("Before iterating PaymentList total  loop ");
                    foreach (FiscalPayment paymentInfo in PaymentList)
                    {
                        decimal total = (Convert.ToDecimal(paymentInfo.Amount) * multiplier);
                        stringList.Add("total amount for payment type: "+ paymentInfo.Description + " " + total);
                        totalPaymentcollected += total;
                        total = (Convert.ToDecimal(paymentInfo.PaymentSurcharge) * multiplier);
                        totalPaymentSurcharge += total;
                        total = (Convert.ToDecimal(paymentInfo.TipAmount) * multiplier);
                        totalTipAmount += total;
                        total = (Convert.ToDecimal(paymentInfo.ChangeAmount) * multiplier);
                        totalChange += total;
                    }
                    //if (!isNormalSale)
                    //    totalPaymentcollected = Math.Abs(totalPaymentcollected);


                    stringList.Add("Before iterating DiscountList total loop ");
                    foreach (DiscountDetails discountInfo in DiscountList)
                    {
                        decimal total = (Convert.ToDecimal(discountInfo.Amount) * multiplier);

                        if (total > 0)
                        {
                            totalSurcharge += total;
                        }
                        else
                        {
                            totalDiscount += total;
                        }
                    }
                    stringList.Add("tab credit is " + tabCredit);
                    decimal rounding = (totalPaymentcollected) - (totalItemprice + totalChange + totalTipAmount + totalPaymentSurcharge + totalDiscount + totalSurcharge + pointsPurchased + tabCredit);
                    stringList.Add("total rounding value is " + rounding);
                    stringList.Add("total totalItemprice value is " + totalItemprice);
                    stringList.Add("total totalChange value is " + totalChange);
                    stringList.Add("total totalTipAmount value is " + totalTipAmount);
                    stringList.Add("total totalPaymentSurcharge value is " + totalPaymentSurcharge);
                    stringList.Add("total totalDiscount value is " + totalDiscount);
                    stringList.Add("total totalSurcharge value is " + totalSurcharge);
                    stringList.Add("total totalPaymentcollected value is " + totalPaymentcollected);

                    if (totalDiscount != 0 || totalSurcharge != 0)
                    {
                        stringList.Add(" Inside discount info block  " + DiscountList.Count);

                        foreach (DiscountDetails discountInfo in DiscountList)
                        {
                            decimal total = (Convert.ToDecimal(discountInfo.Amount) * multiplier);
                            // fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountDiscount, discountInfo.Description,  (decimal)15500);//(Math.Abs(total))
                            stringList.Add("Inside loop " + discountInfo.Description);

                            if (total > 0)
                            {
                                if (!isNormalSale)
                                    fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountDiscount, discountInfo.Description, (decimal)(Math.Abs(total)));
                                else
                                    fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, discountInfo.Description, (decimal)total);
                            }
                            else
                            {
                                if (isNormalSale)
                                    fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountDiscount, discountInfo.Description, (decimal)(Math.Abs(total)));
                                else
                                    fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, discountInfo.Description, (decimal)(Math.Abs(total)));
                            }
                        }
                        stringList.Add(" After discount info block  " + DiscountList.Count);
                    }

                    stringList.Add(" Before tip etc section  ");
                    if (totalTipAmount != 0)
                        fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Tip Amount", (decimal)(totalTipAmount));
                    if (totalChange != 0)
                        fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Total Change", (decimal)(totalChange));
                    if (totalPaymentSurcharge != 0)
                        fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Payment Surcharge", (decimal)(totalPaymentSurcharge));
                    stringList.Add("rounding  is " + rounding);
                    if (rounding > 0)
                    {
                        if (isNormalSale)
                            fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Rounding ", (decimal)(Math.Abs(rounding))); //fiscalprinter.PrintRecNotPaid("Rounding +ve", (decimal)rounding);
                        else
                            fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountDiscount, "Rounding ", (decimal)(Math.Abs(rounding)));
                    }
                    else if (rounding < 0)
                    {
                        if (isNormalSale)
                            fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountDiscount, "Rounding ", (decimal)(Math.Abs(rounding)));
                        else
                            fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Rounding ", (decimal)(Math.Abs(rounding)));
                    }

                    //fiscalprinter.PrintRecSubtotalAdjustment(FiscalAdjustment.AmountSurcharge, "Rounding -ve", (decimal)(Math.Abs(rounding)));
                    stringList.Add("rounding  is " + rounding);
                    stringList.Add("total payment collected  is " + totalPaymentcollected);

                    stringList.Add(" Before payment section iteration  ");
                    foreach (FiscalPayment paymentInfo in PaymentList)
                    {
                        decimal total = (Convert.ToDecimal(paymentInfo.Amount) * multiplier);
                        bool isTipAppliedByPOS = Convert.ToBoolean(Convert.ToInt32(paymentInfo.IsTipAppliedFromPOS));

                        if (!isNormalSale)
                            total = Math.Abs(total);

                        if (!isTipAppliedByPOS)
                            fiscalprinter.PrintRecTotal((decimal)total, (decimal)total, paymentInfo.Description);
                    }

                    //for total amount of bill is zero.
                    if (PaymentList.Count == 0 && totalPaymentcollected == 0 && ItemList.Count > 0)
                        fiscalprinter.PrintRecTotal((decimal)0, (decimal)0, "Cash");

                    stringList.Add(" After payment section iteration  ");

                    fiscalprinter.EndFiscalReceipt(false);
                    stringList.Add("EndFiscalReceipt ");

                    if (OpenCD)
                    {
                        stringList.Add("Inside open cashdrawer check ");
                        OpenCashDrawer();
                        stringList.Add("After opening cashdrawer fundction. ");
                    }
                    stringList.Add("End of print receipt function. ");
                    WriteToFile(stringList);
                    stringList.Clear();
                }
                catch (Exception Ex)
                {
                    //stringList.Clear();                    
                    stringList.Add("catch block of First Try block" + Ex.Message);
                    stringList.Add(DateTime.Now.ToString("hh:mm:ss tt"));
                    stringList.Add("printResponseString is: " + printResponseString);
                    WriteToFile(stringList);
                    stringList.Clear();                    
                    //fiscalprinter.ResetPrinter();
                    fiscalprinter.Close();
                    throw;
                }
                stringList.Add("After first try completion " + DateTime.Now.ToString("hh:mm:ss tt"));
                WriteToFile(stringList);
                stringList.Clear();
                fiscalprinter.Close();
                stringList.Add("printer closed in print receipt ");
                printResponseString = "OK";
                stringList.Add("printResponseString: " + printResponseString);
                WriteToFile(stringList);
                stringList.Clear();                
            }
            catch (Exception Ex)
            {
                printResponseString = Ex.Message;
                stringList.Add("catch block of second Try block" + Ex.Message);
                stringList.Add(DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add(Ex.Message);
                WriteToFile(stringList);
                stringList.Clear();
                //fiscalprinter.Close();
            }
            return printResponseString;
        }

        public string PrintZReport()
        {
            string ZReportState = "";
            try
            {
                if (!CheckPortOpenAndRetry())
                { return "Printer is not responding or might be busy in PrintZReport Method"; }
             

                fiscalprinter.ResetPrinter();
                stringList.Add("After ResetPrinter in ZED ");
                fiscalprinter.PrintZReport();
                stringList.Add("After PrintZReport in ZED ");
                fiscalprinter.Close();
                stringList.Add("After fiscalprinter.Close() in ZED " + DateTime.Now.ToString("hh:mm:ss tt"));
                ZReportState = "OK";

                WriteToFile(stringList);
                stringList.Clear();
            }
            catch (Exception Ex)
            {
                ZReportState = Ex.Message;
                stringList.Add(DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception in ZED " + ZReportState);
                WriteToFile(stringList);
                stringList.Clear();
                fiscalprinter.Close();
            }
            return ZReportState;
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                                         System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Fiscal Printer Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "Fiscal Logs " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
                string fileName = Path.Combine(location, name2);

                if (fileName.Contains(@"file:\"))
                {
                    fileName = fileName.Replace(@"file:\", "");
                }
                if (!File.Exists(fileName))
                {

                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
                else
                {
                    using (var sw = File.AppendText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {

            }
        }

        public string CheckPrinterAvailable()
        {
            string printerState = "";
         
            try
            {
                if (!CheckPortOpenAndRetry())
                { return "Printer is not responding or might be busy in CheckPrinterAvailable Method"; }
                else { printerState = "OK"; }
                fiscalprinter.Close();
            }
            catch (Exception Ex)
            {
                stringList.Add("Exception in CheckPrinterAvailable() " + DateTime.Now.ToString("hh:mm:ss tt"));
                printerState = Ex.Message + " :Printer Not Available";
                stringList.Add(printerState);
                WriteToFile(stringList);
                stringList.Clear();
                //fiscalprinter.ResetPrinter();
                //fiscalprinter.Close();
            }
            return printerState;
        }
        private void co_OnStatusUpdateEvent(object source, StatusUpdateEventArgs d)
        {
            try
            {
                Console.WriteLine(d.ToString());

                string text = "unknown";
                switch (d.Status)
                {
                    case Microsoft.PointOfService.FiscalPrinter.StatusCoverOK:
                        text = "Cover OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusCoverOpen:
                        text = "Cover Open";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusIdle:
                        text = "Idle";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusJournalCoverOK:
                        text = "Journal Cover OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusJournalCoverOpen:
                        text = "Journal Cover Open";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusJournalEmpty:
                        text = "Journal Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusJournalNearEmpty:
                        text = "Journal Near Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusJournalPaperOK:
                        text = "Journal Paper OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusPowerOff:
                        text = "Power Off";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusPowerOffline:
                        text = "Power Offline";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusPowerOffOffline:
                        text = "Power Off Offline";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusPowerOnline:
                        text = "Power Online";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusReceiptCoverOK:
                        text = "Receipt Cover OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusReceiptCoverOpen:
                        text = "Receipt Cover Open";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusReceiptEmpty:
                        text = "Receipt Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusReceiptNearEmpty:
                        text = "Receipt Near Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusReceiptPaperOK:
                        text = "Receipt Paper OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusSlipCoverOK:
                        text = "Slip Cover OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusSlipCoverOpen:
                        text = "Slip Cover Open";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusSlipEmpty:
                        text = "Slip Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusSlipNearEmpty:
                        text = "Slip Near Empty";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusSlipPaperOK:
                        text = "Slip Paper OK";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareComplete:
                        text = "Firmware Complete";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareCompleteDeviceNotRestored:
                        text = "Firmware Complete Device Not Restored";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareFailedDeviceNeedsFirmware:
                        text = "Firmware Failed Device Needs Firmware";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareFailedDeviceOk:
                        text = "Firmware Failed Device Ok";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareFailedDeviceUnknown:
                        text = "Firmware Failed Device Unknown";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareFailedDeviceUnrecoverable:
                        text = "Firmware Failed Device Unrecoverable";
                        break;
                    case Microsoft.PointOfService.FiscalPrinter.StatusUpdateFirmwareProgress:
                        text = "Firmware Progress";
                        break;

                    default:
                        text = "Unexpected status code: " + d.Status.ToString();
                        break;
                }

                Console.WriteLine("Status: " + text);
            }
            catch (Exception ae)
            {
                Console.WriteLine("Exception: " + ae);
            }
        }

        public string OpenCashDrawer()
        {
            string CashDrawerResponse = "";
            try
            {
                stringList.Add("inside OpenCashDrawer() " + DateTime.Now.ToString("hh:mm:ss tt"));
                // Initializing PosExplorer
                posExplorer = new PosExplorer();
                DeviceInfo cd = posExplorer.GetDevice("CashDrawer", "EpsonCD1");
                stringList.Add("After Getting cashdrawer device in  OpenCashDrawer() ");
                PosCommon posCommonCD = (PosCommon)posExplorer.CreateInstance(cd);
                stringList.Add("After creating instance  in  OpenCashDrawer() ");
                CashDrawer cashdrawer = (CashDrawer)posCommonCD;
                stringList.Add("After initializing cashdrawer in  OpenCashDrawer() ");
                cashdrawer.Open();
                stringList.Add("Cash Drawer opened  OpenCashDrawer() ");
                cashdrawer.Claim(1000);
                stringList.Add("Cash Drawer claimed in   OpenCashDrawer() ");
                cashdrawer.DeviceEnabled = true;
                stringList.Add("setting device enabled in OpenCashDrawer() ");
                stringList.Add("cash drawer state :  " + cashdrawer.State);
                cashdrawer.OpenDrawer();
                stringList.Add("Performing OpenDrawer() method ");
                cashdrawer.Close();
                stringList.Add("Performing Close() method ");
                stringList.Add("open cashdrawer method execution ends.");
            }
            catch (Exception Ex)
            {
                stringList.Add("Exception in  OpenCashDrawer() method. " + DateTime.Now.ToString("hh:mm:ss tt"));
                CashDrawerResponse = Ex.Message;
                stringList.Add("Exception is  " + CashDrawerResponse);
                WriteToFile(stringList);
                stringList.Clear();
            }
            return CashDrawerResponse;
        }
    }
}
