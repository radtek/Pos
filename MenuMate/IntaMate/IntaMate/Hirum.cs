using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
//using System.IO.Ports;
using System.Diagnostics;
using IntaMate.Properties;
using IntaMate;
using System.Xml.Serialization;
using System.IO;
using NLog;
//using System.Net.Sockets;

namespace IntaMate
{
    class PMS_Hirum : IntaMate.PMS
    {
        public bool Active = false;

        public PMS_Hirum(e_IOType IO_Type) : base(IO_Type)
        {
            
        }

        public override bool Connect()
        {
            try
            {
                Active = GetTransport().Connected();
            }
            catch (System.Exception ex)
            {
                logger.Error("Interface Error : " + ex.ToString());
                Active = false;
            }
            return Active;
        }

        public override void SaveConfig()
        {
        }

        public override bool IsConnected()
        {
            bool RetVal = false;
            try
            {
                GetTransport().DiscardInBuffer();
                GetTransport().Write(new byte[] { ProtocolUtility.ENQ }, 0, 1);
                byte Ack = ProtocolUtility.NAK;
                Ack = (byte)GetTransport().ReadByte();
                if (Ack == ProtocolUtility.ACK)
                {
                    logger.Info("\r Online: Ack Received");
                    RetVal = true;
                }
                else if (Ack == ProtocolUtility.NAK)
                {
                    logger.Info("\r OffLine: Nak Received");
                }
                else
                {
                    logger.Info("\r OffLine: Unexpected Response");
                }
            }
            catch (System.TimeoutException ex)
            {
                logger.Error("\r OffLine: No Reply " + ex.Message);
            }
            catch (System.Exception ex)
            {
                logger.Error("\r OffLine: " + ex.Message);
            }
            return RetVal;
        }

        protected class ProductPortion
        {
            public int _code;
            public int _amount;
            public int _originalAmount;

            public Dictionary<int, int> PaymentPortion = new Dictionary<int, int>();

            public ProductPortion(int inCode, int inOriginalAmount)
            {
                _code = inCode;
                _amount = 0;
                _originalAmount = inOriginalAmount;
            }

            public void PartialPayment(int inPayCode, int inPayAmount)
            {
                _amount -= inPayAmount;
                PaymentPortion.Add(inPayCode, inPayAmount);
            }

            public override bool Equals(object obj)
            {
                return _code.Equals(((ProductPortion)obj)._code);
            }

            public override int GetHashCode()
            {
                return _code.GetHashCode();
            }

            public int getAmountPaid()
            {
                int retVal = 0;
                foreach (KeyValuePair<int, int> KeyValue in PaymentPortion)
                {
                    retVal += KeyValue.Value;
                }
                return retVal;
            }

            internal int getGrandTotal()
            {
                return _amount + getAmountPaid();
            }

            internal bool getFulyPaidItem()
            {
                return _amount == _originalAmount;
            }
        }

        protected override List<PMSMessage> BuildTransmissionArray()
        {
            List<PMSMessage> MsgTransactions = new List<PMSMessage>();

            //loop rough Data.Catagories.
            Dictionary<int, int> Payments = new Dictionary<int, int>();
            Dictionary<int, int> Products = new Dictionary<int, int>();
            // Product Key , < Payment Key , Amount >.
            foreach (KeyValuePair<int, int> KeyValue in _data.Catagories)
            {
                // All Cats under 100 are payments.
                if (KeyValue.Key < 100)
                {
                    Payments[KeyValue.Key] = KeyValue.Value;
                }
                else
                { // All Cats over 100 are products.
                    Products[KeyValue.Key] = KeyValue.Value;
                }
            }

            List<ProductPortion> Transactions = new List<ProductPortion>();

            IEnumerator<KeyValuePair<int, int>> currentPayment = Payments.GetEnumerator();
            //ProductPortion CurrentProduct = new ProductPortion(Product.Key, Product.Value);
            int paymentUsed = 0;
            int productUsed = 0;
            if (Products.Count != 0)
            {
                if (currentPayment.MoveNext())
                {
                    foreach (KeyValuePair<int, int> currentProduct in Products)
                    {
                        bool productFinished = false;
                        ProductPortion productPortion = new ProductPortion(currentProduct.Key, currentProduct.Value);
                        Transactions.Add(productPortion);

                        while (!productFinished)
                        {
                            // Payments are -ve Amounts, Products +ve Amounts.
                            int productRemaining = currentProduct.Value - productUsed;
                            int paymentRemaining = currentPayment.Current.Value + paymentUsed;

                            // Payments are -ve Amounts, Products +ve Amounts.
                            if (Math.Abs(paymentRemaining) >= Math.Abs(productRemaining))
                            {
                                paymentUsed += productRemaining;
                                // We are done with this product so Store it off as all paid for.
                                // Payment is partially used up.
                                productPortion.PartialPayment(currentPayment.Current.Key, -productRemaining);
                                productFinished = true;
                                productUsed = 0;
                            }
                            else
                            {
                                // Payments are -ve Amounts
                                productUsed -= paymentRemaining;
                                paymentUsed -= paymentRemaining;
                                // We are done with this Payment so Store it off as Item Partially paid for.
                                // Payment is all used up.
                                productPortion.PartialPayment(currentPayment.Current.Key, paymentRemaining);
                            }

                            if (-paymentUsed == currentPayment.Current.Value)
                            {
                                if (!currentPayment.MoveNext())
                                { // We have run out of payments to cover this product.
                                    if (!productPortion.getFulyPaidItem()) // Is it all paid for?
                                    {
                                        //Can we charge it to a room?
                                        if (_data.RoomNumber != "0")
                                        {
                                            ProductPortion leftOverProductPortion = new ProductPortion(currentProduct.Key, currentProduct.Value);
                                            // Generate a payment for whats left.
                                            leftOverProductPortion.PartialPayment(17, -(currentProduct.Value - paymentUsed));
                                            Transactions.Add(leftOverProductPortion);
                                            productFinished = true;
                                            productUsed = 0;
                                        }
                                        else
                                        {
                                            logger.Warn((currentProduct.Value - productUsed).ToString() + " of " + currentProduct.Key + " Not accounted for!");
                                            productFinished = true;
                                            productUsed = 0;
                                        }
                                    }
                                }
                                paymentUsed = 0;
                            }
                        }
                    }
                }
                else
                { // Pure room charge with no other payments.
                    if (_data.RoomNumber != "0")
                    {
                        foreach (KeyValuePair<int, int> currentProduct in Products)
                        {
                            ProductPortion productPortion = new ProductPortion(currentProduct.Key, currentProduct.Value);
                            productPortion.PartialPayment(17, -currentProduct.Value);
                            Transactions.Add(productPortion);
                        }
                    }
                    else
                    { // No payment or room, Product Code test.
                        foreach (KeyValuePair<int, int> currentProduct in Products)
                        {
                            PMSMessageFidelioAccCharge AccountCharge = new PMSMessageFidelioAccCharge();
                            AccountCharge.ImportStaticData(_data);
                            AccountCharge.RevenueCenter = currentProduct.Key;
                            AccountCharge.ProductTotal = currentProduct.Value;
                            AccountCharge.PostingTotal = currentProduct.Value;
                            AccountCharge.SetPaymentCode(0, currentProduct.Value);
                            MsgTransactions.Add(AccountCharge);
                        }
                    }
                }                
            }
            else
            { // No product Payment only Code test or Eftpos Cash Out.
                PMSMessageFidelioAccCharge AccountCharge = new PMSMessageFidelioAccCharge();
                AccountCharge.ImportStaticData(_data);
                AccountCharge.RevenueCenter = 0;
                AccountCharge.ProductTotal = 0;
                int PostingTotal = 0;
                while (currentPayment.MoveNext())
                {
                    PostingTotal += currentPayment.Current.Value;                    
                    AccountCharge.SetPaymentCode(currentPayment.Current.Key, currentPayment.Current.Value);
                }
                AccountCharge.PostingTotal = PostingTotal; 
                MsgTransactions.Add(AccountCharge);
            }
        
            IEnumerator<ProductPortion> emessage = Transactions.GetEnumerator();
            while (emessage.MoveNext())
            {
                if (emessage.Current.PaymentPortion.Count != 0)
                {
                    PMSMessageFidelioAccCharge AccountCharge = new PMSMessageFidelioAccCharge();
                    AccountCharge.ImportStaticData(_data);
                    AccountCharge.RevenueCenter = emessage.Current._code;
                    AccountCharge.ProductTotal = emessage.Current._amount;
                    AccountCharge.PostingTotal = emessage.Current.getGrandTotal();
                    foreach (KeyValuePair<int, int> Payment in emessage.Current.PaymentPortion)
                    {
                        AccountCharge.SetPaymentCode(Payment.Key, Payment.Value);
                    }
                    MsgTransactions.Add(AccountCharge);
                }
                else if (_data.RoomNumber != "0")
                {
                    PMSMessageFidelioAccCharge AccountCharge = new PMSMessageFidelioAccCharge();
                    AccountCharge.ImportStaticData(_data);
                    AccountCharge.PaymentMethod = "17";
                    AccountCharge.RevenueCenter = emessage.Current._code;
                    AccountCharge.ProductTotal = emessage.Current._amount;
                    AccountCharge.PostingTotal = emessage.Current.getGrandTotal();
                    MsgTransactions.Add(AccountCharge);
                }
                else
                {
                    int Dollars = emessage.Current._amount / 100;
                    int Cents = emessage.Current._amount % 100;
                    String subTotal = "";
                    if (Cents < 0 && Dollars == 0)
                    { // A negitive Value of Cents only.
                        subTotal = String.Format("${0}.{1}",
                                        "-0".PadLeft(6, ' '),
                                        Math.Abs(Cents).ToString().PadLeft(2, '0'));
                    }
                    else
                    { // The sign is reflected in the dollars amount.
                        subTotal = String.Format("${0}.{1}",
                                        Dollars.ToString().PadLeft(6, ' '),
                                        Math.Abs(Cents).ToString().PadLeft(2, '0'));
                    }
                    logger.Warn(subTotal + " of " + emessage.Current._code + " Not accounted for!");
                }
            }

            return MsgTransactions;
        }

        protected override PMSMessage BuildInquiry()
        {
            //loop rough Data.Catagories.
            PMSMessageFidelioInquiry Inquiry = new PMSMessageFidelioInquiry();
            Inquiry.ImportStaticData(_data);
            return Inquiry;
        }

        protected override void Trasmit(PMSMessage message)
        {
            byte[] ByteArray = message.ToByteArray();
            GetTransport().DiscardInBuffer();
            GetTransport().Write(ByteArray, 0, ByteArray.Length);
            logger.Debug(Encoding.ASCII.GetString(ByteArray));
        }

        // TODO the Receive Function can take a paramiter of the Message that was transmitted and using this
        // information have the message pull in its own responce and decode it.
        // This would allow for a single Receive method at this level.
        protected override void ReceiveAccountChargeResponce()
        {
            // Read in Response.
            List<byte> Buffer = new List<byte>();

            int Count = 0;
            int MaxBytes = 44;
            byte[] Dat = new byte[MaxBytes];
            Count = GetTransport().Read(Dat, 0, MaxBytes);
            if (Count != 0)
            {
                Buffer.AddRange(Dat);
            }
            PostingResponse(Buffer);
        }

        protected override void ReceiveInquiry()
        {
            List<byte> Buffer = new List<byte>();

            int Count = 0;
            int MaxBytes = 172;
            byte[] Dat = new byte[MaxBytes];
            Count = GetTransport().Read(Dat, 0, MaxBytes);
            if (Count != 0)
            {
                Buffer.AddRange(Dat);
            }

            InquiryResponse(Buffer);
        }

        protected override bool CheckResponce()
        {
            if (_data.PostingResponse == "ACK")
            {
                return true;
            }
            return false;
        }

        // Room Charge is now a base class method that uses these functions to 
        // build up the correct packet type.
        /*        public override PMSRoomChargeResponse RoomCharge(PhoenixPacketChargeToRoom ChargeToRoom)
                {
                    Encoding ascii = Encoding.ASCII;
                    PMSRoomChargeResponse Response = new PMSRoomChargeResponse();
                    try
                    {
                        _data.Import(ChargeToRoom);
                        //Check Cache
                        if (!_cache.ContainsKey(_data.RoomNumber))
                        {
                            // We need to get the Foilio Name from the Micros system based on index.
                            PMSRoomStatus ExRoomStatus = _data.ExportPMSRoomStatus();
                            PMSRoomStatusResponse ExRoomStatusResponse = RoomInquiry(ExRoomStatus);
                            _data.Import(ChargeToRoom);
                        }

                        byte[] ByteArray = Posting();
                        GetTransport().Write(ByteArray, 0, ByteArray.Length);
                        Debug.WriteLine(ascii.GetString(ByteArray));
                        // Read in Response.
                        List<byte> Buffer = new List<byte>();
                        Buffer.Clear();

                        int Count = 0;
                        int MaxBytes = 44;
                        byte[] Dat = new byte[MaxBytes];
                        Count = GetTransport().Read(Dat, 0, MaxBytes);
                        if (Count != 0)
                        {
                            Buffer.AddRange(Dat);
                        }

                        PostingResponse(Buffer);

                        Response = _data.ExportPMSRoomChargeResponse();
                    }
                    catch (System.TimeoutException ex)
                    {
                        Debug.WriteLine("\rMicrosServer : No Reply :" + ex.Message);
                        Response.ErrorResponse = ex.Message;
                    }
                    catch (System.Exception ex)
                    {
                        Debug.WriteLine("\rMicrosServer : " + ex.Message);
                        Response.ErrorResponse = ex.Message;
                    }
                    return Response;
                }*/

        protected override void PostingResponse(List<byte> Buffer)
        {
            if (Buffer.Count == 0)
            {
                logger.Trace("No Response from Hotel System. Please try again.");
                throw new Exception("No Response from Hotel System. Please try again.");
            }

            if (Buffer.IndexOf(ProtocolUtility.SOT) < 0 || Buffer.IndexOf(ProtocolUtility.STX) < 0
               || Buffer.IndexOf(ProtocolUtility.ETX) < 0 || Buffer.IndexOf(ProtocolUtility.EOT) < 0)
            {
                logger.Trace("Invalid Message");
                throw new Exception("Invalid Message");
            }

            //            MSGHeader = new String(inputBuffer,1,Array.FindIndex(inputBuffer,1,MatchSTX));
            Encoding ascii = Encoding.ASCII;

            // Check the BCC           
            String BCC1 = _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.IndexOf(ProtocolUtility.ETX)).ToArray()).PadLeft(4, '0');
            String BCC2 = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.ETX) + 1, 4);
            if (BCC1 != BCC2)
            {
                logger.Trace("BCC Error " + String.Format("{0} != {1}", BCC1, BCC2));
                throw new Exception("BCC Error");
            }

            String station = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 1, 4);
            String establishment = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 5, Buffer.IndexOf(ProtocolUtility.STX) - (Buffer.IndexOf(ProtocolUtility.SOT) + 5));

            if (Buffer[22] == '/')
            {
                _data.PostingResponse = ascii.GetString(Buffer.ToArray(), 23, 15);
                logger.Warn("Posting Responce : " + _data.PostingResponse);
            }
            else
            {
                _data.PostingResponse = "ACK";
            }
            logger.Debug("Posting Responce : " + _data.PostingResponse);
        }

        protected override void InquiryResponse(List<byte> Buffer)
        {
            logger.Debug("Inquiry Response : " + Buffer);
            if (Buffer.Count == 0)
            {
                logger.Trace("No Response from Hotel System. Please try again.");
                throw new Exception("No Response from Hotel System. Please try again.");
            }

            if (Buffer.IndexOf(ProtocolUtility.SOT) < 0 || Buffer.IndexOf(ProtocolUtility.STX) < 0
                           || Buffer.IndexOf(ProtocolUtility.ETX) < 0 || Buffer.IndexOf(ProtocolUtility.EOT) < 0)
            {
                logger.Trace("Invalid Message");
                throw new Exception("Invalid Message");
            }

            //            MSGHeader = new String(inputBuffer,1,Array.FindIndex(inputBuffer,1,MatchSTX));
            Encoding ascii = Encoding.ASCII;

            // CHeck the BCC
            String BCC1 = _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.IndexOf(ProtocolUtility.ETX)).ToArray()).PadLeft(4, '0');
            String BCC2 = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.ETX) + 1, 4);
            if (BCC1 != BCC2)
            {
                logger.Trace("BCC Error " + String.Format("{0} != {1}", BCC1, BCC2));
                throw new Exception("BCC Error");
            }

            String station = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 1, 4);
            String establishment = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 5, Buffer.IndexOf(ProtocolUtility.STX) - (Buffer.IndexOf(ProtocolUtility.SOT) + 5));

            List<String> Foilos = new List<String>();
            _data.ErrorResponse = "";
            if (Buffer[22] == ' ')
            {
                _data.ErrorResponse = ascii.GetString(Buffer.ToArray(), 23, 15);
            }
            else
            {
                int Index = 22;
                do
                {
                    Foilos.Add(ascii.GetString(Buffer.ToArray(), Index, 16).Replace(',', ' '));
                    Index += 16;
                }
                while (Buffer[Index] != ProtocolUtility.ETX);
            }
            _cache[_data.RoomNumber] = Foilos;
        }

        /*protected override byte[] Inquiry()
        {
            Encoding ascii = Encoding.ASCII;
            List<byte> Buffer = new List<byte>();
            Buffer.Add(ProtocolUtility.SOT);

            if (_data.RegisterNumber.Length > STATION_LENGTH)
            {
                Buffer.AddRange(ascii.GetBytes(_data.RegisterNumber.Substring(0, STATION_LENGTH)));
            }
            else
            {
                Buffer.AddRange(ascii.GetBytes(_data.RegisterNumber.PadLeft(STATION_LENGTH, '0')));
            }

            if (_data.Establishment.Length > ESTABISHMENT_LENGTH)
            {
                Buffer.AddRange(ascii.GetBytes(_data.Establishment.Substring(0, ESTABISHMENT_LENGTH)));
            }
            else
            {
                Buffer.AddRange(ascii.GetBytes(_data.Establishment.PadRight(ESTABISHMENT_LENGTH, ' ')));
            }
            Buffer.Add(ProtocolUtility.STX);
            Buffer.AddRange(ascii.GetBytes(" 1"));
            Buffer.AddRange(ascii.GetBytes(" "));
            if (_data.RoomNumber.Length > 16)
            {
                Buffer.AddRange(ascii.GetBytes(_data.RoomNumber.Substring(0, 16)));
            }
            else
            {
                Buffer.AddRange(ascii.GetBytes(_data.RoomNumber.PadRight(16, ' ')));
            }
            Buffer.AddRange(ascii.GetBytes(" "));
            //Employee Number.
            Buffer.AddRange(ascii.GetBytes("".PadRight(3, ' ')));
            Buffer.Add(ProtocolUtility.ETX);
            Buffer.AddRange(ascii.GetBytes(_protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0')));
            Buffer.Add(ProtocolUtility.EOT);
            return Buffer.ToArray();
        }*/
    }
}
