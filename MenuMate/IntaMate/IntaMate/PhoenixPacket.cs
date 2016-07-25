using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
using NLog;

namespace IntaMate
{
    class PhoenixPacketBase
    {
        String packetLength;
        const int PACKET_LENGTH = 4;
        const char PACKETTYPE = '0';
        protected const String ack = "ACK";
        protected const String nack = "NACK";

        public PhoenixPacketBase()
        {
            PacketLength = "0";
        }

        public String PacketLength
        {
            get { return packetLength; }
            set { packetLength = value.PadLeft(PACKET_LENGTH, '0'); }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(@"\");
            sb.Append(PACKETTYPE + ",");
            sb.Append("0000" + ",");
            sb.Append(nack);
            sb.Append("\r");
            return UpdatePacketLength(sb.ToString());
        }

        public string UpdatePacketLength(String Data)
        {
            int TempLength = Data.Length-2; // Not inclding the / or the \r
            PacketLength = TempLength.ToString();
            Data = Data.Remove(3,4);
            Data = Data.Insert(3,PacketLength);
            return Data;
        }
    }

    class PhoenixPacketNAK : PhoenixPacketBase 
    {
        const char PACKETTYPE = '0';
        private String ErrorMsg;

        public PhoenixPacketNAK(String inError)
        {
            ErrorMsg = inError;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(@"\");
            sb.Append(PACKETTYPE + ",");
            sb.Append("0000" + ",");
            sb.Append(nack + ErrorMsg);
            sb.Append("\r");
            return UpdatePacketLength(sb.ToString());
        }
    }

    class PhoenixPacketBaseResponse : PhoenixPacketBase
    {
        String errorResponse;
        String response;

        public PhoenixPacketBaseResponse()
        {
            errorResponse = "";
            Response = "";
        }

        public void ACK()
        {
            Response = ack;
        }

        public void NACK()
        {
            Response = nack;
        }

        public String Response
        {
            get { return response; }
            set { response = value; }
        }

        public String ErrorResponse
        {
            get { return errorResponse; }
            set { errorResponse = value; }
        }

    }

    /* Type 7 : Request Extended Room Status Received From MenuMate
     * Format		7,<Length>,<RegisterNo>,<RoomNo>
     * Purpose	Send by POS to find out whether the room can be charged to and the folio
     * balances/credit limits of the account.
     * <RegisterNo> the register identifier, typically a number from 1 to 20
     */ 

    class PMSRoomStatus : PhoenixPacketBase
    {
        const char PACKETTYPE = '7';
        const int HEADER_POSITION = 0;
        const int LENGTH_POSITION = 1;
        const int REGISTERNO_POSITION = 2;
        const int ROOMNO_POSITION = 3;

        private String registerNumber;

        private String roomNumber;

        public String RoomNumber
        {
            get { return roomNumber; }
            set { roomNumber = value; }
        }

        public String RegisterNumber
        {
            get { return registerNumber; }
            set { registerNumber = value; }
        }

        public void Parse(String Buffer)
        {
            String[] Data = Buffer.Split(',');

            if (Data[HEADER_POSITION] != ("\\" + PACKETTYPE))
            {
                throw new Exception("Invalid Message");
            }
            
            RoomNumber = Data[ROOMNO_POSITION];
            registerNumber = Data[REGISTERNO_POSITION];
        }
    }

    /* Type 8 : Extended Room Status Response sent to MenuMate
     * Format:		2,<Length>,<RegisterNo>,<RoomNo>,<Balance>,<CreditLimit>,
     * <NoNames>,<Text>...,<Text>
     * Purpose	Response of PMS Responses to type 7 (based on message type 2)
     * <Balance> is the current account balance in cents
     * <CreditLimit> is the credit limit of the account in cents
     */

    class PMSRoomStatusResponse : PhoenixPacketBaseResponse
    {
        const char PACKETTYPE = '8';

        private String registerNo;

        public String RegisterNo
        {
            get { return registerNo; }
            set { registerNo = value; }
        }

        private String roomNumber;

        public String RoomNumber
        {
            get { return roomNumber; }
            set { roomNumber = value.Trim(); }
        }

        public List<String> Foilos = new List<String>();

        private String balance;

        public String Balance
        {
            get { return balance; }
            set { balance = value; }
        }

        private String creditLimit;

        public String CreditLimit
        {
            get { return creditLimit; }
            set { creditLimit = value; }
        }

        public PMSRoomStatusResponse()
        {
            PacketLength = "0";
            registerNo = "0";
            roomNumber = "0";
            Foilos.Add("UNKONOWN");
            creditLimit = "1000000";
            balance = "0";
            ErrorResponse = "";
            Response = nack;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(@"\");
            sb.Append(PACKETTYPE + ",");
            sb.Append(PacketLength + ",");
            sb.Append(RegisterNo + ",");
            sb.Append(RoomNumber + ",");
            sb.Append(Balance + ",");
            sb.Append(CreditLimit + ",");
            if (ErrorResponse == "")
            {
                sb.Append(Foilos.Count.ToString() + ",");
                foreach (String Foilo in Foilos)
                {
                    sb.Append(Foilo + ",");
                }
            }
            else
            {
                sb.Append("0,");
                sb.Append(ErrorResponse);
            }
            sb.Append("\r");
            return UpdatePacketLength(sb.ToString());
        }
    }

    /* Type 3 : Charge to Room Received From MenuMate
    *  Format		3,<Length>,<RegisterNo>,<RoomNo>,<NameNo>,<Covers>,<ServerNo>,<Total>,
    * <Category>,<CatTotal>...<Category>,<
    * Purpose. Send By POS as a request to charge to a room account. 
    * <NameNo> is the index into the list as received in the type 2 message (starting from 1)
    * <Covers> Number of covers, 0 if not available
    * <ServerNo> server ID
    * <Total> is the overall total, used as a checksum.  It is followed by pairs of 
    * <Category>,<CatTotal>.  The sum of the <CatTotal>s must equal <Total>
    * <Category> is a integer identifying the category
    * <RegisterNo> may or may not contain a hyphen (ASCII character 45) followed by the docket number. It is preferable to include the docket number in this manner, as it is useful for reconciliation
    */

    class PhoenixPacketChargeToRoom : PhoenixPacketBase
    {
        const char PACKETTYPE = '3';
        const int HEADER_POSITION = 0;
        const int LENGTH_POSITION = 1;
        const int REGISTERNO_POSITION = 2;
        const int ROOMNO_POSITION = 3;
        const int FOILO_POSITION = 4;
        const int COVER_POSITION = 5;
        const int SERVER_POSITION = 6;
        const int TOTAL_POSITION = 7;
        const int CAT_POSITION = 8;

        private int foiloIndex;
        private static Logger logger = LogManager.GetCurrentClassLogger();

        public Dictionary<String, int> Catagories = new Dictionary<String, int>();
                     
        private String roomNumber;

        public String RoomNumber
        {
            get { return roomNumber; }
        }

        private String registerNumber;

        public String RegisterNo
        {
            get { return registerNumber; }
        }

        public int FoiloIndex
        {
            get { return foiloIndex; }
        }

        private String total;

        public String Total
        {
            get { return total; }
        }

        private string pmsReference;

        public string PMSReference
        {
            get { return pmsReference; }
            set { pmsReference = value; }
        }

        private String covers;

        public string Covers
        {
            get { return covers; }
        }

        private String serverno;

        public string ServerNo
        {
            get { return serverno; }
        }

        public void Parse(String Buffer)
        {
            String[] Data = Buffer.Split(',');
            String Header = ("\\" + PACKETTYPE);
            if (Data[HEADER_POSITION] != Header )
            {
                throw new Exception("Invalid Message");
            }
            
            String Length = Data[LENGTH_POSITION];
 
            roomNumber = Data[ROOMNO_POSITION];
            registerNumber = Data[REGISTERNO_POSITION];
            if (registerNumber.IndexOf("-") != -1)
            {
                String[] Temp = registerNumber.Split('-');
                if (Temp.Length > 1)
                {
                    registerNumber = Temp[0];
                    PMSReference = Temp[1];
                }
            }
            

            foiloIndex = Convert.ToInt32(Data[FOILO_POSITION]);
            covers = Data[COVER_POSITION];
            serverno = Data[SERVER_POSITION];
            total = Data[TOTAL_POSITION];

            if(Data.Length > CAT_POSITION)
            {
                for(int i = CAT_POSITION; i < Data.Length ; i +=2)
                {
                    String Code = Data[i];
                    int Amount = Convert.ToInt32(Data[i + 1]);
                    if (Catagories.ContainsKey(Code))
                    {
                        int CurrentAmount = Catagories[Code];
                        Amount += CurrentAmount;
                        Catagories[Code] = Amount;
                    }
                    else
                    {
                        Catagories.Add(Code, Amount);
                    }
                }
            }          
        }
    }

    /* Type 4 : Room Charge Response Sent to MenuMate
     * Format		4,<Len>,<RegNo>,<RoomNo>,<NameNo>,<Text> (,<Text>)
     * Purpose	Response of PMS Responses to type 3 message
     * If the room and Name (account) are available for charging, <Text> is ACK
     * Otherwise it is the reason why it can not be charged.  POS does not 
     * Interpret this reason, it is displays verbatim. (Multiple lines as per Type 2, if 
     * required)
     */

    class PMSRoomChargeResponse : PhoenixPacketBaseResponse
    {
        const char PACKETTYPE = '4';

        String registerNo;
        String roomNumber;
        private int foiloIndex;
       
        public String RoomNumber
        {
            get { return roomNumber; }
            set { roomNumber = value.Trim(); }
        }

        public String RegisterNo
        {
            get { return registerNo; }
            set { registerNo = value.Trim(); }
        }

        public int FoiloIndex
        {
            get { return foiloIndex; }
        }

        public PMSRoomChargeResponse()
        {
            PacketLength = "0";
            registerNo = "0";
            roomNumber = "0";
            foiloIndex = 0;
            Response = "NACK";
        }


        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(@"\");
            sb.Append(PACKETTYPE    + ",");
            sb.Append(PacketLength  + ",");
            sb.Append(registerNo    + ",");
            sb.Append(roomNumber    + ",");
            if (ErrorResponse == "")
            {
                sb.Append(foiloIndex + ",");
                sb.Append(Response);
            }
            else
            {
                sb.Append("0,");
                sb.Append(ErrorResponse);
            }
            sb.Append("\r");
            return UpdatePacketLength(sb.ToString());
        }

        internal void CopyFrom(PhoenixPacketChargeToRoom ChargeToRoom)
        {
            registerNo = ChargeToRoom.RegisterNo;
            roomNumber = ChargeToRoom.RoomNumber;
            foiloIndex = ChargeToRoom.FoiloIndex;
        }
    }

}
