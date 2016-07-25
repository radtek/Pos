using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;


namespace SharedMemory
{
    public enum SharedMemoryManagerType
    {
        Server, Client
    }

    public enum SharedMemoryType
    {
        Memory, File
    }

    public class SharedMemoryManager
    {
        MemoryMappedFile _sharedMemory;
        MemoryMappedViewAccessor _sharedMemoryAccesor;

        /// <summary>
        /// 
        /// </summary>
        public SharedMemoryManager()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inType"></param>
        /// <returns></returns>
        public bool Init(SharedMemoryManagerType inType, SharedMemoryType inShareType, string inName, long inCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            result = (inType == SharedMemoryManagerType.Server) ? pInitServer(inShareType, inName, inCapacity) : pInitClient(inShareType, inName, inCapacity);
  
            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public byte ReadByte(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadByte(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Int16 ReadInt16(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadInt16(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public UInt16 ReadUInt16(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadUInt16(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Int32 ReadInt32(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadInt32(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public UInt32 ReadUInt32(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadUInt32(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Int64 ReadInt64(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadInt64(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public UInt64 ReadUInt64(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadUInt64(inPosition);
            }
            catch
            {
                return 0;
            }
        }

                /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public float ReadSingle(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadSingle(inPosition);
            }
            catch 
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public double ReadDouble(long inPosition)
        {
            try
            {
                return _sharedMemoryAccesor.ReadDouble(inPosition);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, byte[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<byte>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, Int16[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<Int16>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, UInt16[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<UInt16>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, Int32[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<Int32>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, UInt32[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<UInt32>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, Int64[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<Int64>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, UInt64[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<UInt64>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, float[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<float>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int ReadBlock(long inPosition, int inCount, double[] outBlock)
        {
            try
            {
                return _sharedMemoryAccesor.ReadArray<double>(inPosition, outBlock, 0, inCount);
            }
            catch
            {
                return 0;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, byte inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, Int16 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, UInt16 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, Int32 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, UInt32 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, Int64 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, UInt64 inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, float inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void Write(long inPosition, double inValue)
        {
            try
            {
                _sharedMemoryAccesor.Write(inPosition, inValue);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, byte[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<byte>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, Int16[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<Int16>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, UInt16[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<UInt16>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, Int32[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<Int32>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, UInt32[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<UInt32>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, Int64[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<Int64>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, UInt64[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<UInt64>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, float[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<float>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public void WriteBlock(long inPosition, int inCount, double[] inBlock)
        {
            try
            {
                _sharedMemoryAccesor.WriteArray<double>(inPosition, inBlock, 0, inCount);
            }
            catch
            {
            }
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inShareType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        bool pInitServer(SharedMemoryType inShareType, string inName, long inCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            result = (inShareType == SharedMemoryType.File) ? pInitServerSharedMemMappedFile(inName, inCapacity) : pInitServerSharedMem(inName, inCapacity);

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inShareType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        bool pInitClient(SharedMemoryType inShareType, string inName, long inCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            result = (inShareType == SharedMemoryType.File) ? pInitClientSharedMemMappedFile(inName) : pInitClientSharedMem(inName);

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        bool pInitServerSharedMemMappedFile(string inName, long inCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        bool pInitServerSharedMem(string inName, long inCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            try
            {
                _sharedMemory = MemoryMappedFile.CreateOrOpen(inName, inCapacity);
                _sharedMemoryAccesor = _sharedMemory.CreateViewAccessor();
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <returns></returns>
        bool pInitClientSharedMemMappedFile(string inName)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            try
            {
                _sharedMemory = MemoryMappedFile.OpenExisting(inName);
                _sharedMemoryAccesor = _sharedMemory.CreateViewAccessor();
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <returns></returns>
        bool pInitClientSharedMem(string inName)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::

            try
            {
                _sharedMemory = MemoryMappedFile.OpenExisting(inName);
                _sharedMemoryAccesor = _sharedMemory.CreateViewAccessor();
            }
            catch(Exception ex)
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::

            return result;
        }

        #endregion
    }
}
