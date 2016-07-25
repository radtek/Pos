#ifndef __hh_size_definition_altered_message_maker__
#define __hh_size_definition_altered_message_maker__

#include <i_size_definition_altered_message_maker.hh>

namespace item_management {
namespace network_messaging {

class size_definition_altered_message_maker
    : public i_size_definition_altered_message_maker {
public:
        size_definition_altered_message_maker();
        size_definition_altered_message_maker(
            i_signalling_item_manager *manager,
            const i_size_definition_altered_message_factory *factory);
        size_definition_altered_message_maker(
            const i_size_definition_altered_message_maker &initializer);
        size_definition_altered_message_maker(
            const size_definition_altered_message_maker &initializer);
        ~size_definition_altered_message_maker();

        size_definition_altered_message_maker &operator=(
            const i_size_definition_altered_message_maker &rhs);
        size_definition_altered_message_maker &operator=(
            const size_definition_altered_message_maker &rhs);

        const i_size_definition_altered_message_factory &
            get_factory() const;
        i_signalling_item_manager &get_item_manager() const;
 
protected:
        TNetMessage *MakeMessage(TMemoryStream *istream) const;

private:
        const i_size_definition_altered_message_factory *factory_;
        i_signalling_item_manager *manager_;
};

} /* network_messaging */
} /* item_management */

#endif /* ! __hh_size_definition_altered_message_maker__ */

