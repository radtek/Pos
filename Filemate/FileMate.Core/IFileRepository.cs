using System.IO;

namespace FileMate.Core
{
    public interface IFileRepository
    {
        string Url { get; }
        string[] GetFileList();
        string Name { get; }
        bool AllowOverwrite { get; }

        Stream GetFileStream(string fileName);

        void CreateDirectory(string directoryName);
        void DeleteFiles();
        bool TransferFile(Stream sourceFileStream, string fileName);

        string TargetName { get; }
        void Initialise();
    }
}
