using System;

namespace IntaMate
{
    /* This Factory provides the correct meesage type based on the Msg type selected.
     * This used for he different Micros implemenatations which have similar message types
     * that change slightly.
     */

    public enum e_PMSMSGType {msgNone, msgFidelio, msgHirum }; 

    public class PMSMessageFactory
    {
        private e_PMSMSGType InterfaceMsgType;

        public PMSMessageFactory()
        {
            setMsgType((e_PMSMSGType)Enum.Parse(typeof(e_PMSMSGType), IntaMate.Properties.Settings.Default.InterfaceMsgType));
        }

        public void setMsgType(e_PMSMSGType inType)
        {
            InterfaceMsgType = inType;
        }

        /* The Mircos message format shouldn't really be the return type here.
         * It should be a format based on "PMSMessageAccCharge" but since 
         * Mircos is all we are dealing with at this stage YAGNI
         */ 
         
        public PMSMessageAccCharge MessageAccCharge()
        {
            switch (InterfaceMsgType)
            {
                case e_PMSMSGType.msgNone :
                    throw new Exception("Interface Not Specified.");
                break;
                case e_PMSMSGType.msgFidelio:
                return new PMSMessageFidelioAccCharge();
                break;
                case e_PMSMSGType.msgHirum:
                return new PMSMessageHirumAccCharge();
                default:
                    throw new Exception("Interface Not Specified.");
            }
        }

        /* The Mircos message format shouldn't really be the return type here.
         * It should be a format based on "PMSMessageInquiry" but since 
         * Mircos is all we are dealing with at this stage YAGNI
         */ 
        public PMSMessageMicrosInquiry MessageInquiry()
        {
            switch (InterfaceMsgType)
            {
                case e_PMSMSGType.msgNone:
                    throw new Exception("Interface Not Specified.");
                    break;
                case e_PMSMSGType.msgFidelio:
                case e_PMSMSGType.msgHirum:
                    return new PMSMessageMicrosInquiry();
                default:
                    throw new Exception("Interface Not Specified.");
            }
        }
    }
}