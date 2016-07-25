#ifndef __c_magic_memories_sf_object__
#define __c_magic_memories_sf_object__

class _Sf_data_object {
public:
   virtual operator UnicodeString() = 0;

protected:
   _Sf_data_object(_Sf_bridge &bridge, Sf_data_object_type type)
     : bridge(bridge), _type(type) { };
   virtual ~_Sf_data_object() { };

   virtual sObject             *create_object() = 0;
   virtual bool                pull_id_from_cache(UnicodeString &destination);
   virtual const UnicodeString push_id_to_cache(const UnicodeString &id);

   virtual const UnicodeString get_or_create_object(
     const UnicodeString oid_query);

   const Sf_data_object_type type() const { return _type; };

   _Sf_bridge &bridge;

private:
   _Sf_data_object(const _Sf_data_object &initializer);
   _Sf_data_object &operator=(const _Sf_data_object &initializer);

   const Sf_data_object_type _type;
};

const UnicodeString
_Sf_data_object::get_or_create_object(const UnicodeString oid_query)
{
   UnicodeString        id;
   auto_ptr<sObject>    so;
   auto_ptr<SaveResult> sr;

   if (pull_id_from_cache(id))
      return id;
   else if ((id = bridge.get_oid(oid_query)) != Nil_string)
      return push_id_to_cache(id);

   so.reset(create_object());
   sr = bridge.create_object(so.get());
   assert(sr->success);

   return sr->success ? push_id_to_cache(sr->id) : Nil_string;
}

/*
 * The _Sf_data_object::(push|pull)_id_(from|to)_cache functions are implemented
 * here as debugging stubs. _Sf_data's get_or_create_object uses these functions
 * to locate and/or add IDs into a derived object's cache. The two functions are
 * overriden so that the caching mechanism isn't locked.
 *
 * There will be cases where an object's ID will not need to be or would make
 * no sense to be cached and so providing routines for managing the IDs would
 * be completely pointless. In these cases, the derived object in question would
 * redefine get_or_create_object.
 *
 * The following warnings are disabled. Rationale is given.
 * - 8070 (-w-par) "Function should return a value."
 *   - There's no point to return a value. These functions are intended to only
 *     ever be called during development and even then accidentally. Note the
 *     use of the "assert" macro which reduces to nothing when the program is
 *     compiled with NDEBUG defined.
 *
 * - 8057 (-w-rvl) "Parameter X is never used."
 *   - The parameters to the two caching functions are ignored as all the
 *     functions are there to achieve is a crash if they are not overridden.
 */

#pragma warn -par
#pragma warn -rvl

bool
_Sf_data_object::pull_id_from_cache(UnicodeString &destination)
{
   unsigned int is_implemented = 0;
   assert((is_implemented == 1));
}

const UnicodeString
_Sf_data_object::push_id_to_cache(const UnicodeString &id)
{
   unsigned int is_implemented = 0;
   assert(is_implemented == 1);
}

#pragma warn .rvl
#pragma warn .par

#endif

