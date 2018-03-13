using System;
using Microsoft.PointOfService;
using FiscalPrinterIntegration.Domain;

namespace FiscalPrinterIntegration
{
    public class FiscalPrinter_FP81II
    {
        public PosExplorer posExplorer;
        public PosCommon posCommonFP;
        DeviceInfo fp;

        public FiscalPrinter_FP81II()
        {
            posExplorer = new PosExplorer();            
            fp = posExplorer.GetDevice("FiscalPrinter", "EpsonFP1");
            posCommonFP = (PosCommon)posExplorer.CreateInstance(fp);
        }

        public FiscalResponseDetails PrintDetails(FiscalDataDetails fiscalData)
        {
            FiscalResponseDetails response = new FiscalResponseDetails();
            try
            {                              
                posCommonFP.StatusUpdateEvent += new StatusUpdateEventHandler(co_OnStatusUpdateEvent);
                FiscalPrinter fiscalprinter = (FiscalPrinter)posCommonFP;

                fiscalprinter.Open();

                fiscalprinter.Claim(1000);
                fiscalprinter.PowerNotify = PowerNotification.Enabled;
                fiscalprinter.DeviceEnabled = true;
                fiscalprinter.ResetPrinter();

                fiscalprinter.BeginFiscalReceipt(true);
                for (int i = 0; i < 2; i++)
                {
                    fiscalprinter.PrintRecItem("test dossa", (decimal)10000, (int)1000, (int)15, (decimal)10000, "");
                }

                fiscalprinter.PrintRecItem("ITEM", (decimal)10000, (int)1000, (int)0, (decimal)10000, "");

                fiscalprinter.PrintRecItemVoid("ITEM VOID", (decimal)10000, (int)1000, (int)0, (decimal)10000, "");

                fiscalprinter.PrintRecTotal((decimal)200000, (decimal)200000, "0");

                string[] strObj = new string[1];
                strObj[0] = "010803111200002801234567890";
                DirectIOData dirIO = posCommonFP.DirectIO(0, 1075, strObj);
                int iData = dirIO.Data;
                string[] iObj = (string[])dirIO.Object;
                fiscalprinter.EndFiscalReceipt(false);
                if (fiscalprinter.CapSlpFiscalDocument)
                {
                    FiscalDataItem dati = fiscalprinter.GetData(FiscalData.FiscalDocument, 0);

                }
                fiscalprinter.Close();
            }
            catch (Exception Ex)
            {
            }
            return response;
        }

        private void co_OnStatusUpdateEvent(object source, StatusUpdateEventArgs d)
        {
            try
            {
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


            }
            catch (Exception Ex)
            {

            }
        }

        public FiscalResponseDetails PrintZReport()
        {
            FiscalResponseDetails response = new FiscalResponseDetails();
            try
            {
                posCommonFP.StatusUpdateEvent += new StatusUpdateEventHandler(co_OnStatusUpdateEvent);
                FiscalPrinter fiscalprinter = (FiscalPrinter)posCommonFP;
                fiscalprinter.Open();
                fiscalprinter.Claim(1000);
                fiscalprinter.PowerNotify = PowerNotification.Enabled;
                fiscalprinter.DeviceEnabled = true;
                fiscalprinter.ResetPrinter();
                fiscalprinter.PrintZReport();
                fiscalprinter.Close();
            }
            catch (Exception Ex)
            {

            }
            return response;
        }
    }
}
