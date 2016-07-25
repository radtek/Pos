using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    public class PMSMessageHirumAccCharge : PMSMessageAccCharge
    {
        public PMSMessageHirumAccCharge(): base()
        {
            REVENUE_CENTER_LENGTH = 10;
        }
       
        public override byte[] ToByteArray()
        {
            Encoding ascii = Encoding.ASCII;
            List<byte> Buffer = new List<byte>();
            Buffer.Add(ProtocolUtility.SOT);
            Buffer.AddRange(ascii.GetBytes(_registerNumber));
            logger.Trace("_registerNumber " + _registerNumber.Length.ToString() + ":" + _registerNumber);
            Buffer.AddRange(ascii.GetBytes(_establishment));
            logger.Trace("_establishment " + _establishment.Length.ToString() + ":" + _establishment);
            Buffer.Add(ProtocolUtility.STX);
            Buffer.AddRange(ascii.GetBytes(_messageType));
            logger.Trace("_messageType " + _messageType.Length.ToString() + ":" + _messageType);
            Buffer.AddRange(ascii.GetBytes(" "));
            Buffer.AddRange(ascii.GetBytes(_roomNumber));
            logger.Trace("_roomNumber " + _roomNumber.Length.ToString() + ":" + _roomNumber);
            Buffer.AddRange(ascii.GetBytes(_postingFoilo));
            logger.Trace("_postingFoilo " + _postingFoilo.Length.ToString() + ":" + _postingFoilo);
            Buffer.Add((byte)(_postingFoiloNumber));
            logger.Trace("_postingFoiloNumber " + "1" + ":" + _postingFoiloNumber);
            Buffer.AddRange(ascii.GetBytes(_staffID)); // Casher # 9995
            // Fix this once you are sending the Payroll ID as the Server Number.
            //Buffer.AddRange(ascii.GetBytes(Data.ServerNo.PadLeft(4,'0'))); // Casher # 9995
            Buffer.AddRange(ascii.GetBytes(_staffID)); // Waiter  # 9995
            logger.Trace("_staffID " + _staffID.Length.ToString() + ":" + _staffID);
            //Buffer.AddRange(ascii.GetBytes(Data.ServerNo.Substring(0, SERVERNO_LENGTH).PadLeft(SERVERNO_LENGTH, '0'))); // Waiter  # 9995
            Buffer.AddRange(ascii.GetBytes(_revenueCenter));  // Revenue Center 995 This will become product code.
            logger.Trace("_revenueCenter " + _revenueCenter.Length.ToString() + ":" + _revenueCenter);
            //Buffer.AddRange(ascii.GetBytes("  10")); // Serving Period '  10'
            Buffer.AddRange(ascii.GetBytes(_servingPeriod)); // Serving Period Breakfast
            logger.Trace("_servingPeriod " + _servingPeriod.Length.ToString() + ":" + _servingPeriod);

            //Buffer.AddRange(ascii.GetBytes("0020"));  // Guest Check Number (PMS####)
            //Buffer.AddRange(ascii.GetBytes("   0")); // Transaction No#
            Buffer.AddRange(ascii.GetBytes(_pMSRef)); // PMS-Ref 8 bytes
            logger.Trace("_pMSRef " + _pMSRef.Length.ToString() + ":" + _pMSRef);

            //Buffer.AddRange(ascii.GetBytes("   1")); // Number of Covers
            Buffer.AddRange(ascii.GetBytes(_covers)); // Number of Covers
            logger.Trace("_covers " + _covers.Length.ToString() + ":" + _covers);
            Buffer.AddRange(ascii.GetBytes(_paymentMethod)); // Payment Method
            logger.Trace("_paymentMethod " + _paymentMethod.Length.ToString() + ":" + _paymentMethod);
            // Total Amount          
            Buffer.AddRange(ascii.GetBytes(_postingTotal)); // Posting Total
            logger.Trace("_postingTotal " + _postingTotal.Length.ToString() + ":" + _postingTotal);

            Buffer.AddRange(ascii.GetBytes(_productTotal)); // Product Total
            logger.Trace("_productTotal " + _productTotal.Length.ToString() + ":" + _productTotal);

            for (int i = 0; i < _subTotals.Length; i++)
            {
                Buffer.AddRange(ascii.GetBytes(_subTotals[i])); // Payment Method
                logger.Trace(i.ToString() + " : " + _subTotals[i]);
            }

            Buffer.Add(ProtocolUtility.ETX);
            Buffer.AddRange(ascii.GetBytes(_protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0')));
            logger.Trace("BCC " + _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0'));
            Buffer.Add(ProtocolUtility.EOT);
            return Buffer.ToArray();
        }

        /*internal void SetPaymentCode(String Key, int Value)
        {
            int Dollars = Value / 100;
            int Cents = Value % 100;
            String subTotal = "";
            if (Cents < 0 && Dollars == 0)
            { // A negitive Value of Cents only.
                subTotal = String.Format("{0}.{1}",
                                "-0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
            }
            else
            { // The sign is reflected in the dollars amount.
                subTotal = String.Format("{0}.{1}",
                                Dollars.ToString().PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
            }
            _paymentMethod = Key.PadLeft(3, ' ').Substring(0, 3);
            _subTotals[0] = subTotal;
            logger.Trace("Payment Type " + _paymentMethod.ToString() + "Total "+ subTotal);
            logger.Trace("Suggested Values.");
            logger.Trace("Payment Type 4 Cash");
            logger.Trace("Payment Type 5 Amex");
            logger.Trace("Payment Type 8 Diners");
            logger.Trace("Payment Type 9 Eftpos");
            logger.Trace("Payment Type 10 Cheque");
            logger.Trace("Payment Type 11 Visa");
            logger.Trace("Payment Type 13 Mastercard");
            logger.Trace("Payment Type 17 Room Charge");
            logger.Trace("Payment Type 19 Vouchers");            
        }*/

        internal override void SetPaymentCode(String Key, int Value)
        {
            int Dollars = Value / 100;
            int Cents = Value % 100;
            String subTotal = "";
            if (Cents < 0 && Dollars == 0)
            { // A negitive Value of Cents only.
                subTotal = String.Format("{0}.{1}",
                                "-0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
            }
            else
            { // The sign is reflected in the dollars amount.
                subTotal = String.Format("{0}.{1}",
                                Dollars.ToString().PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
            }

            _paymentMethod = Key.PadLeft(3, ' ').Substring(0, 3);
            switch (Key)
            {
                case "10": // Cash
                    {
                        _productTotal = subTotal;
                        logger.Trace("Cash " + subTotal);
                    }
                    break;
                case "11": // Cheque
                    {
                        _subTotals[0] = subTotal;
                        logger.Trace("Cheque " + subTotal);
                    }
                    break;
                case "12": // Eftpos
                    {
                        _subTotals[1] = subTotal;
                        logger.Trace("Eftpos " + subTotal);
                    }
                    break;
                case "13": // Amex
                    {
                        _subTotals[2] = subTotal;
                        logger.Trace("Amex " + subTotal);
                    }
                    break;
                case "14": // Diners
                    {
                        _subTotals[3] = subTotal;
                        logger.Trace("Diners " + subTotal);
                    }
                    break;
                case "15": // Visa
                    {
                        _subTotals[4] = subTotal;
                        logger.Trace("Visa " + subTotal);
                    }
                    break;
                case "16": // Mastercard
                    {
                        _subTotals[5] = subTotal;
                        logger.Trace("Mastercard " + subTotal);
                    }
                    break;
                case "17": // Vouchers
                    {
                        _subTotals[6] = subTotal;
                        logger.Trace("JCB " + subTotal);
                    }
                    break;
                case "99": // Room Charge
                    {
                        PostingTotal = -Value;
                        logger.Trace("Room Charged " + subTotal);
                    }
                    break;
            }
        }
    }
   
}

