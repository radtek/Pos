using System;
using System.Collections.Generic;
using Loyalty_mate.Cloud.Types;
using Loyalty_mate.Interfaces;
using Loyalty_mate.Types;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Cloud.Types.Requests;
using Loyalty_mate.Cloud.Exceptions;
using Loyalty_mate.Cloud.Types.Requests.ActivationTokens;
using System.IO;
using System.Xml;

namespace Loyalty_mate.Cloud
{

    /// <summary>
    ///   The cloud interface to the Loyalty mate service.
    /// </summary>
    public sealed class Loyalty_mate_service
        : I_Loyalty_mate_service<Member>,
          I_Loyalty_mate_service_transactions<Transaction>,
          I_Loyalty_mate_service_activation_tokens, I_Loyalty_mate_service_tiers<Tier>, I_Loyalty_mate_service_points<PointsInfo>
    {
        /// <summary>
        ///   Creates an interface that can be used to access cloud functionality.
        /// </summary>
        /// <param name="credentials">
        ///   The basic credentials identifying the peer.
        /// </param>
        /// <param name="immediately_authenticate">
        ///   Whether the service should authenticate on boot.
        /// </param>
        public
        Loyalty_mate_service(Credentials credentials,
                             bool immediately_authenticate = true)
        {
            credentials_ = credentials;
            base_request_address_ =
                String.Join(".",
                            credentials_.Site_name,
                            Request_addresses.Host);

            if (immediately_authenticate)
                Ensure_we_are_authenticated();
        }

        /// <summary>
        ///   Creates a member in the cloud.
        /// </summary>
        /// <param name="member">
        ///   The details to use when creating the member.
        /// </param>
        /// <returns>
        ///   The member created.
        /// </returns>
        public Member
        Create_member(Member member)
        {
            Ensure_we_are_authenticated();

            Create_member_request request =
                new Create_member_request(base_request_address_,
                                          member,
                                          token_);
            request.Execute();
            return request.Member;
        }

        /// <summary>
        ///   Updates the details of an existing member in the cloud.
        /// </summary>
        /// <param name="member">
        ///   The new details of the member. The UUID of the member must be valid.
        /// </param>
        public void
        Update_member_details(Member member)
        {
            Ensure_we_are_authenticated();

            Update_member_request request =
                new Update_member_request(base_request_address_,
                                          member,
                                          token_);

            request.Execute();
        }

        /// <summary>
        ///   Deletes a member in the cloud.
        /// </summary>
        /// <param name="uuid">
        ///   The UUID identifying the member.
        /// </param>
        public void
        Delete_member(string uuid)
        {
            Ensure_we_are_authenticated();

            Delete_member_request request =
                new Delete_member_request(base_request_address_,
                                          uuid,
                                          token_);

            request.Execute();
        }

        /// <summary>
        ///   Checks to see whether a member with a given UUID exists.
        /// </summary>
        /// <param name="uuid">
        ///   The UUID identifying a member.
        /// </param>
        /// <returns>
        ///   True if the member exists, false otherwise.
        /// </returns>
        public bool
        Does_member_exist(string uuid)
        {
            try
            {
                Get_member_details(uuid);
                return true;
            }
            catch (Member_doesnt_exist_exception)
            {
                return false;
            }
        }

        /// <summary>
        ///   Returns the details for a specific member.
        /// </summary>
        /// <param name="uuid">
        ///   The UUID identifying a particular member.
        /// </param>
        /// <returns>
        ///   That member's details.
        /// </returns>
        public Member
        Get_member_details(string uuid)
        {
            Ensure_we_are_authenticated();

            Get_member_details_request request =
                new Get_member_details_request(base_request_address_,
                                               uuid,
                                               token_);
            request.Execute();
            return request.Details;
        }

        public Member Get_member_by_code(string memberCode, string siteId)
        {
            Ensure_we_are_authenticated();

            Get_member_by_code request =
                new Get_member_by_code(base_request_address_,
                                               memberCode,
                                               siteId,
                                               token_);
            request.Execute();
            return request.Details;
        }




        /// <summary>
        ///   Returns the details of all members in the cloud.
        /// </summary>
        /// <returns>
        ///   A dictionary of member details keyed on member UUID.
        /// </returns>
        public Dictionary<string, Member>
        Get_all_member_details()
        {
            Ensure_we_are_authenticated();

            Get_all_member_details_request request =
                new Get_all_member_details_request(base_request_address_,
                                                   token_);
            request.Execute();
            return request.All_member_details;
        }

        /// <summary>
        /// Uploades transaction details to the cloud
        /// </summary>
        /// <param name="transaction"> transaction to be posted </param>
        public void
        Post_transaction(Transaction transaction)
        {
            Ensure_we_are_authenticated();
            Post_transaction_request request =
                new Post_transaction_request(base_request_address_,
                                             transaction,
                                             token_);
            request.Execute();
        }

        /// <summary>
        /// Get member uuid given the activation token
        /// </summary>
        /// <param name="activationToken"></param>
        /// <returns></returns>
        public MemberActivationInfo
        Get_member_by_token(string activationToken)
        {
            Ensure_we_are_authenticated();

            Get_member_id_by_activation_token_request request =
                new Get_member_id_by_activation_token_request(
                                            base_request_address_,
                                            activationToken,
                                            token_);

            request.Execute();
            return request.TokenResponse;
        }

        //TODO: remove method. No longer used 
        private MemberActivationInfo
        getMockMemberActivationInfo(string activationToken)
        {
            string tokenTextFileName = "tokens.txt";
            string tokenGuidMapFileName = "token_guid_map.txt";
            TextWriter tw = File.CreateText(tokenTextFileName);
            tw.WriteLine(
                string.Format(
                    "Token: {0} Received: {1}",
                    activationToken,
                    DateTime.Now.ToString("dd-MM-yy hh:mm:ss tt")));
            tw.Close();

            MemberActivationInfo info = new MemberActivationInfo();
            XmlDocument doc = new XmlDocument();
            if (File.Exists(tokenGuidMapFileName))
            {
                doc.Load(tokenGuidMapFileName);
                XmlNode root = doc.DocumentElement;
                string xpathExpr = string.Format("token[@value='{0}']", activationToken);
                XmlNode tokenNode = root.SelectSingleNode(xpathExpr);
                if (tokenNode != null)
                {
                    info.value = activationToken;
                    info.uuid = tokenNode.Attributes["uuid"].InnerText;
                }
            }

            return info;
        }


        /// <summary>
        ///   Makes sure we're authenticated with the cloud. If we're not,
        ///   it authenticates us. If our credentials have expired, it
        ///   refreshes them.
        /// </summary>
        void
        Ensure_we_are_authenticated()
        {
            if (token_ == null || token_.Has_expired())
            {
                Authenticate_request request =
                    new Authenticate_request(base_request_address_,
                                             credentials_,
                                             token_);
                request.Execute();
                token_ = request.Token;
            }
        }

        /// <summary>
        ///   The base address to be used when forming all request addresses.
        ///   I.e. the address without any path, query or fragment information
        ///   appended.
        /// </summary>
        readonly string base_request_address_;

        /// <summary>
        ///   The basic credentials to be used when creating or refreshing a
        ///   security token.
        /// </summary>
        Credentials credentials_;

        /// <summary>
        ///   The security token to be used when accessing the cloud.
        /// </summary>
        Token token_ = null;

        public Tier Create_tier(Tier tier)
        {
            Ensure_we_are_authenticated();
            Create_Tier_Request request = new Create_Tier_Request(base_request_address_, tier, token_);
            request.Execute();
            return request.CreatedTier;
        }

        public void Delete_tier(Tier tier)
        {
            Ensure_we_are_authenticated();
            Delete_Tier_Request request = new Delete_Tier_Request(base_request_address_, tier, token_);
            request.Execute();
        }

        public void Update_tier_details(Tier tier)
        {
            Ensure_we_are_authenticated();
            Update_Tier_Request request = new Update_Tier_Request(base_request_address_, tier, token_);
            request.Execute();
        }

        public Tier Get_tier_details(string tierid)
        {
            Ensure_we_are_authenticated();
            Get_tier_details_request request = new Get_tier_details_request(base_request_address_, tierid, token_);
            request.Execute();
            return request.TierDetails;
        }


        public bool Does_tier_exist(string tierid)
        {
            try
            {
                Get_tier_details(tierid);
                return true;
            }
            catch (Tier_doesnt_exist_exception)
            {
                return false;
            }
        }

        public PointsInfo GetPointsInRange(PointsInfo pointsDetail)
        {
            Ensure_we_are_authenticated();
            Get_Points_Detail_Request request = new Get_Points_Detail_Request(base_request_address_, pointsDetail, token_);
            request.Execute();
            return request.PointDetails;
        }
    }

} /* Loyalty_mate.Cloud */
