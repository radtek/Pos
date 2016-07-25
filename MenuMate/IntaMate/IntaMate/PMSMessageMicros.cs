using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    public abstract class PMSMessageMicros : PMSMessage
    {
        protected const int DOLLARS_AMOUNT_LENGTH = 7;
        protected const int ESTABISHMENT_LENGTH = 16;
        protected int REVENUE_CENTER_LENGTH = 3;
        protected const int STATION_LENGTH = 2;

        protected String _registerNumber;
        protected String _establishment;
        protected String _roomNumber;
        protected String _staffID;

        protected void ImportBaseData(RoomData data)
        {
            if (data.RegisterNumber.Length > STATION_LENGTH)
            {
                _registerNumber = data.RegisterNumber.Substring(data.RegisterNumber.Length - STATION_LENGTH, STATION_LENGTH);
            }
            else
            {
                _registerNumber = data.RegisterNumber.PadLeft(STATION_LENGTH, '0');
            }

            if (data.Establishment.Length > ESTABISHMENT_LENGTH)
            {
                _establishment = data.Establishment.Substring(0, ESTABISHMENT_LENGTH);
            }
            else
            {
                _establishment = data.Establishment.PadRight(ESTABISHMENT_LENGTH, ' ');
            }

            if (data.RoomNumber.Length > 20)
            {
                _roomNumber = data.RoomNumber.Substring(0, 20);
            }
            else
            {
                _roomNumber = data.RoomNumber.PadRight(20, ' ');
            }

            _staffID = "9995".PadRight(4, ' ');
        }
    }

    public class PMSMessageAccCharge : PMSMessageMicros
    {
        protected String _covers;
        protected String _pMSRef;

        // Postings.
        protected String _postingTotal;
        protected String _productTotal;
        protected String _postingFoilo;
        protected char _postingFoiloNumber;
        protected String _servingPeriod = " 2";
        protected String _revenueCenter = "000";
        protected String _paymentMethod = " 17";
        protected String[] _subTotals = new String[11];

        private String PMSReference
        {
            get { return _pMSRef; }
            set 
            {
                String temp = value;
                if (temp.IndexOf("PMS") != -1)
                {
                    temp = temp.Remove(temp.IndexOf("PMS"), 3);
                }

                if (temp.Length >= 4)
                {   // #12345678 .. Fist the bottom 4 digits 5678                
                    _pMSRef = temp.Substring(temp.Length - 4, 4).PadLeft(4, ' ') + temp.Remove(temp.Length - 4, 4).PadLeft(4, ' ');
                }
                else
                {
                    _pMSRef = temp.PadLeft(4, ' ') + "".PadLeft(4, ' ');
                }            
            }
        }

        public String PaymentMethod
        {
            get { return _paymentMethod; }
            set
            {
                _paymentMethod = value.PadLeft(3, ' ');
            }
        }

        public String RevenueCenter
        {
            get { return _revenueCenter; }
            set
            {
                _revenueCenter = value.ToString().PadLeft(REVENUE_CENTER_LENGTH, ' ').Substring(0, REVENUE_CENTER_LENGTH);
            }
        }

        public int PostingTotal
        {
            set
            {
                int Dollars = Convert.ToInt32(value) / 100;
                int Cents = Convert.ToInt32(value) % 100;
                if (Cents < 0 && Dollars == 0)
                { // A negitive Value of Cents only.
                    _postingTotal = String.Format("{0}.{1}",
                                    "-0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                    Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
                }
                else
                { // The sign is reflected in the dollars amount.
                    _postingTotal = String.Format("{0}.{1}",
                                    Dollars.ToString().PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                    Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
                }
            }
        }

        public int ProductTotal
        {
            set
            {
                int Dollars = Convert.ToInt32(value) / 100;
                int Cents = Convert.ToInt32(value) % 100;
                if (Cents < 0 && Dollars == 0)
                { // A negitive Value of Cents only.
                    _productTotal = String.Format("{0}.{1}",
                                    "-0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                    Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
                }
                else
                { // The sign is reflected in the dollars amount.
                    _productTotal = String.Format("{0}.{1}",
                                    Dollars.ToString().PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                                    Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));
                }
            }
        }

        public PMSMessageAccCharge()
        {
            _messageType = " 2";
            String amount = String.Format("{0}.{1}",
                            "0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                            "0".PadLeft(CENTS_AMOUNT_LENGTH, '0'));

            for (int i = 0; i < _subTotals.Length; i++)
            {
                _subTotals[i] = amount;
            }
        }

        public void ImportStaticData(RoomData data)
        {
            ImportBaseData(data);

            _paymentMethod = data.RoomNumber; // This could break RoomNumber 16 byte Str.
            _postingFoilo = data.PostingFoilo.PadLeft(FOILO_LENGTH, ' ');
            _postingFoiloNumber = data.PostingFoiloNumber;
            PostingTotal = Convert.ToInt32(data.PostingTotal); // Data.PostingTotal be an int?

            if (DateTime.Now > new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, 05, 00, 00) &&
               DateTime.Now < new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, 11, 30, 00))
            {
                _servingPeriod = " 10";
            }
            else if (DateTime.Now > new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, 11, 30, 00) &&
               DateTime.Now < new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, 17, 00, 00))
            {
                _servingPeriod = " 11";
            }
            else
            {
                _servingPeriod = " 12";
            }

            PMSReference = data.PMSRef;
            _covers = data.Covers.PadLeft(COVERS_LENGTH, '0').Substring(0, COVERS_LENGTH);
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
            logger.Trace("_staffID " + _staffID.Length.ToString() + ":" + _staffID);
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

        internal virtual void SetPaymentCode(String Key, int Value)
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

            _paymentMethod = Key.PadLeft(3, ' ').Substring(0,3);
            switch (Key)
            {
                case "4": // Cash
                    {
                        _subTotals[0] = subTotal;
                        logger.Trace("Cash " + subTotal);
                    }
                    break;
                case "5": // Amex
                    {
                        _subTotals[1] = subTotal;
                        logger.Trace("Amex " + subTotal);
                    }
                    break;
                case "8": // Diners
                    {
                        _subTotals[2] = subTotal;
                        logger.Trace("Diners " + subTotal);
                    }
                    break;
                case "9": // Eftpos
                    {
                        _subTotals[5] = subTotal;
                        logger.Trace("Eftpos " + subTotal);
                    }
                    break;
                case "11": // Visa
                    {
                        _subTotals[3] = subTotal;
                        logger.Trace("Visa " + subTotal);
                    }
                    break;
                case "13": // Mastercard
                    {
                        _subTotals[4] = subTotal;
                        logger.Trace("Mastercard " + subTotal);
                    }
                    break;
                case "17": // Room Charge
                    {
                        PostingTotal = -Value;
                        logger.Trace("Room Charged " + subTotal);
                    }
                    break;
                case "19": // Vouchers
                    {
                        _subTotals[6] = subTotal;
                        logger.Trace("JCB " + subTotal);
                    }
                    break;
                case "10": // Cheque
                    {
                        _subTotals[7] = subTotal;
                        logger.Trace("Cheque " + subTotal);
                    }
                    break;
            }
        }
    }

    public class PMSMessageMicrosInquiry : PMSMessageMicros
    {
        public PMSMessageMicrosInquiry()
        {
            _messageType = " 1";
        }

        public void ImportStaticData(RoomData data)
        {
            ImportBaseData(data);                      
        }

        public override byte[] ToByteArray()
        {
            Encoding ascii = Encoding.ASCII;
            List<byte> Buffer = new List<byte>();
            Buffer.Add(ProtocolUtility.SOT);
            Buffer.AddRange(ascii.GetBytes(_registerNumber));
            logger.Trace("_registerNumber " + _registerNumber);
            Buffer.AddRange(ascii.GetBytes(_establishment));
            logger.Trace("_establishment " + _establishment);
            Buffer.Add(ProtocolUtility.STX);
            Buffer.AddRange(ascii.GetBytes(_messageType));
            logger.Trace("_messageType " + _messageType);
            Buffer.AddRange(ascii.GetBytes(" "));
            Buffer.AddRange(ascii.GetBytes(_roomNumber));
            logger.Trace("_roomNumber " + _roomNumber);
            //Employee Number.
            Buffer.AddRange(ascii.GetBytes(_staffID));
            logger.Trace("_staffID " + _staffID);
            Buffer.Add(ProtocolUtility.ETX);
            Buffer.AddRange(ascii.GetBytes(_protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0')));
            logger.Trace("BCC " + _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0'));
            Buffer.Add(ProtocolUtility.EOT);
            return Buffer.ToArray();                      
        }
    }

}

