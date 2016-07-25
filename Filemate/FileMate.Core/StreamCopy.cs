using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using FileMate.Core.Xml;
using NLog;

namespace FileMate.Core
{
    public static class StreamCopy
    {
	public static void Copy(FileMate.Core.Xml.OperationFileType inOpFileType, Stream inSourceStream, Stream inTargetStream)
        {
            switch (inOpFileType)
            {
                case OperationFileType.Binary: CopyBinary(inSourceStream, inTargetStream); break;
                case OperationFileType.ZIP:    CopyZIP(inSourceStream, inTargetStream); break;
                case OperationFileType.Text:   CopyText(inSourceStream, inTargetStream); break;
                default:
                    {
                        throw new System.Exception("Invalid Operation File Type");
                    };
            }
        }

        public static void CopyBinary(Stream inSourceStream, Stream inTargetStream)
        {
            // The buffer size is set to 2kb
            int buffLength = 2048;
            Byte[] buff;
            buff = new byte[buffLength];
            int contentLen;

            // Read from the file stream 2kb at a time
            contentLen = inSourceStream.Read(buff, 0, buffLength);

            // Till Stream content ends
            while (contentLen != 0)
            {
                // Write Content from the source stream to the target stream
                inTargetStream.Write(buff, 0, contentLen);

                contentLen = inSourceStream.Read(buff, 0, buffLength);
            }
        }

        public static void CopyZIP(Stream inSourceStream, Stream inTargetStream)
        {
            CopyBinary(inSourceStream, inTargetStream);
        }

        public static void CopyText(Stream inSourceStream, Stream inTargetStream)
        {
            StreamReader streamReader = new StreamReader(inSourceStream);

            Byte[] buffer = Encoding.UTF8.GetBytes(streamReader.ReadToEnd());
            streamReader.Close();

            inTargetStream.Write(buffer, 0, buffer.Length);
        }
    }
}
