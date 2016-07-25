#ifndef __hh_item_definition_altered_message_maker__
#define __hh_item_definition_altered_message_maker__

#include <i_item_definition_altered_message_maker.hh>

namespace item_management {
namespace network_messaging {

class item_definition_altered_message_maker
    : public i_item_definition_altered_message_maker {
public:
        item_definition_altered_message_maker();
        item_definition_altered_message_maker(
            i_signalling_item_manager *item_manager,
            const i_item_definition_altered_message_factory *
                message_factory);
        item_definition_altered_message_maker(
            const i_item_definition_altered_message_maker &initializer);
        item_definition_altered_message_maker(
            const item_definition_altered_message_maker &initializer);
        ~item_definition_altered_message_maker();

        item_definition_altered_message_maker &operator=(
            const i_item_definition_altered_message_maker &rhs);
        item_definition_altered_message_maker &operator=(
            const item_definition_altered_message_maker &rhs);

        const i_item_definition_altered_message_factory &
            get_factory() const;
        i_signalling_item_manager &get_item_manager() const;
 
protected:
        TNetMessage *MakeMessage(TMemoryStream *istream) const;

private:
        i_signalling_item_manager *manager_;
        const i_item_definition_altered_message_factory *factory_;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_item_definition_altered_message_maker__ */

