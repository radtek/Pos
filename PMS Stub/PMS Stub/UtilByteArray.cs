
using System;
using System.Collections.Generic;
using System.Text;

namespace HDUtil
{
    public static class UtilsExtensionMethods
    {
        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }
    }

    public class UtilByteArray : List<byte>
    {
        public UtilByteArray()
        {

        }

        public void Append(byte[] data)
        {
            if (data.Length != 0)
            {
                AddRange(data);
            }
        }

        public void Append(byte[] data, int length)
        {
            if (length != 0)
            {
                byte[] subarray = data.SubArray(0, length);
                AddRange(subarray);
            }
        }

        public void Append(char[] data, int length)
        {
            if (length != 0)
            {
                byte[] subarray = System.Text.Encoding.ASCII.GetBytes(data, 0, length);
                AddRange(subarray);
            }
        }

        public void Append(String Data)
        {
            AddRange(Encoding.ASCII.GetBytes(Data));
        }

        public void AppendUTF8(String Data, bool NullTerminated)
        {
            AddRange(Encoding.UTF8.GetBytes(Data));
            if (NullTerminated)
            {
                byte Null = 0x00;
                Add(Null);
            }
        }

        public void Append(Int32 data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Append(bytes, bytes.Length);
        }

        public void Append(double data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Append(bytes, bytes.Length);
        }

        public void Append(bool data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Append(bytes, bytes.Length);
        }

        public override string ToString()
        {
            return Encoding.ASCII.GetString(this.ToArray());
        }

        public char[] ToCharArray()
        {
            return Encoding.ASCII.GetChars(this.ToArray());
        }

        public void Set(String Data)
        {
            Clear();
            AddRange(Encoding.ASCII.GetBytes(Data));
        }

        public bool IsXML()
        {
            bool retval = false;
            if (this.Count > 0)
            {
                String Data = ToString();
                int XMLIndex = Data.IndexOf("xml version=");
                if (XMLIndex == 2)
                {
                    retval = true;
                }
            }
            return retval;
        }

        public bool IsXMLCmd()
        {
            bool retval = false;
            if (this.Count > 0)
            {
                String Data = ToString();
                if (Data.IndexOf("Cmd xmlns") > 0)
                {
                    retval = true;
                }
            }
            return retval;
        }
    }
}