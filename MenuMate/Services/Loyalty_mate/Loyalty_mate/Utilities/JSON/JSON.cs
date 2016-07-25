using System;
using System.Net;
using System.Text;
using System.Web.Script.Serialization;
using Loyalty_mate.Utilities.JSON.Exceptions;
using Loyalty_mate.Utilities.JSON.Converters;
using System.Collections;
using System.Collections.Generic;

namespace Loyalty_mate.Utilities.JSON
{

public static class JSON {
        /// <summary>
        ///   Some streams have to have their lengths set before data can be
        ///   written to them. This delegate type allows user code to be called
        ///   just before data is written to a stream, giving them time to set
        ///   the length.
        /// </summary>
        /// <param name="length">
        ///   The length of the data about to be written.
        /// </param>
        public delegate void Stream_length_setter(int length);

        /// <summary>
        ///   Creates an instance of type T from a JSON encoded string.
        /// </summary>
        /// <typeparam name="T">
        ///   The type to be deserialized to.
        /// </typeparam>
        /// <param name="encoded">
        ///   The JSON encoding of an instance of T.
        /// </param>
        /// <returns>
        ///   A new instance of T.
        /// </returns>
        public static T
        Deserialize<T>(string encoded)
        {
                try {
                        T instance = serializer_.Deserialize<T>(encoded);
                        return instance;
                } catch (ArgumentException) {
                        throw new Invalid_JSON_exception(encoded);
                }
        }

        /// <summary>
        ///   Creates an instance of type T from JSON provided by a stream.
        /// </summary>
        /// <typeparam name="T">
        ///   The type to be deserialized to.
        /// </typeparam>
        /// <param name="stream">
        ///   The stream to read the encoding from.
        /// </param>
        /// <returns>
        ///   A new instance of T.
        /// </returns>
        public static T
        Deserialize_from_stream<T>(System.IO.StreamReader stream)
        {
                return Deserialize<T>(stream.ReadToEnd());
        }

        /// <summary>
        ///   Encodes an instance of type T into a JSON string.
        /// </summary>
        /// <typeparam name="T">
        ///   The type of the object to be serialized.
        /// </typeparam>
        /// <param name="datum">
        ///   The object to be serialized.
        /// </param>
        /// <returns>
        ///   A JSON encoding of that object.
        /// </returns>
        public static string
        Serialize<T>(T datum)
        {
                return serializer_.Serialize(datum);
        }

        /// <summary>
        ///   Serializes an object and writes the JSON encoding to a stream.
        /// </summary>
        /// <typeparam name="T">
        ///   The type of the object being serialized.
        /// </typeparam>
        /// <param name="datum">
        ///   The object to be serialized.
        /// </param>
        /// <param name="stream">
        ///   The stream to be written to.
        /// </param>
        /// <returns>
        ///   The number of bytes written to the stream.
        /// </returns>
        public static int
        Serialize_to<T>(T datum, System.IO.StreamWriter stream)
        {
                byte[] bytes =
                    stream.Encoding.GetBytes(Serialize<T>(datum));
                stream.Write(bytes);

                return bytes.Length;
        }

        /// <summary>
        ///   Serializes the object and makes the encoding the body of
        ///   an HttpWebRequest.
        /// </summary>
        /// <typeparam name="T">
        ///   The type of the object to be serialized.
        /// </typeparam>
        /// <param name="datum">
        ///   The object to be serialized.
        /// </param>
        /// <param name="request">
        ///   The request whose body should be the JSON encoding.
        /// </param>
        /// <param name="requestMode">
        ///   The request type to be used (GET/POST/PUT)
        /// </param>
        /// <returns>
        ///   The number of bytes written to the request body.
        /// </returns>
        public static int
        Serialize_to<T>(T datum, HttpWebRequest request, string requestMode)
        {
                byte[] bytes =
                    Encoding.UTF8.GetBytes(Serialize<T>(datum));

                request.Method = requestMode;
                request.ContentLength = bytes.Length;
                request.ContentType = "application/json";

                request.GetRequestStream().Write(bytes, 0, bytes.Length);

                return bytes.Length;
        }

        /// <summary>
        ///   Attempts to create an instance of type T from a JSON string.
        /// </summary>
        /// <typeparam name="T">
        ///   The type to be deserialized to.
        /// </typeparam>
        /// <param name="encoded">
        ///   A JSON encoding of an instance of type T.
        /// </param>
        /// <param name="deserialize_to">
        ///   The variable to which we assign the decoded instance of T.
        /// </param>
        /// <returns>
        ///   True if the deserialization succeeded.
        /// </returns>
        public static bool
        Try_and_deserialize<T>(string encoded, out T deserialize_to)
        {
                try {
                        deserialize_to = Deserialize<T>(encoded);
                        return true;
                } catch (ArgumentException) {
                        deserialize_to = default(T);
                        return false;
                }
        }

        /// <summary>
        ///   Attempts to create an instance of type T from JSON provided by
        ///   a stream.
        /// </summary>
        /// <typeparam name="T">
        ///   The type of object to be deserialized to.
        /// </typeparam>
        /// <param name="stream">
        ///   The stream to fetch JSON from.
        /// </param>
        /// <param name="deserialize_to">
        ///   The variable to which we assign the decoded instance of T.
        /// </param>
        /// <returns>
        ///   True if the deserialization succeeded.
        /// </returns>
        public static bool
        Try_and_deserialize_from_stream<T>(System.IO.StreamReader stream,
                                          out T deserialize_to)
        {
                return Try_and_deserialize<T>(stream.ReadToEnd(),
                                              out deserialize_to);
        }

        /// <summary>
        /// Registered pre-implemented converters with this serializer
        /// </summary>
        /// <param name="converters">
        /// Converters collection
        /// </param>
        public static void 
        Register_Converters(IEnumerable<JavaScriptConverter> converters)
        {
            serializer_.RegisterConverters(converters);
        }

        static JavaScriptSerializer serializer_ = new JavaScriptSerializer();

} /* JSON */

} /* Utilities.JSON */
