namespace RegistrationIntegration.Sevices
{
    public class RequestAddress
    {
        //public const string BaseAddress = "http://mmregistrations.azurewebsites.net/";
       public const string BaseAddress = "http://mmregistrations-stg-mmregistrations.azurewebsites.net/";//"http://192.168.0.120/Menumatem/";//;
        public const string BaseAddress = "http://mmregistrations.azurewebsites.net/";
      // public const string BaseAddress = "http://mmregistrations-stg-mmregistrations.azurewebsites.net/";//"http://192.168.0.120/Menumatem/";//;
       public const string PosTerminalDetails = "api/v1/TerminalApi/PostTerminalDetail";
       public const string ValidateCompanyInfo = "api/v1/CompanyApi/GetValidateCompanyInfo";

    }
}
