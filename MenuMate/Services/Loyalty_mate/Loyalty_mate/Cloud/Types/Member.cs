using System;
using Loyalty_mate.Cloud.Types;

namespace Loyalty_mate.Cloud
{

    /// <summary>
    ///   Represents a member definition in the cloud.
    /// </summary>

    [Serializable]
    public sealed class Member
    {
        public
        Member()
        {
        }

       /// <summary>
        ///   Initializing constructor.
        /// </summary>
        /// <param name="account">
        ///   The member's account.
        /// </param>
        /// <param name="profile">
        ///   The member's profile.
        /// </param>
        /// <param name="uuid">
        ///   The UUID identifying the member.
        /// </param>
        /// <param name="activation_token">
        ///   The member's activation token; used later to make the
        ///   member permanent.
        /// </param>
        /// <param name="activated">
        ///   Specifies whether the member's been activated; I.e. made permanent.
        /// </param>
        /// <param name="earned_points">
        ///   How many points the member's earned.
        /// </param>
        /// <param name="loaded_points">
        ///   How many points the member's purchased.
        /// </param>
        /// <param name="member_type"></param>
        /// <param name="current_year_point"></param>
        /// <param name="previous_year_point"></param>
        /// <param name="available_birthday_point"></param>
        /// <param name="available_firstvisit_point"></param>
        public
        Member(Account account,
               Profile profile,
               MiscInfo miscInfo,
               string uuid = null,
               string activation_token = "",
               bool activated = false,
               string earned_points = "0.0",
               string loaded_points = "0.0",
               string member_type="0",
               string current_year_point= "0.0",
               string previous_year_point= "0.0",
               string available_birthday_point = "0.0",
               string available_firstvisit_point = "0.0")
        {
            if (uuid == null)
                uuid = Guid.NewGuid().ToString();

            this.account = account;
            this.profile = profile;
            this.member_misc_info = miscInfo;
            this.uuid = uuid;
            this.activation_token = activation_token;
            this.activated = activated;
            this.earned_points = earned_points;
            this.loaded_points = loaded_points;
            this.member_type = member_type;
            this.current_year_point = current_year_point;
            this.previous_year_point = previous_year_point;
            this.available_birthday_point = available_birthday_point;
            this.available_firstvisit_point = available_firstvisit_point;
        }

        /// <summary>
        ///   UUID identifying the member.
        /// </summary>
        public string uuid
        {
            get;
            set;
        }

        /// <summary>
        ///   Whether the member's been activated; I.e. made permanent.
        /// </summary>
        public bool activated
        {
            get;
            set;
        }

        /// <summary>
        ///   How many points the member's earned.
        /// </summary>
        public string earned_points
        {
            get;
            set;
        }

        /// <summary>
        ///   How many points the member's purchased.
        /// </summary>
        public string loaded_points
        {
            get;
            set;
        }

        /// <summary>
        ///   The token to be used to activate the member. I.e. make
        ///   the member permanent.
        /// </summary>
        public string activation_token
        {
            get;
            set;
        }

        public string member_type
        {
            get;
            set;
        }

        /// <summary>
        ///   The member's account.
        /// </summary>
        public Account account
        {
            get;
            set;
        }
        
        /// <summary>
        ///   The member's profile.
        /// </summary>
        public Profile profile
        {
            get;
            set;
        }

        /// <summary>
        ///   The member's miscellaneous info.
        /// </summary>
        public MiscInfo member_misc_info
        {
            get;
            set;
        }

        public string current_year_point
        {
            get;
            set;
        }

        public string previous_year_point
        {
            get;
            set;
        }

        public string available_birthday_point
        {
            get;
            set;
        }

        public string available_firstvisit_point
        {
            get;
            set;
        }

    } /* Member */

} /* Loyalty_mate.Cloud */
