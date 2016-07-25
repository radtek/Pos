using System;
using System.Collections.Generic;

namespace IntaMate
{
    class PMSInterfaceHirum : IntaMate.PMSInterfaceMicros
    {
        public PMSInterfaceHirum()
        {
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
                                            leftOverProductPortion.PartialPayment("99", -(currentProduct.Value - paymentUsed));
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
                            else
                            { // there is money left over better wirte that down.

                            }
                        }
                    }
                    // Ok we have been though all the products is there any remianing payments.

                }
                else
                { // Pure room charge with no other payments.
                    if (_data.RoomNumber != "0")
                    {
                        foreach (KeyValuePair<String, int> currentProduct in Products)
                        {
                            ProductPortion productPortion = new ProductPortion(currentProduct.Key, currentProduct.Value);
                            productPortion.PartialPayment("99", -currentProduct.Value);
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
                    AccountCharge.PaymentMethod = "99";
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
    }
}

