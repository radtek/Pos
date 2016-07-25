using Microsoft.Win32;
using System;
using System.Collections.Generic;
using Loyalty_mate.Utilities.OS.Exceptions.Registry;

namespace Loyalty_mate.Utilities.OS {

/// <summary>
///   Provides useful methods and structures to make working with the registry
///   more convenient.
/// </summary>
public sealed class Registry {
        /// <summary>
        ///   Used to incrementally traverse the Windows registry tree.
        ///   This is useful when you're trying to validate the path you're
        ///   accessing.
        /// </summary>
        public sealed class Iterator {
                delegate string[] String_array_producer();

                /// <summary>
                ///   Creates a new iterator which will begin its search at
                ///   root. Successive steps of the iterator will be rooted
                ///   at descendant paths of that root.
                /// </summary>
                /// <param name="root">
                ///   The key from which to begin traversal.
                /// </param>
                /// <param name="path">
                ///   The target path rooted at the specified root key. The
                ///   path format is the standard Windows format. E.g. "a\b\c".
                /// </param>
                /// <param name="writable">
                ///   Whether or not the target key should be made writable.
                /// </param>
                public
                Iterator(RegistryKey root, string path, bool writable)
                {
                        string[] path_fragments =
                            path.Split(fragment_separator,
                                       StringSplitOptions.RemoveEmptyEntries);
                        Key = root;
                        Keys_to_visit = new Queue<string>();
                        Keys_visited = new Queue<string>();
                        Writable = writable;

                        foreach (string m in path_fragments)
                                Keys_to_visit.Enqueue(m);
                }

                /// <summary>
                ///   Creates a sub key rooted at the current iterator position. A key will
                ///   only be created if it doesn't already exist.
                /// </summary>
                /// <param name="name">
                ///   The name of the new key.
                /// </param>
                /// <returns>
                ///   The iterator that created the key.
                /// </returns>
                public Iterator
                Create_sub_key(string name)
                {
                        if (!Writable)
                                throw new Modifying_read_only_key_exception(
                                    this,
                                    (Modifying_read_only_key_exception
                                     .Faulting_operation
                                     .Creating_sub_key),
                                    name);
                        else if (!Does_sub_key_exist(name))
                                Key.CreateSubKey(name);

                        return this;
                }

                /// <summary>
                ///   Sets the named value of the current key to the specified value. If no
                ///   value with the name exists, it is created.
                /// </summary>
                /// <param name="name">
                ///   The name of the value.
                /// </param>
                /// <param name="value">
                ///   The new value.
                /// </param>
                /// <returns>
                ///   The iterator that created/set the value.
                /// </returns>
                public Iterator
                Set_int_value(string name, int value)
                {
                        return Set_value<int>(name, value, RegistryValueKind.DWord);
                }

                /// <summary>
                ///   Sets the named value of the current key to the specified value. If no
                ///   value with the name exists, it is created.
                /// </summary>
                /// <param name="name">
                ///   The name of the value.
                /// </param>
                /// <param name="value">
                ///   The new value.
                /// </param>
                /// <returns>
                ///   The iterator that created/set the value.
                /// </returns>
                public Iterator
                Set_string_value(string name, string value)
                {
                        return Set_value<string>(name, value, RegistryValueKind.String);
                }

                /// <summary>
                ///   Sets the named value of the current key to the specified value. If no
                ///   value with the name exists, it is created.
                /// </summary>
                /// <typeparam name="T">
                ///   The C# type the value is to be set to.
                /// </typeparam>
                /// <param name="name">
                ///   The name of the value.
                /// </param>
                /// <param name="value">
                ///   The new value.
                /// </param>
                /// <param name="value_type">
                ///   The registry type of the value to be set.
                /// </param>
                /// <returns>
                ///   The iterator that created/set the value.
                /// </returns>
                public Iterator
                Set_value<T>(string name,
                             T value,
                             RegistryValueKind value_type)
                {
                        if (!Writable)
                                throw new Modifying_read_only_key_exception(
                                    this,
                                    (Modifying_read_only_key_exception
                                     .Faulting_operation
                                     .Creating_sub_key),
                                    name);

                        Key.SetValue(name, value, value_type);

                        return this;
                }

                /// <summary>
                ///   Descends one level in the tree growing one step closer to
                ///   the traversal target.
                /// </summary>
                /// <returns>
                ///   The next step in the traversal. This method will return null
                ///   in the case that we'd already iterated to the target path.
                /// </returns>
                public Iterator
                Descend()
                {
                        try {
                                string key_to_visit = Keys_to_visit.Peek();
                                RegistryKey key = Key.OpenSubKey(key_to_visit, true);

                                if (key == null)
                                        throw new Key_doesnt_exist_exception(this);

                                Keys_to_visit.Dequeue();
                                Keys_visited.Enqueue(key_to_visit);

                                return new Iterator(key, Keys_to_visit, Keys_visited, Writable);
                        } catch (InvalidOperationException) {
                                return null;
                        }
                }

                /// <summary>
                ///   Checks to see whether the current key has a child with
                ///   the specified name directly below. 
                /// </summary>
                /// <param name="name">
                ///   The name of the descendent key.
                /// </param>
                /// <returns>
                ///   True if the key exists, false otherwise.
                /// </returns>
                public bool
                Does_sub_key_exist(string name)
                {
                        return Value_present_in_array(name,
                                                      delegate()
                                                      {
                                                          return Key.GetSubKeyNames();
                                                      });
                }

                /// <summary>
                ///   Checks to see whether the current key has a value with the
                ///   specified name directly below.
                /// </summary>
                /// <param name="name">
                ///   The name of the value.
                /// </param>
                /// <returns>
                ///   True if the value exists, false otherwise.
                /// </returns>
                public bool
                Does_value_exist(string name)
                {
                        return Value_present_in_array(name,
                                                      delegate()
                                                      {
                                                          return Key.GetValueNames();
                                                      });
                }

                /// <summary>
                ///   Checks whether a string is present in a dynamically generated
                ///   string array.
                /// </summary>
                /// <param name="value">
                ///   The value to be searched for; it must be a straight name, not a path.
                /// </param>
                /// <param name="producer">
                ///   The method called to generate the array.
                /// </param>
                /// <returns>
                ///   True if the value was found, false otherwise.
                /// </returns>
                bool
                Value_present_in_array(string value, String_array_producer producer)
                {
                        if (value.Split(fragment_separator,
                                        StringSplitOptions.None).Length != 1)
                                throw new Nested_registry_path_exception(this, value);
                        string[] strings = producer();
                        string discovered_value =
                            Array.Find<string>(strings,
                                               Create_string_match_predicate(value));
                        return discovered_value == value;
                }

                /// <summary>
                ///   Retrieves the string-value named contained in the current key.
                /// </summary>
                /// <param name="value_name">
                ///   The name of the value to be retrieved.
                /// </param>
                /// <returns>
                ///   A string value.
                /// </returns>
                public int
                Int_value(string value_name)
                {
                        return Value<int>(value_name, RegistryValueKind.DWord);
                }

                /// <summary>
                ///   Retrieves the integer-value named contained in the current key.
                /// </summary>
                /// <param name="value_name">
                ///   The name of the value to be retrieved.
                /// </param>
                /// <returns>
                ///   An integer value.
                /// </returns>
                public string
                String_value(string value_name)
                {
                        return Value<string>(value_name, RegistryValueKind.String);
                }

                /// <summary>
                ///   Retrieves the named value contained in the current key.
                /// </summary>
                /// <typeparam name="T">
                ///   The C# type of the registry value.
                /// </typeparam>
                /// <param name="value_name">
                ///   The name of the value to be retrieved.
                /// </param>
                /// <param name="value_type">
                ///   The registry value type.
                /// </param>
                /// <returns>
                ///   An instance of T initialized from the retrieved registry value.
                /// </returns>
                public T
                Value<T>(string value_name, RegistryValueKind value_type)
                {
                        try {
                                RegistryValueKind discovered_type =
                                    Key.GetValueKind(value_name);

                                if (discovered_type != value_type)
                                        throw new Unexpected_value_type_exception(value_type,
                                                                                  discovered_type,
                                                                                  this,
                                                                                  value_name);
                                return (T)Key.GetValue(value_name);
                        } catch (System.IO.IOException) {
                                throw new Value_doesnt_exist_exception(this, value_name);
                        }
                }

                /// <summary>
                ///   Checks whether or not we've arrived at our target.
                /// </summary>
                /// <param name="iterator">
                ///   The iterator to check.
                /// </param>
                /// <returns>
                ///   True if we've arrived at our target, false otherwise.
                /// </returns>
                public static implicit
                operator bool(Iterator iterator)
                {
                        return iterator.Keys_to_visit.Count == 0;
                }

                /// <summary>
                ///   Creates a new iterator.
                /// </summary>
                /// <param name="key">
                ///   The new key to search from.
                /// </param>
                /// <param name="keys_to_visit">
                ///   Subkeys left to traverse.
                /// </param>
                /// <param name="keys_visited">
                ///   Subkeys already traversed.
                /// </param>
                Iterator(RegistryKey key,
                         Queue<string> keys_to_visit,
                         Queue<string> keys_visited,
                         bool writable)
                {
                        Key = key;
                        Keys_to_visit = keys_to_visit;
                        Keys_visited = keys_visited;
                        Writable = writable;
                }

                System.Predicate<string>
                Create_string_match_predicate(string matching)
                {
                        return delegate(string m)
                               {
                                       return m == matching;
                               };
                }

                /// <summary>
                ///   The current key on which the iterator is rooted.
                /// </summary>
                public readonly RegistryKey Key;
                /// <summary>
                ///   Subkeys left to visit.
                /// </summary>
                public readonly Queue<string> Keys_to_visit;
                /// <summary>
                ///   Subkeys traversed. This should end up being
                ///   equal to the path specified when the iterator
                ///   was originally created.
                /// </summary>
                public readonly Queue<string> Keys_visited;

                /// <summary>
                ///   Specifies whether the iterator is writable.
                /// </summary>
                public readonly bool Writable;

                /// <summary>
                ///   The character which divides path fragments.
                /// </summary>
                static readonly char[] fragment_separator = new char[] {
                        '\\'
                }; /* path_delimiter */
        } /* Iterator */

        /// <summary>
        ///   Creates a tree of registry keys. Only keys that do not exist
        ///   will be created.
        /// </summary>
        /// <param name="root">
        ///   The hive that the tree will be created in.
        /// </param>
        /// <param name="path">
        ///   The tree to be created. I.e. the path.
        /// </param>
        /// <returns>
        ///   An iterator positioned on the leaf key.
        /// </returns>
        public static Iterator
        Create_sub_key_tree(RegistryKey root, string path)
        {
                Iterator i = new Iterator(root, path, true);

                while (!i) {
                        i.Create_sub_key(i.Keys_to_visit.Peek());
                        i = i.Descend();
                }

                return i;
        }

        /// <summary>
        ///   Creates an iterator which is rooted at the target sub-key.
        /// </summary>
        /// <param name="root">
        ///   The hive that the path exists in.
        /// </param>
        /// <param name="path">
        ///   The path to target sub-key.
        /// </param>
        /// <returns>
        ///   An iterator to that sub-key.
        /// </returns>
        public static Iterator
        Open_sub_key(RegistryKey root, string path = "", bool writable = false)
        {
                Iterator i = new Iterator(root, path, writable);

                while (!i)
                        i = i.Descend();

                return i;
        }

        /// <summary>
        ///   Retrieves an integer value from the registry.
        /// </summary>
        /// <param name="root">
        ///   The hive that the path exists in.
        /// </param>
        /// <param name="path">
        ///   The path to the key in which the value is held.
        /// </param>
        /// <param name="value_name">
        ///   The name of the value to retrieve.
        /// </param>
        /// <returns>
        ///   An integer value.
        /// </returns>
        public static int
        Get_int_value(RegistryKey root, string path, string value_name)
        {
                return Get_value<int>(root,
                                      path,
                                      value_name,
                                      RegistryValueKind.DWord);
        }

        /// <summary>
        ///   Retrieves a string value from the registry.
        /// </summary>
        /// <param name="root">
        ///   The hive that the path exists in.
        /// </param>
        /// <param name="path">
        ///   The path to the key in which the value is held.
        /// </param>
        /// <param name="value_name">
        ///   The name of the value to retrieve.
        /// </param>
        /// <returns>
        ///   A string value.
        /// </returns>
        public static string
        Get_string_value(RegistryKey root, string path, string value_name)
        {
                return Get_value<string>(root,
                                         path,
                                         value_name,
                                         RegistryValueKind.String);
        }

        /// <summary>
        ///   Retrieves a value from the registry.
        /// </summary>
        /// <typeparam name="T">
        ///   The C# type that the registry value will be converted to.
        /// </typeparam>
        /// <param name="root">
        ///   The hive that the path exists in.
        /// </param>
        /// <param name="path">
        ///   The path to the key in which the value is held.
        /// </param>
        /// <param name="value_name">
        ///   The name of the value to retrieve.
        /// </param>
        /// <param name="value_type">
        ///   The registry type of the value. This is used to make retrieving
        ///   a value from the registry as type-safe as possible.
        /// </param>
        /// <returns>
        ///   An instance of T initialized from the registry value found.
        /// </returns>
        public static T
        Get_value<T>(RegistryKey root,
                     string path,
                     string value_name,
                     RegistryValueKind value_type)
        {
                Iterator i = Open_sub_key(root, path);

                return i.Value<T>(value_name, value_type);
        }
} /* Registry */

} /* Utilites.OS */
