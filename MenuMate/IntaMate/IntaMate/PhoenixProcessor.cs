using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Diagnostics;
using NLog;

namespace IntaMate
{
    class PhoenixProcessor
    {
        private static Logger logger = LogManager.GetCurrentClassLogger();
        private NetworkStream NetStream;
        private PMS PMSInterface;

        public PhoenixProcessor(NetworkStream inNetStream, PMS inPMSInterface) 
        {
            NetStream = inNetStream;
            PMSInterface = inPMSInterface;
        }

        /// <summary>Starts communication with client.</summary>
        public void Process(string clientMessage) 
        {
            StreamWriter streamWriter = new StreamWriter(NetStream);
    	    try 
            {			
                // All writes will be done immediately and not cached:
                streamWriter.AutoFlush = true;

                // Start loop and handle commands:
                logger.Debug("PMS Received :" + clientMessage);
                PhoenixPacketBase PacketBase = EvaluateCommand(clientMessage);
                logger.Debug("PMS Response :" + PacketBase.ToString());
                streamWriter.WriteLine(PacketBase.ToString());
            }
            catch (Exception ex) 
            {
                PhoenixPacketNAK Response = new PhoenixPacketNAK(ex.ToString());
                streamWriter.WriteLine(Response.ToString());
                logger.Error("An Exception occured: " + ex.ToString());
                logger.Error("Raw Data Received: " + clientMessage);
            }
            finally 
            {
                streamWriter.Close();
            }
        }

        public PhoenixPacketBase EvaluateCommand(String Data) 
        {
            /* MenuMate basicly sends two commands.
             *
             * 1) Type 3, Room charge command to post money charges to rooms and check codes.
             *      Return ACK for any room charge commands on the default transaction account.
             *      This are used to test the phoenix codes and pass thoses amounts though to phoenix.
             *      You could enhance this by created a list of codes to ignore and thus filter what
             *      is sent though to Roommaster.
             *      This could further be filtered out by Total, ie when MenuMate is checking codes
             *      Its posts to those codes with a total of $0.00 if the total is $0.00 just _protocolUtility.ACK it.
             * 
             * 2) Type 7,Rooms Status commands. This is used to collect a list of the room folios.
             */

            switch (Data[1])
            {
                case '3':
                    PhoenixPacketChargeToRoom ChargeToRoom = new PhoenixPacketChargeToRoom();
                    ChargeToRoom.Parse(Data);
                    return PMSInterface.Charge(ChargeToRoom);                                          
                    break;
                case '7':
                    PMSRoomStatus ExRoomStatus = new PMSRoomStatus();
                    ExRoomStatus.Parse(Data);
                    PMSRoomStatusResponse ExRoomStatusResponse = PMSInterface.Inquiry(ExRoomStatus);
                    return ExRoomStatusResponse;
                    break;
                default:
                    throw new Exception("Invalid Message");
                    break;
            }
        }
    }
}
