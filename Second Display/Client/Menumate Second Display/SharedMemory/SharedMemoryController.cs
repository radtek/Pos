using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SharedMemory
{
    public class SharedMemoryController
    {
        internal SharedMemoryManager _sharedMemoryManager;

        SharedMemoryManagerType _sharedMemoryManagerType;

        long _serverCapacity = 0;
        long _clientCapacity = 0;

        long _serverBeginMemory = 0;
        long _clientBeginMemory = 0;

        /// <summary>
        /// 
        /// </summary>
        internal SharedMemoryController()
        {
            _sharedMemoryManager = new SharedMemoryManager();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSharedMemType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        internal bool InitServer(SharedMemoryType inSharedMemType, string inName, long inServerCapacity, long inClientCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::::::::::

            try
            {
                _serverCapacity = inServerCapacity;
                _clientCapacity = inClientCapacity; 

                _sharedMemoryManagerType = SharedMemoryManagerType.Server;
                _sharedMemoryManager.Init(_sharedMemoryManagerType, inSharedMemType, inName, TotalCapacity);
                pSetBeginMemory(TotalCapacity);
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSharedMemType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        internal bool InitClient(SharedMemoryType inSharedMemType, string inName, long inServerCapacity, long inClientCapacity)
        {
            bool result = true;

            //:::::::::::::::::::::::::::::::::::::

            try
            {
                _serverCapacity = inServerCapacity;
                _clientCapacity = inClientCapacity; 

                _sharedMemoryManagerType = SharedMemoryManagerType.Client;

                result = _sharedMemoryManager.Init(_sharedMemoryManagerType, inSharedMemType, inName, TotalCapacity);
                if (result)
                {
                    pSetBeginMemory(TotalCapacity);
                }
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::::::::::

            return result;
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public long TotalCapacity
        {
            get
            {
                return _serverCapacity + _clientCapacity;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public SharedMemoryManagerType MemoryManagerType
        {
            get
            {
                return _sharedMemoryManagerType;
            }
        }

        #endregion

        #region Public

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public byte ReadByte(long inPosition)
        {
            try
            {
                return _sharedMemoryManager.ReadByte(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadInt16(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadUInt16(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadInt32(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadUInt32(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadInt64(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadUInt64(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadSingle(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadDouble(pGetActualReadPosition(inPosition, MemoryManagerType));
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
                return _sharedMemoryManager.ReadBlock(pGetActualReadPosition(inPosition, MemoryManagerType), inCount, outBlock);
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
        public bool Write(long inPosition, byte inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, Int16 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return true;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, UInt16 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, Int32 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, UInt32 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, Int64 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, UInt64 inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, float inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool Write(long inPosition, double inValue)
        {
            try
            {
                _sharedMemoryManager.Write(pGetActualWritePosition(inPosition, MemoryManagerType), inValue);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, byte[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, Int16[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, UInt16[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, Int32[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, UInt32[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, Int64[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, UInt64[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, float[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return true;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inValue"></param>
        public bool WriteBlock(long inPosition, int inCount, double[] inBlock)
        {
            try
            {
                _sharedMemoryManager.WriteBlock(pGetActualWritePosition(inPosition, MemoryManagerType), inCount, inBlock);
                return true;
            }
            catch
            {
                return false;
            }
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCapacity"></param>
        void pSetBeginMemory(long inCapacity)
        {
            _serverBeginMemory = 0 ;
            _clientBeginMemory = _serverBeginMemory + _serverCapacity;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inMemoryManagerType"></param>
        /// <returns></returns>
        long pGetActualReadPosition(long inPosition, SharedMemoryManagerType inMemoryManagerType)
        {
            long result = 0;

            //::::::::::::::::::::::::::::::::::::::::::::::::

            if (inMemoryManagerType == SharedMemoryManagerType.Server)
            {
                result = _clientBeginMemory + inPosition;

                if (!pPosInClientMemory(result))
                {
                    throw new Exception(@"Read out of the Server memory zone"); // Read out of the Server memory zone
                }
            }
            else
            {
                result = _serverBeginMemory + inPosition;

                if (!pPosInServerMemory(result))
                {
                    throw new Exception(@"Read out of the Client memory zone"); // Read out of the Client memory zone
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <param name="inMemoryManagerType"></param>
        /// <returns></returns>
        long pGetActualWritePosition(long inPosition, SharedMemoryManagerType inMemoryManagerType)
        {
            long result = 0;

            //::::::::::::::::::::::::::::::::::::::::::::::::

            if (inMemoryManagerType == SharedMemoryManagerType.Server)
            {
                result = _serverBeginMemory + inPosition;

                if (!pPosInServerMemory(result)) 
                {
                    throw new Exception(@"Write out of the Server memory zone"); // Write out of the Server memory zone
                }
            }
            else
            {
                result = _clientBeginMemory + inPosition;

                if (!pPosInClientMemory(result))
                {
                    throw new Exception(@"Write out of the Client memory zone"); // Write out of the Client memory zone
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <returns></returns>
        bool pPosInServerMemory(long inPosition)
        {
            return (inPosition >= _serverBeginMemory) && (inPosition < (_serverBeginMemory + _serverCapacity)); 
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPosition"></param>
        /// <returns></returns>
        bool pPosInClientMemory(long inPosition)
        {
            return (inPosition >= _clientBeginMemory) && (inPosition < (_clientBeginMemory + _clientCapacity));
        }

        #endregion
    }

    /// <summary>
    /// Shared Memory Controller Builder
    /// </summary>
    public class SharedMemControllerBuilder
    {
        /// <summary>
        /// 
        /// </summary>
        public SharedMemControllerBuilder()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static SharedMemControllerBuilder Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly SharedMemControllerBuilder instance = new SharedMemControllerBuilder();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSharedMemType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildServerController(SharedMemoryType inSharedMemType, string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            result.InitServer(inSharedMemType, inName, inServerCapacity, inClientCapacity);

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSharedMemType"></param>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildClientController(SharedMemoryType inSharedMemType, string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            result.InitClient(inSharedMemType, inName, inServerCapacity, inClientCapacity);

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildServerControllerMemMappedFile(string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            result.InitServer(SharedMemoryType.File, inName, inServerCapacity, inClientCapacity);

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildClientControllerMemMappedFile(string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            result.InitClient(SharedMemoryType.File, inName, inServerCapacity, inClientCapacity);

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildServerController(string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            result.InitServer(SharedMemoryType.Memory, inName, inServerCapacity, inClientCapacity);

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inCapacity"></param>
        /// <returns></returns>
        public SharedMemoryController BuildClientController(string inName, long inServerCapacity, long inClientCapacity)
        {
            SharedMemoryController result = new SharedMemoryController();

            //:::::::::::::::::::::::::::::::::::::::::

            if (!result.InitClient(SharedMemoryType.Memory, inName, inServerCapacity, inClientCapacity))
            {
                result = null;
                //GC.Collect();
            }

            //:::::::::::::::::::::::::::::::::::::::::

            return result;
        }


    }
}
