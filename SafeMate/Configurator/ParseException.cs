using System;

namespace Safemate.Configurator
{
    public enum ParseError {
        InvalidPort,
        InvalidUsername,
        InvalidPassword,
        InvalidRepositoryPath,
        InvalidSchedule,
        MissingAccountDeclaration,
        MissingCopyGroup,
        MissingCopyOperation,
        MissingCrmDeclaration,
        MissingEmailNotifications,
        MissingMenumateTarget,
        MissingPattern,
        MissingRepositoryName,
        MissingSafemateConfiguration,
        MissingSenderSpecification,
        MissingSource
    };

    public class ParseException : Exception
    {
        public
        ParseException(
            ParseError cause,
            Exception innerException)
        : base("Configuration error", innerException)
        {
            this.cause = cause;
        }

        public ParseError Cause
        {
            get {
                return Cause;
            }
        }

        ParseError cause;
    };
};