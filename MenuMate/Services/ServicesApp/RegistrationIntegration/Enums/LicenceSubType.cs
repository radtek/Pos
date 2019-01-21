namespace RegistrationIntegration.Enums
{
    public enum Eftpos
    {
        Eftposnz = 1,
        Provenco,
        IngenicoandPceftposAustralia,
        CadmusKeylinkOneWay,
        CadmusCronos,
        IceLinkIce5000Hyperlcom,
        DpsPaymentExpress,
        EftposSmartpay,
        EftposSmartConnect,
        EftposAdyen,
        EftposPaymentSense

    }

    public enum Loyalty
    {
        MenumateLoyaltyLocal = 1,
        MenumateLoyaltyLocalSubscription,
        MenumateLoyaltyMateWeb,
        MenuMateClubMembership,
        EBetGamingMembership,
        CasinoExternalMembership
    }

    public enum Accounting
    {
        Xero = 1,
        Myob,
        Peachtree,
    }

    public enum PropertyManagement
    {
        MotelMate = 1,
        Sihot,
        Oracle,
        Mews
    }

    public enum Rooms
    {
        Strait = 1,
        NewBook,
    }

    public enum Fiscal
    {
        PosPlus = 1,
        FiscalPrinter,
        AustriaPrinter,
    }
}