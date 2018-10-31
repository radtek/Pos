using System.Runtime.Serialization;
namespace MewsIntegration.Domain
{
    [DataContract]
    public class Extent
    {
        [DataMember]
        public bool Spaces { get; set; }
        [DataMember]
        public bool SpaceCategories { get; set; }
        [DataMember]
        public bool SpaceFeatures { get; set; }
        [DataMember]
        public bool Inactive { get; set; }
    }
}
