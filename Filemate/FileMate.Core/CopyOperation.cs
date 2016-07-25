using FCE = FileMate.Core.Exceptions;
using FCSH = FileMate.Core.EventHub;

using FileMate.Core.Xml;

using Microsoft.Win32;

using NLog;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Timers;
using System.Xml;

namespace FileMate.Core
{
    public enum OperationStatus
    {
        GoOn,
        ForceToTerminate
    };

    public class CopyOperation
    {
        private bool _isExecuting = false;

        public bool IsExecuting
        {
            get
            {
                lock (this)
                {
                    return _isExecuting;
                }
            }
            set
            {
                lock (this)
                {
                    _isExecuting = value;
                }
            }
        }

        public void Cleanup()
        {
            transferInterval.Stop();
            int status = 0;
            FCSH.EventHub.CopyOperationShutdownHandler(this, null, ref status);
        }

        // source
        IFileRepository _source = null;

        public double lastTransferSize = 0;

        public IFileRepository SourceFileRepository
        {
            get { return _source; }
        }

        // targets
        List<IFileRepository> _targets = new List<IFileRepository>();

        public List<IFileRepository> Targets
        {
            get { return _targets.ToList<IFileRepository>(); }
        }

        // dated schedule, determines whether X weeks/months have passed.
        // Delegates used so we don't have to create mulitple timers, functions ...
        UInt32 dailyTriggerTime;
        public Timer transferInterval;

        // set from the ctor (overloaded) to allow an event to be fired so external assemblies can
        // call ExecuteOperation.
        private bool _executeCalledViaEvent = false;

        public bool ExecuteCalledViaEvent
        {
            get { return _executeCalledViaEvent; }

        }

        private bool _deleteSource = false;
        private string _name = null;

        public String Name
        {
            get
            {
                return _name;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="opNode"></param>
        public CopyOperation(XmlNode opNode)
        {
            _name = Helper.GetCopyOperationName(opNode);
            LogManager.GetLogger("FileMate").Info(string.Format("Build CopyOperation => {0}", _name));

            _deleteSource = Helper.GetCopyOperationDeleteSource(opNode);

            BuildSourceFileRepository(opNode);
            BuildTargetFileRepositories(opNode);

            BuildSchedule(opNode);

            int status = 0;
            FCSH.EventHub.CopyOperationCreationHandler(this, _targets, ref status);
            // transferInterval.Start();
        }

        /// <summary>
        /// Execute this operation. Exceptions are handled by calling method.
        /// </summary>
        public void ExecuteOperation()
        {
            try
            {
                int status = 0;
                FCSH.EventHub.CopyOperationCreationHandler(this, _targets, ref status);

                //::::::::::::::::::::::::::::::

                if (_executeCalledViaEvent)
                    ExecuteEvent();

                //::::::::::::::::::::::::::::::

                LogManager.GetLogger("FileMate").Trace("Entry of ExecuteOperation() ...");
                if (!IsExecuting)
                {

                    LogManager.GetLogger("FileMate").Debug("Executing operation ...");

                    bool overallResultOk = true;
                    string[] sourceFiles = SourceFileRepository.GetFileList();

                    LogManager.GetLogger("FileMate").Debug(string.Format("sourceFiles count = {0}", sourceFiles.Length));

                    status = 0;

                    if (sourceFiles.Length > 0)
                    {      // only do work if necessary
                        LogManager.GetLogger("FileMate").Debug("Found source files ...");

                        // initialise targets (for use with dynamic targets i.e., c:\target\{date}\)
                        foreach (IFileRepository t in _targets)
                        {
                            FCSH.EventHub.BackupInitiationHandler(this, t.Name, ref status);
                            t.Initialise();
                        }

                        // do the transfer
                        foreach (String file in sourceFiles)
                        {
                            string fileName = Path.GetFileName(file);
                            //FileInfo fi = new FileInfo(file);
                            //lastTransferSize = fi.Length;

                            status = 0;

                            foreach (IFileRepository t in _targets)
                            {
                                LogManager.GetLogger("FileMate").Debug(string.Format("transferring {0} to {1} ...", fileName, t.Url));
                                try
                                {
                                    if ((overallResultOk = t.TransferFile(_source.GetFileStream(fileName), fileName)))
                                    {
                                        FCSH.EventHub.BackupCompletionHandler(this, t.Name, ref status);
                                    }
                                }
                                catch (FCE.Base te)
                                {
                                    FCSH.EventHub.FailureHandler(this, new Object[2] { transferInterval, te }, ref status);
                                }

                                if (status == (int)OperationStatus.ForceToTerminate)
                                {
                                    overallResultOk = false;
                                    break;
                                }
                            }
                            if (!overallResultOk)
                            {
                                break;
                            }
                        }

                        // delete source files only if all transfers completed ok
                        if (overallResultOk)
                        {
                            LogManager.GetLogger("FileMate").Debug("deleting source ...");
                            if (_deleteSource)
                            {
                                _source.DeleteFiles();
                            }
                        }
                        else
                        {
                            LogManager.GetLogger("FileMate").Error("errors occured during transfer, processing stopped");
                        }
                    }
                    else
                    {
                        LogManager.GetLogger("FileMate").Debug("No files to transfer.");
                    }
                    IsExecuting = false;
                }

                //::::::::::::::::::::::::::::::

                FCSH.EventHub.CopyOperationShutdownHandler(this, _targets, ref status);
            }
            catch (Exception e)
            {
                LogManager.GetLogger("FileMate").FatalException("Unhandled exception while executing operation; throwing!", e);
                throw new FCE.TransferOperation(String.Format("Failure transfering file to {0}", _name), _name, e);
            }
        }

        private string getStackTrace(Exception e)
        {
            return e.StackTrace;
        }

        public delegate void ExecuteEventDelegate();
        public event ExecuteEventDelegate ExecuteEvent;

        private void BuildSourceFileRepository(XmlNode opNode)
        {
            _source = RepositoryBuilder.BuildSourceRepository(opNode);
        }

        private void BuildTargetFileRepositories(XmlNode opNode)
        {
            _targets = RepositoryBuilder.BuildTargetRepositories(opNode);
        }



        void BuildSchedule(XmlNode opNode)
        {
            String scheduleSpec =
              Helper.GetScheduleForOperation(opNode);
            int cachedLength = scheduleSpec.Length - 1;

            _executeCalledViaEvent =
              Helper.GetCalledFromForOperation(opNode);

            transferInterval = new Timer();
            transferInterval.AutoReset = true;
            transferInterval.Elapsed += scheduleCheckHandler;

            try
            {
                if (Char.IsDigit(scheduleSpec[cachedLength]))
                {
                    dailyTriggerTime = UInt32.Parse(
                        scheduleSpec.Substring(0, 2)) << 5;
                    dailyTriggerTime |= UInt32.Parse(
                        scheduleSpec.Substring(3, 2));
                }
                else transferInterval.Interval =
                  Int32.Parse(scheduleSpec.Substring(0, cachedLength));
            }
            catch (SystemException parseError)
            {
                throw new SystemException(
                  "Schedule specified in unknown format", parseError);
            }

            switch (scheduleSpec[cachedLength])
            {
                case 'M':
                    CheckWhetherScheduleIsPending = monthCheck;
                    transferInterval.Interval *= 2419200000;
                    break;
                case 'w':
                    CheckWhetherScheduleIsPending = weekCheck;
                    transferInterval.Interval *= 604800000;
                    break;
                case 'h':
                    CheckWhetherScheduleIsPending = hourCheck;
                    transferInterval.Interval *= 3600000;
                    break;
                case 'm':
                    CheckWhetherScheduleIsPending = minuteCheck;
                    transferInterval.Interval *= 60000;
                    break;
                default:
                    CheckWhetherScheduleIsPending = dayCheck;
                    transferInterval.Interval = 60000;
                    break;
            }
        }

        bool dayCheck()
        {
            return ((((UInt32)DateTime.Now.Hour) << 5) |
                (UInt32)DateTime.Now.Minute) == dailyTriggerTime;
        }

        bool hourCheck()
        {
            return getLastTransferTime().Hour != DateTime.Now.Hour;
        }

        bool minuteCheck()
        {
            return getLastTransferTime().Minute != DateTime.Now.Minute;
        }

        bool monthCheck()
        {
            return getLastTransferTime().Month != DateTime.Now.Month;
        }

        bool weekCheck()
        {
            return (getLastTransferTime().Day / 7)
                != (DateTime.Now.Day / 7);
        }

        DateTime getLastTransferTime()
        {
            DateTime lastTransferTime = DateTime.Now;
            String defaultDateTimeString = lastTransferTime.ToString();

            try
            {
                String dateTimeString = (String)Registry.GetValue(
                    @"HKEY_CURRENT_USER\SOFTWARE\IQWorks\SafeMate",
                    "LastTransferDateTime",
                    DateTime.Now.ToString());

                lastTransferTime = DateTime.Parse(
                dateTimeString == null ? defaultDateTimeString
                                       : dateTimeString);
            }
            catch
            {
            }
            finally
            {
                updateScheduleTrackingRegistryEntry();
            }

            return lastTransferTime;
        }

        void updateScheduleTrackingRegistryEntry()
        {
            Registry.SetValue(
                @"HKEY_CURRENT_USER\SOFTWARE\IQWorks\SafeMate",
                "LastTransferDateTime",
                DateTime.Now.ToString(),
                RegistryValueKind.String);
        }

        delegate bool ScheduleIsPendingPredicate();
        ScheduleIsPendingPredicate CheckWhetherScheduleIsPending;

        void scheduleCheckHandler(Object sender, EventArgs eventArgs)
        {
            try
            {
                if (!CheckWhetherScheduleIsPending())
                    return;

                transferInterval.Stop();

                //.................................

                ExecuteOperation();

                //.................................

                transferInterval.Start();
            }
            catch (Exception e)
            {
                int status = 0;
                FCSH.EventHub.FailureHandler(this, new Object[2] { transferInterval, e }, ref status);

                if (!transferInterval.Enabled)
                {
                    transferInterval.Start();
                }
            }
        }

        public override string ToString()
        {
            StringBuilder b = new StringBuilder();
            b.Append("Name = ");
            b.Append(_name);

            b.Append(", deleteSource = ");
            b.Append(_deleteSource.ToString());

            b.Append(", calledFromExternal = ");
            b.Append(_executeCalledViaEvent.ToString());

            b.Append(Environment.NewLine);

            b.AppendLine("Source;");
            b.AppendLine(_source.Url);

            b.AppendLine("Targets;");
            foreach (IFileRepository t in Targets)
            {
                b.AppendLine(t.Url);
            }

            return b.ToString();
        }
    }
}
