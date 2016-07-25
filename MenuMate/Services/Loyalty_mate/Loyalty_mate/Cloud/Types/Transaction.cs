using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyalty_mate.Cloud
{
    /// <summary>
    /// Represents a transaction for a member in the cloud
    /// </summary>
    [Serializable]
    public sealed class Transaction
    {
        /// <summary>
        /// Default constructor
        /// </summary>
        public 
        Transaction()
        {
        }

        /// <summary>
        /// parameterized constructor for easy instantiation
        /// </summary>
        /// <param name="uuid"> member's cloud id </param>
        /// <param name="occurred_at"> current date time where this transaction has occurred at </param>
        /// <param name="earned_points"> amount of earned points </param>
        /// <param name="earned_points_delta"> earned points adjustment </param>
        /// <param name="loaded_points"> amount of loaded points </param>
        /// <param name="loaded_points_delta"> loaded points adjustment </param>
        public
        Transaction(
            string uuid,
            string occurred_at,
            string earned_points="0.0",
            string earned_points_delta="0.0",
            string loaded_points="0.0",
            string loaded_points_delta="0.0",
            string points_type="1")
        {
            this.uuid = uuid;
            this.occurred_at = occurred_at;
            this.earned_points = earned_points;
            this.earned_points_delta = earned_points_delta;
            this.loaded_points = loaded_points;
            this.loaded_points_delta = loaded_points_delta;
            this.points_type = points_type;
        }

        /// <summary>
        /// unique id of the member this transaction belongs to
        /// </summary>
        public string uuid
        {
            get;
            set;
        }

        /// <summary>
        /// date time string where this transaction occured at
        /// </summary>
        public string occurred_at
        {
            get;
            set;
        }

        /// <summary>
        /// amount of earned points for the member
        /// </summary>
        public string earned_points
        {
            get;
            set;
        }

        /// <summary>
        /// difference between previous transaction / the adjustment
        /// </summary>
        public string earned_points_delta
        {
            get;
            set;
        }

        /// <summary>
        /// amount of loaded points for the member
        /// </summary>
        public string loaded_points
        {
            get;
            set;
        }

        /// <summary>
        /// difference between previous transaction / adjustment
        /// </summary>
        public string loaded_points_delta
        {
            get;
            set;
        }

        /// <summary>
        /// Typpe of adjustment i.e. normal(earned, redeemed or puchased), birthday reward, first visit points
        /// </summary>
        public string points_type
        {
            get;
            set;
        }
    }
}
