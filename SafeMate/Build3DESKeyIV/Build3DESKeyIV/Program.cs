using System;
using System.Security.Cryptography;
using System.Text;
using System.IO;
using System.Xml;

namespace TripleDESKeyGenerator {
        public class TripleDESKeyGeneratorApp {
                static
                TripleDESKeyGeneratorApp()
                {
                        provider = new DESCryptoServiceProvider();
                        provider.KeySize = 64;
                        provider.GenerateIV();
                        provider.GenerateKey();
                        provider.Mode = CipherMode.CBC;
                        provider.Padding = PaddingMode.ISO10126;
                }

                static void
                EncryptAuthenticationFile()
                {
                    ICryptoTransform xfrm =
                        provider.CreateEncryptor();
                    FileStream io = File.OpenRead("plaintext_authfile.xml");
                    byte []inBuf = new byte[io.Length];
                    byte []outBuf;

                    io.Read(inBuf, 0, inBuf.Length);
                    io.Close();

                    outBuf = xfrm.TransformFinalBlock(inBuf, 0, inBuf.Length);

                    io = File.OpenWrite("ciphertext_authfile.xml");
                    io.Write(outBuf, 0, outBuf.Length);
                    io.Flush();
                    io.Close();
                }

                static void RebuildIVK(string str)
                {
                    byte []keysizeBuf = Convert.FromBase64String(
                      str.Substring(0, 2) +  str.Substring(26, 2));
                    int ks = (((keysizeBuf[0] - 0x30) * 10) +
                              (keysizeBuf[1] - 0x30));
                    byte []iv =
                      Convert.FromBase64String(str.Substring(2, 12));
                    byte []k =
                      Convert.FromBase64String(str.Substring(14, 12));

                    provider.KeySize = ks;
                    provider.Key = k;
                    provider.IV = iv;
                }

                static void
                DecryptAuthenticationFile()
                {
                    ICryptoTransform xfrm =
                        provider.CreateDecryptor();
                    FileStream io = File.OpenRead("plaintext_authfile.xml");
                    byte []inBuf = new byte[io.Length];
                    byte []outBuf;
                    MemoryStream m;

                    io.Read(inBuf, 0, inBuf.Length);
                    io.Close();

                    outBuf = xfrm.TransformFinalBlock(inBuf, 0, inBuf.Length);
                    m = new MemoryStream(outBuf);
                    XmlDocument e = new XmlDocument();
                    e.Load(m);
;                    
                }

                static public void
                Main()
                {        
                    EncryptAuthenticationFile();

                    string initializationVectorAsBase64 =
                        Convert.ToBase64String(provider.IV);
                    string keyAsBase64 =
                        Convert.ToBase64String(provider.Key);
                    string keysizeAsBase64 =
                        Convert.ToBase64String(
                            Encoding.ASCII.GetBytes(
                                Convert.ToString(provider.KeySize)));
                    string encodedDecryptInformation =
                        String.Format("{0}{1}{2}{3}",
                            keysizeAsBase64.Substring(0, 2),
                            initializationVectorAsBase64,
                            keyAsBase64,
                            keysizeAsBase64.Substring(2));
                    Console.WriteLine(encodedDecryptInformation);

                    RebuildIVK(encodedDecryptInformation);
                    DecryptAuthenticationFile();
                }

                static DESCryptoServiceProvider provider;
        };
};
