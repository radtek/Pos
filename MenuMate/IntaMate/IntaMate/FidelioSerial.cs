using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
using System.Diagnostics;
using IntaMate.Properties;

namespace IntaMate
{
    class PMSInterfaceFidelioSerial : IntaMate.PMSInterface
    {
        protected const int DOLLARS_AMOUNT_LENGTH = 7;

        public bool Active = false;

        public PMSInterfaceFidelioSerial()
        {
            //roomNumberLength = IntaMate.Properties.Settings.Default.RoomNumberLength;
        }

        protected override I_IO_Interface CreateTransport()
        {
            return new InterfaceIOSerial(9600,2000,10000);
        }
        
        public override void SaveConfig(){}

        public override bool Connect()
        {
            try
            {
                Active = GetTransport().Online();
            }
            catch (System.Exception ex)
            {
                Debug.WriteLine("Micros Interface Error : " + ex.ToString());
                Active = false;
            }
            return Active;
        }

        ~PMSInterfaceFidelioSerial()
        {
            GetTransport().Close();
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
                    Debug.WriteLine("\rMicrosServer Online: Ack Received");
                    RetVal = true;
                }
                else if (Ack == ProtocolUtility.NAK)
                {
                    Debug.WriteLine("\rMicrosServer Online: Nak Received");
                }
                else
                {
                    Debug.WriteLine("\rMicrosServer Online: Unexpected Response");
                }
            }
            catch (System.TimeoutException ex)
            {
                Debug.WriteLine("\rMicrosServer Online: No Reply " + ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.WriteLine("\rMicrosServer Online: " + ex.Message);
            }
            return RetVal;
        }

        /*public override PMSRoomChargeResponse RoomCharge(PhoenixPacketChargeToRoom ChargeToRoom)
        {
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
                byte Ack = ProtocolUtility.NAK;
                Ack = (byte)GetTransport().ReadByte();
                if (Ack == ProtocolUtility.ACK)
                {
                    Debug.WriteLine("\rMicrosServer Room Charge: Ack Received");
                }
                else if (Ack == ProtocolUtility.NAK)
                {
                    Debug.WriteLine("\rMicrosServer Room Charge: Nak Received");
                }
                else
                {
                    Debug.WriteLine("\rMicrosServer Room Charge: Unexpected Response");
                }

                // Read in Response.
                List<byte> Buffer = new List<byte>();
                Buffer.Clear();
                int Count = 0;
                int BytesLeft = 44;
                byte[] Dat = new byte[BytesLeft];
                int Index = 0;
                do
                {
                    Count = GetTransport().Read(Dat, Index, BytesLeft);

                    if (Count != 0)
                    {
                        byte[] Temp = new byte[Count];
                        Array.Copy(Dat, Index, Temp, 0, Count);
                        Buffer.AddRange(Temp);
                        Index += Count;
                        BytesLeft -= Count;
                    }
                    else
                    {
                        BytesLeft = 0;
                    }
                }
                while (BytesLeft != 0);
                PostingResponse(Buffer);

                Response = _data.ExportPMSRoomChargeResponse();

                byte[] AckData = new byte[1];
                AckData[0] = ProtocolUtility.ACK;
                GetTransport().Write(AckData, 0, 1);
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
                throw new Exception("No Response from Hotel System. Please try again.");
            }

            if (Buffer.IndexOf(ProtocolUtility.SOT) < 0 || Buffer.IndexOf(ProtocolUtility.STX) < 0
               || Buffer.IndexOf(ProtocolUtility.ETX) < 0 || Buffer.IndexOf(ProtocolUtility.EOT) < 0)
            {
                throw new Exception("Invalid Message");
            }

            //            MSGHeader = new String(inputBuffer,1,Array.FindIndex(inputBuffer,1,MatchSTX));
            Encoding ascii = Encoding.ASCII;

            // Check the BCC           
            String BCC1 = _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.IndexOf(ProtocolUtility.ETX)).ToArray()).PadLeft(4, '0');
            String BCC2 = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.ETX) + 1, 4);
            if (BCC1 != BCC2)
            {
                throw new Exception("BCC Error");
            }

            String station = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 1, 4);
            String establishment = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.SOT) + 5, Buffer.IndexOf(ProtocolUtility.STX) - (Buffer.IndexOf(ProtocolUtility.SOT) + 5));

            if (Buffer[22] == '/')
            {
                _data.PostingResponse = ascii.GetString(Buffer.ToArray(), 23, 15);
            }
            else
            {
                _data.PostingResponse = "ACK";
            }
        }

        protected byte[] Posting()
        {
            Encoding ascii = Encoding.ASCII;
            List<byte> Buffer = new List<byte>();
            /*Buffer.Add(ProtocolUtility.SOT);
            Buffer.AddRange(ascii.GetBytes(_data.RegisterNumber.PadLeft(2, '0')));
            Buffer.AddRange(ascii.GetBytes(_data.Establishment.PadRight(16, ' ')));
            Buffer.Add(ProtocolUtility.STX);
            Buffer.AddRange(ascii.GetBytes(" 2"));
            Buffer.AddRange(ascii.GetBytes(" "));
            if (_data.RoomNumber.Length > 20)
            {
                Buffer.AddRange(ascii.GetBytes(_data.RoomNumber.Substring(0, 20)));
            }
            else
            {
                Buffer.AddRange(ascii.GetBytes(_data.RoomNumber.PadRight(20, ' ')));
            }           
            Buffer.AddRange(ascii.GetBytes(_data.PostingFoilo));
            Buffer.Add((byte)(_data.PostingFoiloNumber));
            Buffer.AddRange(ascii.GetBytes("9995")); // Casher # 9995
            Buffer.AddRange(ascii.GetBytes("9995")); // Waiter  # 9995
            //Buffer.AddRange(ascii.GetBytes(Data.ServerNo.Substring(0, SERVERNO_LENGTH).PadLeft(SERVERNO_LENGTH, '0'))); // Waiter  # 9995
            Buffer.AddRange(ascii.GetBytes("995"));  // Revenue Center 995
            Buffer.AddRange(ascii.GetBytes("  10")); // Serving Period '  10'
            Buffer.AddRange(ascii.GetBytes("020"));  // Guest Check Number (PMS-Ref)
            Buffer.AddRange(ascii.GetBytes("   0")); // Transaction No#
            //Buffer.AddRange(ascii.GetBytes("   1")); // Number of Covers
            Buffer.AddRange(ascii.GetBytes(_data.Covers.PadLeft(COVERS_LENGTH, '0').Substring(0, COVERS_LENGTH))); // Number of Covers
            Buffer.AddRange(ascii.GetBytes(" 17")); // Payment Method
            // Total Amount          
            int Dollars = Convert.ToInt32(_data.PostingTotal) / 100;
            int Cents = Convert.ToInt32(_data.PostingTotal) % 100;
            String amount = String.Format("{0}.{1}",
                            Dollars.ToString().PadLeft(DOLLARS_AMOUNT_LENGTH, ' '),
                            Math.Abs(Cents).ToString().PadLeft(CENTS_AMOUNT_LENGTH, '0'));

            Buffer.AddRange(ascii.GetBytes(amount));
            //Sales Total 1
            Buffer.AddRange(ascii.GetBytes(amount));
            //Sales Total 2
            amount = String.Format("{0}.{1}", "0".PadLeft(DOLLARS_AMOUNT_LENGTH, ' '), "00".PadLeft(CENTS_AMOUNT_LENGTH, '0'));
            Buffer.AddRange(ascii.GetBytes(amount));
            //Sales Total 3           
            Buffer.AddRange(ascii.GetBytes(amount));
            //Sales Total 4
            Buffer.AddRange(ascii.GetBytes(amount));
            //Discount Total
            Buffer.AddRange(ascii.GetBytes(amount));
            //Tip Total
            Buffer.AddRange(ascii.GetBytes(amount));
            //Entertainment Total
            Buffer.AddRange(ascii.GetBytes(amount));
            //Tax Total 1
            Buffer.AddRange(ascii.GetBytes(amount));
            //Tax Total 2
            Buffer.AddRange(ascii.GetBytes(amount));
            //Tax Total 3
            Buffer.AddRange(ascii.GetBytes(amount));
            //Tax Total 4
            Buffer.AddRange(ascii.GetBytes(amount));
            //Prev Payments
            Buffer.AddRange(ascii.GetBytes(amount));
            Buffer.Add(ProtocolUtility.ETX);
            Buffer.AddRange(ascii.GetBytes(_protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.Count - 1).ToArray()).PadLeft(4, '0')));
            Buffer.Add(ProtocolUtility.EOT);*/
            return Buffer.ToArray();
        }

        /*public PMSRoomStatusResponse RoomInquiry(PMSRoomStatus Status)
        {
            _data.Import(Status);
            PMSRoomStatusResponse RoomStatusResponse = new PMSRoomStatusResponse();
            try
            {
                byte[] ByteArray = Inquiry();
                GetTransport().DiscardInBuffer();
                GetTransport().Write(ByteArray, 0, ByteArray.Length);
                byte Ack = ProtocolUtility.NAK;
                Ack = (byte)GetTransport().ReadByte();
                if (Ack == ProtocolUtility.ACK)
                {
                    Debug.WriteLine("\rMicrosServer : Ack Received (Message Acknowledged)");
                }
                else if (Ack == ProtocolUtility.NAK)
                {
                    Debug.WriteLine("\rMicrosServer : Nak Received (Message Rejected)");
                }
                else
                {
                    Debug.WriteLine("\rMicrosServer : Unexpected Response");
                }

                List<byte> Buffer = new List<byte>();

                int Count = 0;
                int BytesLeft = 172;
                byte[] Dat = new byte[BytesLeft];
                int Index = 0;
                do
                {
                    try
                    {
                        Count = GetTransport().Read(Dat, Index, BytesLeft);
                    }
                    catch (System.Net.Sockets.SocketException)
                    {
                        BytesLeft = 0;
                        Count = 0;
                    }
                    catch (System.TimeoutException)
                    {
                        BytesLeft = 0;
                        Count = 0;
                    }

                    if (Count != 0)
                    {
                        byte[] Temp = new byte[Count];
                        Array.Copy(Dat, Index, Temp, 0, Count);
                        Buffer.AddRange(Temp);
                        Index += Count;
                        BytesLeft -= Count;
                    }
                }
                while (BytesLeft != 0);

                InquiryResponse(Buffer);
                // Provide a Cach of this room number and foilos 
                // for quick look up when rooms are charged.               
                RoomStatusResponse = _data.ExportPMSRoomStatusResponse();

                byte[] AckData = new byte[1];
                AckData[0] = ProtocolUtility.ACK;
                GetTransport().Write(AckData, 0, 1);
            }
            catch (System.TimeoutException ex)
            {
                Debug.WriteLine("\rMicrosServer : No Reply :" + ex.Message);
                RoomStatusResponse.ErrorResponse = ex.Message;
            }
            catch (System.Exception ex)
            {
                Debug.WriteLine("\rMicrosServer : " + ex.Message);
                RoomStatusResponse.ErrorResponse = ex.Message;
            }
            return RoomStatusResponse;
        }*/

        protected override void InquiryResponse(List<byte> Buffer)
        {
            if (Buffer.Count == 0)
            {
                throw new Exception("No Response from Hotel System. Please try again.");
            }

            if (Buffer.IndexOf(ProtocolUtility.SOT) < 0 || Buffer.IndexOf(ProtocolUtility.STX) < 0
                           || Buffer.IndexOf(ProtocolUtility.ETX) < 0 || Buffer.IndexOf(ProtocolUtility.EOT) < 0)
            {
                throw new Exception("Invalid Message");
            }

            //            MSGHeader = new String(inputBuffer,1,Array.FindIndex(inputBuffer,1,MatchSTX));
            Encoding ascii = Encoding.ASCII;

            // CHeck the BCC
            String BCC1 = _protocolUtility.GetBCC(Buffer.GetRange(1, Buffer.IndexOf(ProtocolUtility.ETX)).ToArray()).PadLeft(4, '0');
            String BCC2 = ascii.GetString(Buffer.ToArray(), Buffer.IndexOf(ProtocolUtility.ETX) + 1, 4);
            if (BCC1 != BCC2)
            {
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

        protected byte[] RoomInquiry()
        {
            Encoding ascii = Encoding.ASCII;
            List<byte> Buffer = new List<byte>();
            /*Buffer.Add(ProtocolUtility.SOT);

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
            Buffer.Add(ProtocolUtility.EOT);*/
            return Buffer.ToArray();
        }

        protected override bool CheckResponce()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        protected override void ReceiveAccountChargeResponce()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        protected override List<PMSMessage> BuildTransmissionArray()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        protected override void Trasmit(PMSMessage message)
        {
            throw new Exception("The method or operation is not implemented.");
        }

        protected override void ReceiveInquiry()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        protected override PMSMessage BuildInquiry()
        {
            throw new Exception("The method or operation is not implemented.");
        }
    }

}
