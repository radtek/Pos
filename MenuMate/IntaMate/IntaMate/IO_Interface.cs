using System;

namespace IntaMate
{

    interface I_IO_Interface
    {
        void DiscardInBuffer();
        void Write(byte[] Data, int Index, int ByteCount);
        byte ReadByte();
        int Read(byte[] Data, int Index, int ByteCount);
        void Close();
        bool Connected();
    }
}