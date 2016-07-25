#include <i_item_definition_altered_message.hh>
#include <i_signalling_item_definition.hh>
#include <i_size_definition.hh>
#include <i_size_definition_altered_message.hh>
#include <item_definition_altered_message_factory.hh>
#include <item_definition_altered_message_maker.hh>
#include <item_definition_factory.hh>
#include <item_management_booter.hh>
#include <item_management_network_messaging_fwd.hh>
#include <menu_manager.hh>
#include <signalling_item_definition_factory.hh>
#include <signalling_item_manager.hh>
#include <size_definition_altered_message_factory.hh>
#include <size_definition_altered_message_maker.hh>
#include <size_definition_factory.hh>
#include <spinlock.hh>

#include "DeviceRealControl.h"
#include "MMMessageBox.h"

namespace item_management {

using utilities::locking::i_spinlock;
using utilities::locking::spinlock;
using utilities::locking::spinlock_reference;

using network_messaging::i_item_definition_altered_message;
using network_messaging::i_size_definition_altered_message;
using network_messaging::item_definition_altered_message_factory;
using network_messaging::item_definition_altered_message_maker;
using network_messaging::size_definition_altered_message_factory;
using network_messaging::size_definition_altered_message_maker;
using network_messaging::refresh_item_display;

using Database::TDBTransaction;

static const UnicodeString TrueOrFalse[2] = {
   "F", "T"
};

class item_definition_altered_handler
    : public i_item_altered_signal_handler {
public:
        item_definition_altered_handler()
        {
        };

        item_definition_altered_handler(
            i_signalling_item_manager *item_manager,
            const i_item_definition_altered_message_factory *message_factory)
            : item_manager_(item_manager),
              message_factory_(message_factory)
        {
        };

        void
        operator()(const i_item_definition &definition)
        const
        {
                static TDeviceRealControl &drc =
                    TDeviceRealControl::ActiveInstance();
                TDBTransaction tr(drc.DBControl);
                TIBSQL *qr = tr.Query(tr.AddQuery());

                std::auto_ptr<i_item_definition_altered_message>
                    message(
                        message_factory_->create(item_manager_,
                                                 definition.get_enabled(),
                                                 definition.get_key()));

                drc.ManagerNet->SendToAll(message.get());

                Screen->ActiveForm->Perform(refresh_item_display, 0x0, 0x0);

               qr->SQL->Text = "update item "
                               "       set enabled = :enabled_state "
                               "where item_key = :item_key;";
               tr.StartTransaction();
               qr->ParamByName("item_key")->AsInteger = definition.get_key();
               qr->ParamByName("enabled_state")->AsString =
                 TrueOrFalse[definition.get_enabled()];
               qr->ExecQuery();
               qr->Close();
               tr.Commit();
        };

        item_definition_altered_handler &
        operator=(const item_definition_altered_handler &rhs)
        {
                item_manager_ = rhs.item_manager_;
                message_factory_ = rhs.message_factory_;

                return *this;
        };

private:
        item_definition_altered_handler(
            const item_definition_altered_handler &initializer);

        i_signalling_item_manager *item_manager_;
        const i_item_definition_altered_message_factory *message_factory_;
};

class size_definition_altered_handler
    : public i_size_altered_signal_handler {
public:
        size_definition_altered_handler()
        {
        };

        size_definition_altered_handler(
            i_signalling_item_manager *item_manager,
            const i_size_definition_altered_message_factory *message_factory)
            : item_manager_(item_manager),
              message_factory_(message_factory)
        {
        };

        void
        operator()(
            const i_item_definition &item_definition,
            const i_size_definition &size_definition)
        const
        {
                static TDeviceRealControl &drc =
                    TDeviceRealControl::ActiveInstance();
                TDBTransaction tr(drc.DBControl);
                TIBSQL *qr = tr.Query(tr.AddQuery());
                TIBSQL *qr1 = tr.Query(tr.AddQuery());

                std::auto_ptr<i_size_definition_altered_message>
                    message(
                        message_factory_->create(
                            item_manager_,
                            item_definition.get_key(),
                            size_definition.get_key(),
                            size_definition.get_available_quantity(),
                            size_definition.get_default_quantity(),
                            size_definition.get_enabled(),
                            size_definition.get_price(),
                            size_definition.get_special_price(),
                            size_definition.get_warning_quantity()));

                drc.ManagerNet->SendToAll(message.get());

                Screen->ActiveForm->Perform(refresh_item_display, 0x0, 0x0);

               qr->SQL->Text =
                 "update itemsize "
                 "       set available_quantity = :available_quantity, "
                 "           default_quantity = :default_quantity, "
                 "           warning_quantity = :warning_quantity, "
                 "           price = :price, "
                 "           special_price = :special_price, "
                 "           enabled = :enabled_state "
                 "where item_key = :item_key "
                 "      and itemsize_key = :itemsize_key;";

               tr.StartTransaction();
               qr->ParamByName("item_key")->AsInteger = item_definition.get_key();
               qr->ParamByName("itemsize_key")->AsInteger = size_definition.get_key();

               qr->ParamByName("available_quantity")->AsDouble          = size_definition.get_available_quantity();
               qr->ParamByName("default_quantity")->AsDouble            = size_definition.get_default_quantity();
               qr->ParamByName("warning_quantity")->AsDouble            = size_definition.get_warning_quantity();
               qr->ParamByName("price")->AsDouble                       = size_definition.get_price();
               qr->ParamByName("special_price")->AsDouble               = size_definition.get_special_price();
               qr->ParamByName("enabled_state")->AsString               = TrueOrFalse[size_definition.get_enabled()];

               qr->ExecQuery();
               qr->Close();
               tr.Commit();


               qr1->Close();
               qr1->SQL->Text =
                         "UPDATE PRICELEVELITEMSIZE "
                                " SET   PRICE = :PRICE, "
                                " ISENABLED = :ISENABLED "
                        "where ITEMSIZE_KEY = :ITEMSIZE_KEY "
                        " and PRICELEVEL_KEY = :PRICELEVEL_KEY; ";
               tr.StartTransaction();
               qr1->ParamByName("itemsize_key")->AsInteger = size_definition.get_key();
               qr1->ParamByName("PRICE")->AsDouble = size_definition.get_price();
               qr1->ParamByName("ISENABLED")->AsString = "T";
               qr1->ParamByName("PRICELEVEL_KEY")->AsInteger = 1;
               qr1->ExecQuery();
               qr1->Close();
               tr.Commit();

        };

        size_definition_altered_handler &
        operator=(const size_definition_altered_handler &rhs)
        {
                item_manager_ = rhs.item_manager_;
                message_factory_ = rhs.message_factory_;

                return *this;
        };

private:
        size_definition_altered_handler(
            const size_definition_altered_handler &initializer);

        i_signalling_item_manager *item_manager_;
        const i_size_definition_altered_message_factory *message_factory_;
};

class size_definition_warning_reached_handler
    : public i_size_warning_reached_signal_handler {
public:
        size_definition_warning_reached_handler() { };
        ~size_definition_warning_reached_handler() { };

#pragma warn -par
        size_definition_warning_reached_handler &
        operator=(const size_definition_warning_reached_handler &rhs)
        {
                return *this;
        };
#pragma warn .par

        void
        operator()(
            const i_item_definition &id,
            const i_size_definition &sd)
        const
        {
                MessageBox(id.get_name()
                           + " ("
                           + sd.get_name()
                           + ") has fallen below its warning threshold.",
                           "Item has fallen below its warning threshold!",
                           MB_ICONINFORMATION | MB_OK);
        };

private:
        size_definition_warning_reached_handler(
            const size_definition_warning_reached_handler &initializer);
};

static item_definition_altered_handler item_definition_altered_handler_;
static size_definition_altered_handler size_definition_altered_handler_;
static size_definition_warning_reached_handler
    size_definition_warning_reached_handler_;

kickstarter *kickstarter::instance_ = 0x0;

i_spinlock *kickstarter::instance_initialization_lock_ = new spinlock();

kickstarter::kickstarter()
    : item_manager_(0x0),
      menu_manager_(0x0),
      item_definition_factory_(0x0),
      signalling_item_definition_factory_(0x0),
      size_definition_factory_(0x0),
      item_definition_altered_message_factory_(0x0),
      item_definition_altered_message_maker_(0x0),
      size_definition_altered_message_factory_(0x0),
      size_definition_altered_message_maker_(0x0)
{
        initialize_proper();
}

kickstarter::~kickstarter()
{
        cleanup();
}

i_item_manager &
kickstarter::get_item_manager()
{
        initialize();

        return *instance_->item_manager_;
}

i_menu_manager &
kickstarter::get_menu_manager()
{
        initialize();

        return *instance_->menu_manager_;
}

const i_item_definition_factory &
kickstarter::get_item_definition_factory()
{
        initialize();

        return *instance_->item_definition_factory_;
}

const i_size_definition_factory &
kickstarter::get_size_definition_factory()
{
        initialize();

        return *instance_->size_definition_factory_;
}

void
kickstarter::kickstart()
{
        initialize();
}

void
kickstarter::initialize()
{
        spinlock_reference lock(*instance_initialization_lock_);

        if (instance_)
                return;

        instance_ = new kickstarter();
}

void
kickstarter::cleanup()
{
        destroy_makers();
        destroy_managers();
        destroy_factories();
}

void
kickstarter::destroy_factories()
{
        delete item_definition_altered_message_factory_;
        item_definition_altered_message_factory_ = 0x0;

        delete item_definition_factory_;
        item_definition_factory_ = 0x0;

        delete signalling_item_definition_factory_;
        signalling_item_definition_factory_ = 0x0;

        delete size_definition_altered_message_factory_;
        size_definition_altered_message_factory_ = 0x0;

        delete size_definition_factory_;
        size_definition_factory_ = 0x0;
}

void
kickstarter::destroy_makers()
{
        delete item_definition_altered_message_maker_;
        item_definition_altered_message_maker_ = 0x0;

        delete size_definition_altered_message_maker_;
        size_definition_altered_message_maker_ = 0x0;
}

void
kickstarter::destroy_managers()
{
        delete item_manager_;
        item_manager_ = 0x0;

        delete menu_manager_;
        menu_manager_ = 0x0;
}

void
kickstarter::initialize_proper()
{
        try {
                initialize_factories();
                initialize_managers();
                initialize_makers();
        } catch (...) {
                cleanup();
                throw;
        }
}

void
kickstarter::initialize_factories()
{
        size_definition_factory_ = new size_definition_factory();
        item_definition_factory_ = new item_definition_factory();

        signalling_item_definition_factory_ =
            new signalling_item_definition_factory(*item_definition_factory_);

        item_definition_altered_message_factory_ =
            new item_definition_altered_message_factory();
        size_definition_altered_message_factory_ =
            new size_definition_altered_message_factory();
}

void
kickstarter::initialize_makers()
{
        item_definition_altered_message_maker_ =
            new item_definition_altered_message_maker(
                item_manager_, item_definition_altered_message_factory_);

        size_definition_altered_message_maker_ =
            new size_definition_altered_message_maker(
                item_manager_, size_definition_altered_message_factory_);
}

void
kickstarter::initialize_managers()
{
        item_definition_altered_handler_ =
            item_definition_altered_handler(
                item_manager_, item_definition_altered_message_factory_);

        size_definition_altered_handler_ =
            size_definition_altered_handler(
                item_manager_, size_definition_altered_message_factory_);

        item_manager_ =
            new signalling_item_manager(
                item_definition_factory_,
                signalling_item_definition_factory_,
                size_definition_factory_,
                &item_definition_altered_handler_,
                &size_definition_altered_handler_,
                &size_definition_warning_reached_handler_);

        menu_manager_ = new menu_manager(item_manager_);
}

} /* item_management */

