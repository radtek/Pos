using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using SharedMemory;

using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace SMInterProcessCommunication
{
    public class SMInterProcessComm 
    {
        protected Int64 _initPosition;

        protected SharedMemoryController _sharedMemoryController;

        protected bool _active = false;

        /// <summary>
        /// 
        /// </summary>
	    public SMInterProcessComm()
        {
            _initPosition = sizeof(Int32); // Right after the HWNDHandle
        }

        /// <summary>
        /// 
        /// </summary>
	    ~SMInterProcessComm()
        {
            _sharedMemoryController = null;
        }

        #region Properties

        public bool Active
        {
            get
            {
                return _active;
            }
        }

        #endregion

        /// <summary>
        ///     
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inServerCapacity"></param>
        /// <param name="inClientCapacity"></param>
	    public virtual bool Init(IntPtr inHandle, string inName, Int64 inServerCapacity, Int64 inClientCapacity )
        {
            bool result = true;

            //::::::::::::::::::::::::::::::::::::::::::::

            if (_sharedMemoryController == null)
            {
                try
                {
                    _sharedMemoryController = createSharedMemoryController(inName, inServerCapacity, inClientCapacity);

                    if (_sharedMemoryController != null)
                    {
                        publishHWNDHandle(inHandle);
                        _active = true;
                    }
                    else
                    {
                        result = false; 
                    }
                }
                catch
                {
                    result = false; 
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// Disposes of both the Shared Memory and the MessageWindows.Instance. 
        /// </summary>
        public void Done()
        {
 	        _sharedMemoryController = null;

            _active = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHandle"></param>
        void publishHWNDHandle(IntPtr inHandle)
        {
            _sharedMemoryController.Write(0, (Int32)inHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inServerCapacity"></param>
        /// <param name="inClientCapacity"></param>
        /// <returns></returns>
	    internal virtual SharedMemoryController createSharedMemoryController( string inName, Int64 inServerCapacity, Int64 inClientCapacity )
        {
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inMsg"></param>
        /// <returns></returns>
        virtual public bool processMessage(ref Message inMsg)
        {
            return false;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class SMInterProcessCommServer : SMInterProcessComm
    {
        /// <summary>
        /// 
        /// </summary>
        public SMInterProcessCommServer()
        {
        }

        /// <summary>
        /// 
        /// </summary>
	    ~SMInterProcessCommServer()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        protected IntPtr ClientWMsgHandle
        { 
            get
            {
                return ( IntPtr )_sharedMemoryController.ReadInt32( 0 );
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inServerCapacity"></param>
        /// <param name="inClientCapacity"></param>
        /// <returns></returns>
	    internal override SharedMemoryController createSharedMemoryController( string inName, Int64 inServerCapacity, Int64 inClientCapacity )
        {
            return SharedMemControllerBuilder.Instance.BuildServerController( inName, inServerCapacity, inClientCapacity );
        }
    }

    public class SMInterProcessCommClient : SMInterProcessComm
    {
        /// <summary>
        /// 
        /// </summary>
        public SMInterProcessCommClient()
        {
        }

        /// <summary>
        /// 
        /// </summary>
	    ~SMInterProcessCommClient()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        protected IntPtr ServerWMsgHandle
        { 
            get
            {
                try
                {
                    return (IntPtr)_sharedMemoryController.ReadInt32(0);
                }
                catch
                {
                    return IntPtr.Zero;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inName"></param>
        /// <param name="inServerCapacity"></param>
        /// <param name="inClientCapacity"></param>
        /// <returns></returns>
        internal override SharedMemoryController createSharedMemoryController(string inName, Int64 inServerCapacity, Int64 inClientCapacity)
        {
            return SharedMemControllerBuilder.Instance.BuildClientController( inName, inServerCapacity, inClientCapacity );
        }
    }
}
