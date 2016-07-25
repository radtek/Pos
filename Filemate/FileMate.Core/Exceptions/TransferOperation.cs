using System;

namespace FileMate.Core.Exceptions {
	public class TransferOperation : Base {
		public TransferOperation(String message,
		    String targetName, Exception innerException)
		        : base(message, targetName, innerException) {
		}
	};
};
