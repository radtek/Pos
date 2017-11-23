using Newtonsoft.Json;
using SiHotIntegration.Domain;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace SiHotIntegration.Utility
{
    public class SiHotSerializer
    {
        private string groupSeparatorHex = "1D";
        private string fieldSeparatorHex = "1C"; 
        
        public SiHotSerializer()
        {
 
        }

        public List<string> GetRoomRequestContent(RoomRequest roomRequest)
        {
            List<string> detailsList = new List<string>();
            detailsList.Add("transno:" + roomRequest.TransactionNumber);
            detailsList.Add("roomreq:" + roomRequest.RoomNumber);
            return detailsList;
        }

        public List<byte> GetValidateContent(int transno)
        {
            byte groupSeparator = 29;
            List<byte> bytesList = new List<byte>();
            string content = "transno:"+transno;
            bytesList.AddRange(Encoding.UTF8.GetBytes(content).ToList<byte>());
            bytesList.Add(groupSeparator);
            content = "paymenttypes";
            bytesList.AddRange(Encoding.UTF8.GetBytes(content).ToList<byte>());
            bytesList.Add(groupSeparator);
            return bytesList; 
        }
        public List<byte> GetRoomChargeContent(RoomChargeDetails roomDetails)
        {
            byte fileSeparator = 28;
            byte groupSeparator = 29;
            List<byte> bytesList = new List<byte>();
            bytesList.AddRange(Encoding.UTF8.GetBytes("transno:"+roomDetails.TransNo).ToList<byte>());
            bytesList.Add(groupSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("accountbook:" + roomDetails.AccountNumber).ToList<byte>());
            bytesList.Add(groupSeparator);
            List<string> servicesList = new List<string>();
            for (int i = 0; i < roomDetails.ItemList.Count; i++)
            {
                bytesList.AddRange(Encoding.UTF8.GetBytes("block:service").ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("supercat:" + roomDetails.ItemList[i].SuperCategory).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("supercat_desc:" + roomDetails.ItemList[i].SuperCategory_Desc).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("middlecat:" + roomDetails.ItemList[i].MiddleCategory).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("middlecat_desc:" + roomDetails.ItemList[i].MiddleCategory_Desc).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("articlecat:" + roomDetails.ItemList[i].ArticleCategory).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("articlecat_desc:" + roomDetails.ItemList[i].ArticleCategory_Desc).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("articleno:" + roomDetails.ItemList[i].ArticleNo).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("articleno_desc:" + roomDetails.ItemList[i].ArticleNo_Desc).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("amount:" + roomDetails.ItemList[i].Amount.ToString()).ToList<byte>());
                //bytesList.AddRange(Encoding.UTF8.GetBytes("amount:" + roomDetails.ItemList[i].Amount.ToString().Replace(".", ",")).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("priceperunit:" + Math.Round(Convert.ToDecimal(roomDetails.ItemList[i].PricePerUnit),2).ToString()).ToList<byte>());
                //bytesList.AddRange(Encoding.UTF8.GetBytes("priceperunit:" + roomDetails.ItemList[i].PricePerUnit.ToString().Replace(".",",")).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("pricetotal:" + Math.Round(Convert.ToDecimal(roomDetails.ItemList[i].PriceTotal),2).ToString()).ToList<byte>());
                //bytesList.AddRange(Encoding.UTF8.GetBytes("pricetotal:" + roomDetails.ItemList[i].PriceTotal.ToString().Replace(".", ",")).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("billno:" + roomDetails.ItemList[i].Billno).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("vatpercentage:" + roomDetails.ItemList[i].VATPercentage.ToString()).ToList<byte>());
                //bytesList.AddRange(Encoding.UTF8.GetBytes("vatpercentage:" + roomDetails.ItemList[i].VATPercentage.ToString().Replace(".", ",")).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("cashno:" + roomDetails.ItemList[i].Cashno).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("cashier:" + roomDetails.ItemList[i].Cashier).ToList<byte>());
                if (i != roomDetails.ItemList.Count)
                    bytesList.Add(groupSeparator);
                else
                    bytesList.Add(fileSeparator);
            }

            for (int i = 0; i < roomDetails.PaymentList.Count; i++)
            {
                bytesList.AddRange(Encoding.UTF8.GetBytes("block:payment").ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("type:" + roomDetails.PaymentList[i].Type).ToList<byte>());
                bytesList.Add(fileSeparator);
                //bytesList.AddRange(Encoding.UTF8.GetBytes("amount:" + roomDetails.PaymentList[i].Amount.ToString().Replace(".", ",")).ToList<byte>());
                bytesList.AddRange(Encoding.UTF8.GetBytes("amount:" + roomDetails.PaymentList[i].Amount.ToString()).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("payment_desc:" + roomDetails.PaymentList[i].Description).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("billno:" + roomDetails.PaymentList[i].Billno).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("cashno:" + roomDetails.PaymentList[i].Cashno).ToList<byte>());
                bytesList.Add(fileSeparator);
                bytesList.AddRange(Encoding.UTF8.GetBytes("cashier:" + roomDetails.PaymentList[i].Cashier).ToList<byte>());
                if (i != roomDetails.PaymentList.Count)
                    bytesList.Add(groupSeparator);
                else
                    bytesList.Add(fileSeparator);
            }

            bytesList.AddRange(Encoding.UTF8.GetBytes("block:cover").ToList<byte>());
            bytesList.Add(groupSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("covers:").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("covertype:guest").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("linkID:").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("linkText:").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("shift:").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("date:" + DateTime.Now.ToString("yyyyMMdd")).ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("linkID:").ToList<byte>());
            bytesList.Add(fileSeparator);
            bytesList.AddRange(Encoding.UTF8.GetBytes("time" + DateTime.Now.ToString("HHmmss")).ToList<byte>());
            bytesList.Add(groupSeparator);

            return bytesList;
        }      
    }
}
