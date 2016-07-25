using System.Collections.Generic;
using System.Xml;
using FileMate.Core.Xml;
using NLog;
using System.Reflection;
using System;

namespace FileMate.Core
{
    public class CopyGroup
    {

        #region Private Data
        private XmlDocument _settings = null;
        private List<CopyOperation> _operations = new List<CopyOperation>();


        #endregion

        public List<CopyOperation> CopyOperations
        {
            get { return _operations; }
            set { _operations = value; }
        }

        #region Ctors

        /// <summary>
        /// </summary>
        /// <param name="settings"></param>
        public CopyGroup(XmlDocument settings)
        {
            _settings = settings;
            LogManager.GetLogger("FileMate").Info(string.Format("FileMate v{0}",
                Assembly.GetExecutingAssembly().GetName().Version.ToString()));

            BuildOps();
        }

        public void Cleanup()
        {
            foreach (CopyOperation op in _operations)
                op.Cleanup();
        }

        #endregion

        #region Public Methods
        public void ExecuteCopyOperations()
        {
            try
            {
                // executing operations
                foreach (CopyOperation op in _operations)
                {
                    op.ExecuteOperation();
                }
            }
            catch (Exception se)
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while executing operations", se);
            }
        }

        public void StartCopyOperationsSchedules()
        {
            try
            {
                // starting schedules
                foreach (CopyOperation op in _operations)
                {
                    if (op.transferInterval != null)
                        op.transferInterval.Start();
                }
            }
            catch (Exception se)
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while starting operation schedules", se);
            }
        }

        public void ReInitiateCopyOperations(XmlDocument settings)
        {
            try
            {
                _settings = settings;

                // stops current oprations
                foreach (CopyOperation op in _operations)
                {
                    if (op.transferInterval != null)
                        op.transferInterval.Stop();
                }

                // re-loading operations
                _operations.Clear();
                BuildOps();
            }
            catch (Exception se)
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while re-initiating copy operations", se);
            }
        }

        #endregion

        #region Private Methods
        private void BuildOps()
        {
            XmlNodeList opsNodes = Helper.GetCopyOperationNodes(_settings);
            foreach (XmlNode opNode in opsNodes)
            {
                BuildCopyOperation(opNode);
            }
        }

        private void BuildCopyOperation(XmlNode opNode)
        {
            if (opNode.Attributes["enabled"].Value == @"true")
            {
                CopyOperation o = new CopyOperation(opNode);
                _operations.Add(o);
                LogManager.GetLogger("FileMate").Info(o.ToString());
            }
        }

        #endregion
    }
}
