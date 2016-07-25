using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace IntaMate
{
    public abstract class PMSMessageMicrosOrig : PMSMessage
    {
        protected const int DOLLARS_AMOUNT_LENGTH = 7;

        protected String _registerNumber;
        protected String _establishment;
        protected String _roomNumber;

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
        }
    }

    public class PMSMessageMicrosOrigAccCharge : PMSMessageMicrosOrig
    {
        private String _pMSRef;
        // Postings.
        private String _postingTotal;
        private String _postingFoilo;
        private char   _postingFoiloNumber;

        private String PMSReference
        {
            get { return _pMSRef; }
            set 
            {
                if (value.Length > PMSREF_LENGTH)
                {
                    _pMSRef = value.Substring(0, PMSREF_LENGTH);
                }
                else
                {
                    _pMSRef = value.PadRight(PMSREF_LENGTH, ' ');
                }
            }
        }

        public String PostingTotal
        {
            get { return _postingTotal; }
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

        public PMSMessageMicrosOrigAccCharge()
        {
            _messageType = " 2";
        }
      
        public void ImportStaticData(RoomData data)
        {
            ImportBaseData(data);

            _postingFoilo = data.PostingFoilo;
            _postingFoiloNumber = data.PostingFoiloNumber;
            PMSReference = data.PMSRef;
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
            Buffer.AddRange(ascii.GetBytes(_postingFoilo));
            logger.Trace("_postingFoilo " + _postingFoilo);
            Buffer.Add((byte)(_postingFoiloNumber));
            logger.Trace("_postingFoiloNumber " + _postingFoiloNumber);
            Buffer.AddRange(ascii.GetBytes(new String(' ', 14)));

            Buffer.AddRange(ascii.GetBytes(_pMSRef)); // PMS-Ref 8 bytes
            logger.Trace("_pMSRef " + _pMSRef);          
            Buffer.AddRange(ascii.GetBytes(" "));

            // Total Amount          
            Buffer.AddRange(ascii.GetBytes(_postingTotal)); // Payment Method
            logger.Trace("_postingTotal " + _postingTotal);

            Buffer.AddRange(ascii.GetBytes(" "));
            Buffer.Add(ProtocolUtility.ETX);
            Buffer.AddRange(ascii.GetBytes(_protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0')));
            Buffer.Add(ProtocolUtility.EOT);
            return Buffer.ToArray();
        }

    }    
}
