using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for the order struct used by ChefMate to store and retrieve order in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateFullOrder
    {
        #region PUBLIC
        public void CopyTo(DTO_ChefMateFullOrder outResult)
        {
            outResult.OrderDbKey                = OrderDbKey;
            outResult.OrderNumber               = OrderNumber;
            outResult.OrderPosDbKey             = OrderPosDbKey;
            outResult.PatronCount               = PatronCount;
            outResult.ChitValue                 = ChitValue;
            outResult.TableTabName              = TableTabName;
            outResult.PartyName                 = PartyName;
            outResult.ServerName                = ServerName;
            outResult.CustomerName              = CustomerName;
            outResult.OrderType                 = OrderType;
            outResult.SaleStartTime             = SaleStartTime;
            outResult.SaleFinishTime            = SaleFinishTime;
            outResult.CourseKey                 = CourseKey;
            outResult.CourseName                = CourseName;
            outResult.CoursePosKey              = CoursePosKey;
            outResult.CourseDisplayOrder        = CourseDisplayOrder;
            outResult.CourseCalledAway          = CourseCalledAway;
            outResult.CourseBumpTime            = CourseBumpTime;
            outResult.ServingCourseKey          = ServingCourseKey;
            outResult.ServingCourseName         = ServingCourseName;
            outResult.ServingCoursePosKey       = ServingCoursePosKey;
            outResult.ServingCourseDisplayOrder = ServingCourseDisplayOrder;
            outResult.ServingCourseCalledAway   = ServingCourseCalledAway;
            outResult.ServingCourseBumpTime     = ServingCourseBumpTime;
            outResult.OrderItemKey              = OrderItemKey;
            outResult.ItemPosDbKey              = ItemPosDbKey;
            outResult.OrderItemPosDbKey         = OrderItemPosDbKey;
            outResult.ItemName                  = ItemName;
            outResult.OrderItemStatus           = OrderItemStatus;
            outResult.OrderItemNote             = OrderItemNote;
            outResult.OrderItemArrivalTime      = OrderItemArrivalTime;
            outResult.OrderItemOptionKey        = OrderItemOptionKey;
            outResult.OptionName                = OptionName;
            outResult.OrderItemOptionIsPlus     = OrderItemOptionIsPlus;
            outResult.OrderItemSideKey          = OrderItemSideKey;
            outResult.OrderItemSidePosKey       = OrderItemSidePosKey;
            outResult.SideName                  = SideName;
            outResult.OrderItemSideIsCanceled   = OrderItemSideIsCanceled;
            outResult.DeliveryTime              = DeliveryTime;
        }

        [DataMember]
        public int OrderDbKey { get; set; }

        [DataMember]
        public int OrderNumber { get; set; }

        [DataMember]
        public int OrderPosDbKey { get; set; }

        [DataMember]
        public int PatronCount { get; set; }

        [DataMember]
        public string ChitValue { get; set; }

        [DataMember]
        public string TableTabName { get; set; }

        [DataMember]
        public string PartyName { get; set; }

        [DataMember]
        public string ServerName { get; set; }

        [DataMember]
        public string CustomerName { get; set; }

        [DataMember]
        public string OrderType { get; set; }

        [DataMember]
        public DateTime SaleStartTime { get; set; }

        [DataMember]
        public DateTime SaleFinishTime { get; set; }

        [DataMember]
        public int CourseKey { get; set; }

        [DataMember]
        public int CoursePosKey { get; set; }

        [DataMember]
        public string CourseName { get; set; }

        [DataMember]
        public int CourseDisplayOrder { get; set; }

        [DataMember]
        public bool CourseCalledAway { get; set; }

        [DataMember]
        public DateTime CourseBumpTime { get; set; }

        [DataMember]
        public int ServingCourseKey { get; set; }

        [DataMember]
        public int ServingCoursePosKey { get; set; }

        [DataMember]
        public string ServingCourseName { get; set; }

        [DataMember]
        public int ServingCourseDisplayOrder { get; set; }

        [DataMember]
        public bool ServingCourseCalledAway { get; set; }

        [DataMember]
        public DateTime ServingCourseBumpTime { get; set; }

        [DataMember]
        public int OrderItemKey { get; set; }

        [DataMember]
        public int ItemPosDbKey { get; set; }

        [DataMember]
        public int OrderItemPosDbKey { get; set; }

        [DataMember]
        public string ItemName { get; set; }

        [DataMember]
        public string OrderItemStatus { get; set; }

        [DataMember]
        public string OrderItemNote { get; set; }

        [DataMember]
        public DateTime OrderItemArrivalTime { get; set; }

        [DataMember]
        public int OrderItemOptionKey { get; set; }

        [DataMember]
        public string OptionName { get; set; }

        [DataMember]
        public bool OrderItemOptionIsPlus { get; set; }

        [DataMember]
        public int OrderItemSideKey { get; set; }

        [DataMember]
        public int OrderItemSidePosKey { get; set; }

        [DataMember]
        public string SideName { get; set; }

        [DataMember]
        public bool OrderItemSideIsCanceled { get; set; }

        [DataMember]
        public DateTime DeliveryTime { get; set; }

       #endregion
    }
}
