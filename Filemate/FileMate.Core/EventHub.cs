using System;

namespace FileMate.Core.EventHub {
    public delegate void EhEventHandler(Object sender, Object data, ref int status); // status will tell the CopyGroup to either go on or inmidiately finish the execution
                                                                                     // 0 => go on   1 => stop
	public enum EhHandlerType {
		BackupCompletion,
		BackupInitiation,
		CopyOperationCreation,
		CopyOperationShutdown,
		Failure,
		NumberOfHandlerTypes
	};

	public static class EventHub {
		public static void AddHandler(EhHandlerType handlerType,
		    EhEventHandler handler) {
			handlers[(int)handlerType] = handler;
		}

		static EventHub() {
			handlers = new EhEventHandler[
			    (int)EhHandlerType.NumberOfHandlerTypes]{
				EhSentinel, EhSentinel, EhSentinel, EhSentinel, EhSentinel
			};
		}

		public static void RemoveHandler(
		    EhHandlerType handlerType) {
			handlers[(int)handlerType] = EhSentinel;
		}

        static void EhSentinel(Object sender, Object data, ref int status)
        {
		}

		static public EhEventHandler BackupCompletionHandler {
			get {
				return handlers[
				    (int)EhHandlerType.BackupCompletion];
			}
			set {
				handlers[
				    (int)EhHandlerType.BackupCompletion] =
				        value;
			}
		}

		static public EhEventHandler BackupInitiationHandler {
			get {
				return handlers[
				    (int)EhHandlerType.BackupInitiation];
			}
			set {
				handlers[
				    (int)EhHandlerType.BackupInitiation] =
				        value;
			}
		}

		static public EhEventHandler CopyOperationCreationHandler {
			get {
				return handlers[
				    (int)EhHandlerType.CopyOperationCreation];
			}
			set {
				handlers[
				    (int)EhHandlerType.CopyOperationCreation] =
				        value;
			}
		}

		static public EhEventHandler FailureHandler {
			get {
				return handlers[
				    (int)EhHandlerType.Failure];
			}
			set {
				handlers[
				    (int)EhHandlerType.Failure] =
				        value;
			}
		}

		static public EhEventHandler CopyOperationShutdownHandler {
			get {
				return handlers[
				    (int)EhHandlerType.CopyOperationShutdown];
			}
			set {
				handlers[
				    (int)EhHandlerType.CopyOperationShutdown] =
				        value;
			}
		}

		static EhEventHandler[] handlers;
	}
};
