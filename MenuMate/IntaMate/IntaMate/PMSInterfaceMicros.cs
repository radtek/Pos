using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
using System.Diagnostics;
using IntaMate.Properties;
using IntaMate;
using System.Xml.Serialization;
using System.IO;
using NLog;

namespace IntaMate
{
    class PMSInterfaceMicros : IntaMate.PMS
    {
        public bool Active = false;

        public PMSInterfaceMicros() : base()
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
            public String _code;
            public int _amount;
            public int _originalAmount;

            public Dictionary<String, int> PaymentPortion = new Dictionary<String, int>();

            public ProductPortion(String inCode, int inOriginalAmount)
            {
                _code = inCode;
                _amount = 0;
                _originalAmount = inOriginalAmount;
            }

            public void PartialPayment(String inPayCode, int inPayAmount)
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
                foreach (KeyValuePair<String, int> KeyValue in PaymentPortion)
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
            if (_data.RoomNumber == "90000")
            {
                // This Interface Ignores 90000 type transactions.
                _data.PostingResponse = "ACK";
            }
            else
            {
                //loop rough Data.Catagories.
                Dictionary<String, int> Payments = new Dictionary<String, int>();
                Dictionary<String, int> Products = new Dictionary<String, int>();
                // Product Key , < Payment Key , Amount >.
                foreach (KeyValuePair<String, int> KeyValue in _data.Catagories)
                {

                    int IntKeyValue = 0;
                    if (Int32.TryParse(KeyValue.Key,out IntKeyValue))
                    {// All Cats under 100 are payments.
                        if (IntKeyValue < 100)
                        {
                            Payments[KeyValue.Key] = KeyValue.Value;
                        }
                        else
                        { // All Cats over 100 are products.
                            Products[KeyValue.Key] = KeyValue.Value;
                        }
                    }
                    else
                    {
                        Products[KeyValue.Key] = KeyValue.Value;
                    }
                }

                List<ProductPortion> Transactions = new List<ProductPortion>();

                IEnumerator<KeyValuePair<String, int>> currentPayment = Payments.GetEnumerator();

                int paymentUsed = 0;
                int productUsed = 0;
                if (Products.Count != 0)
                {
                    if (currentPayment.MoveNext())
                    {
                        foreach (KeyValuePair<String, int> currentProduct in Products)
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
                                                leftOverProductPortion.PartialPayment("17", -(currentProduct.Value - paymentUsed));
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
                            foreach (KeyValuePair<String, int> currentProduct in Products)
                            {
                                ProductPortion productPortion = new ProductPortion(currentProduct.Key, currentProduct.Value);
                                productPortion.PartialPayment("17", -currentProduct.Value);
                                Transactions.Add(productPortion);
                            }
                        }
                        else
                        { // No payment or room, Product Code test.
                            foreach (KeyValuePair<String, int> currentProduct in Products)
                            {
                                PMSMessageAccCharge AccountCharge = GetMsg().MessageAccCharge();
                                AccountCharge.ImportStaticData(_data);
                                AccountCharge.RevenueCenter = currentProduct.Key;
                                AccountCharge.ProductTotal = currentProduct.Value;
                                AccountCharge.PostingTotal = currentProduct.Value;
                                AccountCharge.SetPaymentCode("0", currentProduct.Value);
                                MsgTransactions.Add(AccountCharge);
                            }
                        }
                    }
                }
                else
                { // No product Payment only Code test or Eftpos Cash Out.
                    PMSMessageAccCharge AccountCharge = GetMsg().MessageAccCharge();
                    AccountCharge.ImportStaticData(_data);
                    AccountCharge.RevenueCenter = "";
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

                // This is where Hirum and Mircros Differ. 
                IEnumerator<ProductPortion> emessage = Transactions.GetEnumerator();
                while (emessage.MoveNext())
                {
                    if (emessage.Current.PaymentPortion.Count != 0)
                    {
                        PMSMessageAccCharge AccountCharge = GetMsg().MessageAccCharge();
                        AccountCharge.ImportStaticData(_data);
                        AccountCharge.RevenueCenter = emessage.Current._code;
                        AccountCharge.ProductTotal = emessage.Current._amount;
                        AccountCharge.PostingTotal = emessage.Current.getGrandTotal();
                        foreach (KeyValuePair<String, int> Payment in emessage.Current.PaymentPortion)
                        {
                            AccountCharge.SetPaymentCode(Payment.Key, Payment.Value);
                        }
                        MsgTransactions.Add(AccountCharge);
                    }
                    else if (_data.RoomNumber != "0")
                    {
                        PMSMessageAccCharge AccountCharge = GetMsg().MessageAccCharge();
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
            }
            return MsgTransactions;
        }

        protected override PMSMessage BuildInquiry()
        {
            //loop rough Data.Catagories.
            PMSMessageMicrosInquiry Inquiry = new PMSMessageMicrosInquiry();
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

        protected override void PostingResponse(List<byte> Buffer)
        {
            if (GetTransport() is IO_Stub)
            {
                _data.PostingResponse = "ACK";
            }
            else
            {
                if (Buffer.Count == 0)
                {
                    logger.Debug("No Response from Hotel System. Please try again.");
                    throw new Exception("No Response from Hotel System. Please try again.");
                }

                if (Buffer.IndexOf(ProtocolUtility.SOT) < 0 || Buffer.IndexOf(ProtocolUtility.STX) < 0
                   || Buffer.IndexOf(ProtocolUtility.ETX) < 0 || Buffer.IndexOf(ProtocolUtility.EOT) < 0)
                {
                    logger.Debug("Invalid Message : " + Encoding.ASCII.GetString(Buffer.ToArray()));
                    throw new Exception("Invalid Message");
                }

                //            MSGHeader = new String(inputBuffer,1,Array.FindIndex(inputBuffer,1,MatchSTX));
                Encoding ascii = Encoding.ASCII;

                // Check the BCC           
                String BCC1 = _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.IndexOf(ProtocolUtility.ETX)).ToArray()).PadLeft(4, '0');
                String BCC2 = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.ETX) + 1, 4);
                if (BCC1 != BCC2)
                {
                    logger.Debug("BCC Error " + String.Format("{0} != {1}", BCC1, BCC2));
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
    }
}
