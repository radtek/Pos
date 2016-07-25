#ifndef hh_webmate_order_processor_necromancer
#define hh_webmate_order_processor_necromancer

#include <Classes.hpp>

#include "webmate_order_processor_impl.hh"

class webmate_order_processor_t::impl_t::necromancer_t : public TThread {
public:
    necromancer_t(
      std::auto_ptr<webmate_order_processor_t::impl_t::worker_t>
        &worker_ptr_ref);

private:
    void __fastcall Execute();

    std::auto_ptr<webmate_order_processor_t::impl_t::worker_t>
      &worker_ptr_ref_;
}; /* webmate_order_processor_t::necromancer_t */

#endif /* ! hh_webmate_order_processor_necromancer */

