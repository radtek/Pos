using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
//using System.Threading.Tasks;

namespace FiscalLibraries
{
    [Guid("f04df0aa-c1d5-4f9e-95c8-61e6474899e2")]
    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    public interface IFiscalLibrary
    {
        [DispId(1)]
        string InvoiceNumber { get; set; }
        [DispId(2)]
        string Date { get; set; }
        [DispId(3)]
        string Time { get; set; }
        [DispId(4)]
        string Billno { get; set; }
        [DispId(5)]
        string Cashno { get; set; }
        [DispId(6)]
        string Cashier { get; set; }
        [DispId(7)]
        string Source { get; set; }
        [DispId(8)]
        string PrintReceipt();
        [DispId(9)]
        void LoadReceiptItemInfo(int identifier, string invoiceNumber);
        [DispId(10)]
        void LoadReceiptPaymentInfo(int identifier, string invoiceNumber);
        [DispId(11)]
        void LoadReceiptDiscountInfo(int identifier, string invoiceNumber);
        [DispId(12)]
        void InitializeItemProperties();
        [DispId(13)]
        void InitializePaymentProperties();
        [DispId(14)]
        void InitializeDiscountProperties();
        [DispId(15)]
        void AddItemToList();
        [DispId(16)]
        void AddPaymentInfoToList();
        [DispId(17)]
        void AddDiscountInfoToList();
        [DispId(18)]
        string PrintZReport();
        [DispId(19)]
        string TerminalName { get; set; }
        [DispId(20)]
        string PointPurchased { get; set; }
        [DispId(21)]
        string TabCredit { get; set; }
        [DispId(22)]
        string PrinterType { get; set; }
        [DispId(23)]
        string PrinterLogicalName { get; set; }
        [DispId(24)]
        string Saletype { get; set; }
        [DispId(25)]
        string CheckPrinterAvailable();
        [DispId(26)]
        string OpenCashDrawer();
        [DispId(27)]
        bool OpenCD { get; set; }

    }

}
