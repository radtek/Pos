using FC = FileMate.Core;

using SCE  = Safemate.Core.Exceptions;
using SCLP = Safemate.Core.LogProxy;

using System;
using System.Timers;
using System.Collections.Generic;

namespace Safemate.Core
{
    public class CopyOperationErrorBlock
    {
        ~CopyOperationErrorBlock()
        {
            collectedErrors.Clear();
        }

        public CopyOperationErrorBlock(
            FC.CopyOperation operation,
            Timer operationHeartbeat)
        {
            collectedErrors = new List<Exception>();
            this.operation = operation;
            this.operationHeartbeat = operationHeartbeat;
            resumeQuantum = 0;
        }

        public bool CollectError(Exception error)
        {
            collectedErrors.Add(error);
            if (collectedErrors.Count > 2) {
                String errors = "";

                foreach (Exception e in collectedErrors)
                    errors += String.Format("{0}\n", e.Message);

                SCLP.LogProxy.LogToMail(errors, String.Format(
                                            "Safemate Error Notification: Target {0} failures.", operation.Name));

                SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                  operation.Name + " has been suspended due to reoccuring errors.");
                operationHeartbeat.Stop();
                resumeQuantum = 8;
                return true;
            }

            return !(operationHeartbeat.Enabled = true);
        }

        public bool Resume()
        {
            if (resumeQuantum == 0) {
                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  operation.Name + " has been resumed. Assuming failure was transient.");
                collectedErrors.Clear();
                return operationHeartbeat.Enabled = true;
            }

            resumeQuantum--;
            return false;
        }

        public void Shutdown()
        {
            operationHeartbeat.Stop();
        }

        public List<Exception> CollectedErrors
        {
            get {
                return collectedErrors;
            }
        }

        List<Exception> collectedErrors;
        FC.CopyOperation operation;
        Timer operationHeartbeat;
        UInt32 resumeQuantum;
    };
};