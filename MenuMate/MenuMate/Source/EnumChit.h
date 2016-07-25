#ifndef h_enum_chit
#define h_enum_chit

#include <System.hpp>

enum EChitPromptType {
    ectOff,
    ectAuto,
    ectPrompt,
    __numChitPromptTypes
};

enum EChitTriggerType {
    ecttOff,
    ecttMenu,
    ecttCourse,
    ecttServingCourse,
    __numChitTriggerTypes
};

enum EChitFormatType {
    ectNumeric,
    ectAlphaNumeric,
    ectList,
    __numChitFormatTypes
};

enum EChitRemoveTaxType {
    ectPurchaseTax,
    ectLocalTax,
    ectSalesTax,
    __numChitRemoveTaxTypes
};

extern const UnicodeString ChitPromptTypeStrings[__numChitPromptTypes];
extern const UnicodeString ChitTriggerTypeStrings[__numChitTriggerTypes];
extern const UnicodeString ChitFormatTypeStrings[__numChitFormatTypes];

#endif /* ! h_enum_chit */
