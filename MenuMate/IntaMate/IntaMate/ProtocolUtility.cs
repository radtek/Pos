using System;
using System.Collections.Generic;
using System.Text;

namespace IntaMate
{
    public class ProtocolUtility
    {
        public const byte SOT = (byte)1;
        public const byte STX = (byte)2;
        public const byte ETX = (byte)3;
        public const byte EOT = (byte)4;
        public const byte ENQ = (byte)5;
        public const byte ACK = (byte)6;
        public const byte NAK = (byte)21;

        public String GetBCC(String block)
        {
            int bcc = 0;
            foreach (char c in block.ToCharArray())
            {
                bcc += c;
            }
            return String.Format("{0:X}", bcc & 0xFFFF);
        }

        //protected String _protocolUtility.GetBCC(List<byte> block)
        public String GetBCC(byte[] block)
        {
            int bcc = 0;
            foreach (byte c in block)
            {
                bcc += c;
            }
            return String.Format("{0:X}", bcc & 0xFFFF);
        }
    }  
}
