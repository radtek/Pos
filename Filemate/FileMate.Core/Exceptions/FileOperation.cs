using System;

namespace FileMate.Core.Exceptions {
	public class FileOperation : Base  {
		public FileOperation(String message,
		    String targetName, Exception innerException)
		        : base(message, targetName, innerException) {
		}
	};
};
