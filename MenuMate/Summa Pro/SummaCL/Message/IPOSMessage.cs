using System.Xml;

namespace SummaCL
{
    public interface IPOSMessage {
        void Process(XmlDocument document);
    }
}
