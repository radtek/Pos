using System;
using System.Collections.Generic;
using System.Collections;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    public enum e_IOType { proto_Stub, proto_RS232, proto_TCPIP };        

    abstract class PMS : IPMSInterface
    {
        protected e_IOType ioType;
        private I_IO_Interface IO;
        private PMSMessageFactory msgType;

        protected static Logger logger = LogManager.GetCurrentClassLogger();

        protected ProtocolUtility _protocolUtility = new ProtocolUtility(); //TODO : Remove this it belongs in the message.

        //public List<String> Foilos = new List<String>();
        public Dictionary<String, List<String>> _cache = new Dictionary<string, List<string>>();

        protected RoomData _data = new RoomData();

        abstract public bool Connect();
        //abstract public PMSRoomChargeResponse RoomCharge(PhoenixPacketChargeToRoom ChargeToRoom);
        //abstract public PMSRoomStatusResponse RoomInquiry(PMSRoomStatus Status);
        abstract public bool IsConnected();
        abstract public void SaveConfig();

        abstract protected bool CheckResponce();
        
        abstract protected void ReceiveAccountChargeResponce();
        abstract protected void ReceiveInquiry();

        abstract protected List<PMSMessage> BuildTransmissionArray();
        abstract protected PMSMessage BuildInquiry();
        abstract protected void Trasmit(PMSMessage message);

        abstract protected void PostingResponse(List<byte> Buffer);
        abstract protected void InquiryResponse(List<byte> Buffer);

        public PMS()
        {
            msgType = new PMSMessageFactory();
            
            SetIOType((e_IOType)Enum.Parse(typeof(e_IOType), IntaMate.Properties.Settings.Default.IOInterfaceType));
            SetMsgType((e_PMSMSGType)Enum.Parse(typeof(e_PMSMSGType), IntaMate.Properties.Settings.Default.InterfaceMsgType));
        }

        public void SetIOType(e_IOType IOType)
        {
            switch(IOType)
            {
                case e_IOType.proto_RS232 :
                    {
                        //throw new Exception("proto_RS232 Not Implemented");
                        // These serial configs have to be loaded from somewhere.
                        // [Interface][IOType]=[Settings]
                        // Baud.
                        // ReadTimeOut
                        // WriteTimeOut
                        // Fedilo
                        // IO = new IO_RS232(9600,2000,10000);
                        // Micros
                        // IO = new IO_RS232(57600, 5000, 10000);
                    }
                    break;
                case e_IOType.proto_TCPIP :
                    {
                        // These TCPIP configs have to be loaded from somewhere.
                        // [Interface][IOType]=[Settings]
                        // IPAddress
                        // Port
                        // ReceiveTimeOut
                        IO = new IO_TCPIP(IntaMate.Properties.Settings.Default.IPAddress,
                                          Convert.ToInt32(IntaMate.Properties.Settings.Default.Port),
                                          Convert.ToInt32(IntaMate.Properties.Settings.Default.ReadTimeOut));
                    }
                    break;
                case e_IOType.proto_Stub:
                    {
                        IO = new IO_Stub();
                    }
                    break;
                default :
                    {
                        throw new Exception("Invalid IO Type For Interface");
                    }
            }
        }

        protected I_IO_Interface GetTransport()
        {
            return IO;
        }

        public void SetMsgType(e_PMSMSGType inMsgType)
        {
            msgType.setMsgType(inMsgType);
        }

        

        protected PMSMessageFactory GetMsg()
        {
            return msgType;
        }


        public PMSRoomChargeResponse Charge(PhoenixPacketChargeToRoom chargeToRoom)
        {
            PMSRoomChargeResponse Response = new PMSRoomChargeResponse();
            try
            {
                UpdateCache(chargeToRoom.RoomNumber, chargeToRoom.RegisterNo);
                _data.Import(_cache, chargeToRoom);               
                
                List<PMSMessage> messagesToTransmit = BuildTransmissionArray();
                IEnumerator<PMSMessage> emessage = messagesToTransmit.GetEnumerator();
                if(emessage.MoveNext())
                {
                    do
                    {
                        Trasmit(emessage.Current);
                        ReceiveAccountChargeResponce();
                    }while(emessage.MoveNext() && CheckResponce());
                }
                Response = _data.ExportPMSRoomChargeResponse();
            }
            catch (System.TimeoutException ex)
            {
                logger.Error("MicrosServer : No Reply :" + ex.Message);
                Response.ErrorResponse = ex.Message;
            }
            catch (System.Exception ex)
            {
                logger.Error("\rMicrosServer : " + ex.Message);
                Response.ErrorResponse = ex.Message;
            }
            return Response;
        }

        public PMSRoomStatusResponse Inquiry(PMSRoomStatus Status)
        {
            _data.Import(Status);
            PMSRoomStatusResponse Response = new PMSRoomStatusResponse();
            try
            {
                PMSMessage messageToTransmit = BuildInquiry();
                Trasmit(messageToTransmit);
                ReceiveInquiry();
                // Provide a Cach of this room number and foilos 
                // for quick look up when rooms are charged.               
                Response = _data.ExportPMSRoomStatusResponse(_cache);
            }
            catch (System.TimeoutException ex)
            {
                logger.Error("MicrosServer : No Reply :" + ex.Message);
                Response.ErrorResponse = ex.Message;
            }
            catch (System.Exception ex)
            {
                logger.Error("\rMicrosServer : " + ex.Message);
                Response.ErrorResponse = ex.Message;
            }
            return Response;
        }


        private void UpdateCache(String roomNumber, String RegisterNumber)
        {
            if (!_cache.ContainsKey(roomNumber) && roomNumber != "0")
            {
                // We need to get the Foilio Name from the Micros system based on index.
                PMSRoomStatus ExRoomStatus = new PMSRoomStatus();
                ExRoomStatus.RegisterNumber = RegisterNumber;
                ExRoomStatus.RoomNumber = roomNumber;
                PMSRoomStatusResponse ExRoomStatusResponse = Inquiry(ExRoomStatus);
            }
        }

    }
}
