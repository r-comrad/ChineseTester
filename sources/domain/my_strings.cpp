#include "domain/my_strings.hpp"

str_val
getString
(
    const char* aString,
    bool aCheckZero
)
{
    str_val result(aString);
    if (aCheckZero && result[result.size()] != 0)
    {
        // TODO: check for zero index
        // TODO1
        result.push_back(0);
    }
    return result;
}

str_val
getString
(
    const unsigned char* aString,
    bool aCheckZero
)
{
    str_val result;
    for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
    return result;
}

#ifdef BILL_WINDOWS
void
makeWindowString
(
    str_ref aString
)
{
    for (std_string_cell& i : aString)
    {
        if (i == '/') i = '\\';
    }
}
#endif // !BILL_WINDOWS