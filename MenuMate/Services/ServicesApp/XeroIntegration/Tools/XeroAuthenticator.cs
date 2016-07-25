using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xero.Api.Infrastructure.Interfaces;
using System.Security.Cryptography.X509Certificates;
using Xero.Api.Infrastructure.OAuth;
using Xero.Api.Infrastructure.OAuth.Signing;

namespace XeroIntegration.Utility
{
    class XeroAuthenticator:IAuthenticator
    {
        private readonly X509Certificate2 _certificate;

        public XeroAuthenticator(string certificatePath)
            : this(certificatePath, "")
        {

        }

        public XeroAuthenticator(string certificatePath, string certificatePassword = "")
        {
            _certificate = new X509Certificate2();
            _certificate.Import(certificatePath, certificatePassword, X509KeyStorageFlags.MachineKeySet);
        }

        public XeroAuthenticator(X509Certificate2 certificate)
        {
            _certificate = certificate;
        }

        public X509Certificate Certificate { get { return _certificate; } }

        public string GetSignature(IConsumer consumer, IUser user, Uri uri, string verb, IConsumer consumer1)
        {
            var token = new Token
            {
                ConsumerKey = consumer.ConsumerKey,
                ConsumerSecret = consumer.ConsumerSecret,
                TokenKey = consumer.ConsumerKey
            };

            return new RsaSha1Signer().CreateSignature(_certificate, token, uri, verb);
        }

        public IToken GetToken(IConsumer consumer, IUser user)
        {
            throw new NotImplementedException();
        }

        public IUser User { get; set; }
    }
}
