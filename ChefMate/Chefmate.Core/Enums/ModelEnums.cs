namespace Chefmate.Core.Enums
{
    public enum OrderState { Complete = 0, Incomplete, Credit, Cancel, Runner, Replacement, CallAway, Transfer }
    public enum OrderStatus { Normal = 0, Started, Hold, Bumped, Completed, Transfered, OutOfStock, Canceled }
    public enum GroupType { Course = 0, ServingCourse }

    //Setting Enums
    public enum TerminalType { Kitchen = 0, Runner }
    public enum OrderInfoDisplay { TableTabName = 0, ChitNumber }
    public enum OutputTime { Immediately = 0, OrderBumped, GroupBumped, ItemBumped }
    public enum OutputScope { Order = 0, Group, Item }
    public enum OrderLayout { TwoByFour = 0, TwoByFive, TwoBySix, TwoBySeven, TwoByTen, TwoByEleven, TwoByTwelve }
    public enum OutputType { None = 0, RunnerTerminal, KitchenTerminal, Printer }
    public enum KeypadOperation { BumpByLocation = 0, BumpByTableChitNumber }
    public enum TerminalNameType { Computer = 0, UserAdded }
    public enum NumpadMode
    {
        None,
        Numeric,
        IpAddress
    }
    public enum DatabaseMode
    {
        Correct,
        InCorrect,
        UnVersion
    }
    public enum FileType
    {
        Xml,
        Unknonw
    };
}
