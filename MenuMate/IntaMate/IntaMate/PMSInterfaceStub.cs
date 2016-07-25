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
    class PMSInterfaceStub : IntaMate.PMS
    {
        protected static Logger logger = LogManager.GetCurrentClassLogger();

        public bool Active = false;

        public PMSInterfaceStub() : base()
        {
            SetIOType(e_IOType.proto_Stub);
            SetMsgType(e_PMSMSGType.msgNone);
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
            bool RetVal = true;
            logger.Info("\rInterface Stub Online Ok");               
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

            //loop rough Data.Catagories.
            Dictionary<String, int> Payments = new Dictionary<String, int>();
            Dictionary<String, int> Products = new Dictionary<String, int>();
            // Product Key , < Payment Key , Amount >.
            foreach (KeyValuePair<String, int> KeyValue in _data.Catagories)
            {
                int IntKeyValue = 0;
                if (Int32.TryParse(KeyValue.Key, out IntKeyValue))
                {// All Cats under 100 are payments.
                    if (IntKeyValue < 100)
                    {
                        Payments[KeyValue.Key] = KeyValue.Value;
                        logger.Debug("\rPayment Code" + KeyValue.Key.ToString() + "Value " + KeyValue.Value.ToString());
                    }
                    else
                    { // All Cats over 100 are products.
                        Products[KeyValue.Key] = KeyValue.Value;
                        logger.Debug("\rProduct Code" + KeyValue.Key.ToString() + "Value " + KeyValue.Value.ToString());
                    }
                }
                else
                {
                    Products[KeyValue.Key] = KeyValue.Value;
                    logger.Debug("\rProduct Code" + KeyValue.Key.ToString() + "Value " + KeyValue.Value.ToString());
                }
            }

            List<ProductPortion> Transactions = new List<ProductPortion>();

            IEnumerator<KeyValuePair<String, int>> currentPayment = Payments.GetEnumerator();
            //ProductPortion CurrentProduct = new ProductPortion(Product.Key, Product.Value);
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
                AccountCharge.RevenueCenter = "0";
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
                    logger.Info(subTotal + " of " + emessage.Current._code + " Not accounted for!");
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
            logger.Info(Encoding.ASCII.GetString(ByteArray));
        }

        protected override void ReceiveAccountChargeResponce()
        {            
            List<byte> Buffer = new List<byte>();
            PostingResponse(Buffer);
        }

        protected override void ReceiveInquiry()
        {
            List<byte> Buffer = new List<byte>();
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
            _data.PostingResponse = "ACK";            
        }

        protected override void InquiryResponse(List<byte> Buffer)
        {           
            Encoding ascii = Encoding.ASCII;
            String station = "Stub Station";
            String establishment = "MenuMate Test";

            List<String> Foilos = new List<String>();
            _data.ErrorResponse = "";
            for(int i = 0; i < 5; i++)
            {
                Foilos.Add("Filos " + i.ToString());
            }
            _cache[_data.RoomNumber] = Foilos;
        }       
    }
}