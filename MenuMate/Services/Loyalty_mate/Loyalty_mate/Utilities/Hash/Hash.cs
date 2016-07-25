using System;
using System.Security.Cryptography;
using System.Text;

namespace Loyalty_mate.Utilities.Hash
{

public static class Hash {
        public static byte[]
        Create_hash(HashAlgorithm algorithm, byte[] input)
        {
                return algorithm.ComputeHash(input);
        }

        public static string
        Create_checksum(HashAlgorithm algorithm, byte[] input)
        {
                byte[] hash = Create_hash(algorithm, input);
                string checksum = "";

                foreach (byte i in hash)
                        checksum = string.Concat(checksum, i.ToString("x2"));

                return checksum;
        }

        public static string
        Create_checksum_from_string(HashAlgorithm algorithm,
                                    string input,
                                    Encoding encoding = null)
        {
                if (encoding == null)
                        encoding = Encoding.UTF8;

                return Create_checksum(algorithm, encoding.GetBytes(input));
        }

        public static string
        Create_md5_checksum(byte[] input)
        {
                return Create_checksum(md5_algorithm, input);
        }

        public static string
        Create_md5_checksum_from_string(string input,
                                        Encoding encoding = null)
        {
                return Create_checksum_from_string(md5_algorithm,
                                                   input,
                                                   encoding);
        }

        static MD5 md5_algorithm = MD5.Create();
} /* Hash */

} /* Utilities.Hash */
