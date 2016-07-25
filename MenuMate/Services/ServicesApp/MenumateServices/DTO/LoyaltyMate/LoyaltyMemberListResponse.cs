using System.Collections.Generic;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyMemberListResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        MemberInfo[]    _memberInfo;


        internal LoyaltyMemberListResponse(
                        bool inSuccesful,
                        string inMessage,
                        string inDescription,
                        LoyaltyResponseCode inResponseCode,
                        MemberInfo[] inMemberInfoList)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _memberInfo   = createMemberInfoList(inMemberInfoList);
        }

        #region Public

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public MemberInfo[] MemberList
        {
            get { return _memberInfo; }
            set { _memberInfo = value; }
        }


        public static LoyaltyMemberListResponse CreateResponseNoError(MemberInfo[] inMemberInfoList)
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful, inMemberInfoList);
        }

        public static LoyaltyMemberListResponse CreateResponseError(
                                                        string inMessage,
                                                        string inDescription,
                                                        LoyaltyResponseCode inResponseCode,
                                                        MemberInfo[] inMemberInfoList)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode, inMemberInfoList);
        }


        public static LoyaltyMemberListResponse CreateResponse(
                                                        bool inSuccesful,
                                                        string inMessage,
                                                        string inDescription,
                                                        LoyaltyResponseCode inResponseCode,
                                                        MemberInfo[] inMemberInfoList)
        {
            return new LoyaltyMemberListResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode,
                                inMemberInfoList);
        }
        #endregion

        #region Private

        MemberInfo[] createMemberInfoList(MemberInfo[] inMemberInfoList)
        {
            List<MemberInfo> result = new List<MemberInfo>();

            foreach(MemberInfo info in inMemberInfoList)
            {
                result.Add(info);
            }

            return result.ToArray(); 
        }
        #endregion
    }
}
