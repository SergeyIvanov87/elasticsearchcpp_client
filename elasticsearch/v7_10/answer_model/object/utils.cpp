#include <algorithm>
#include <sstream>

#include "elasticsearch/v7_10/answer_model/object/utils.h"

namespace model
{
bool str_to_bool(std::string str)
{
    bool res = false;

    std::transform(str.begin(), str.end(), str.begin(), [] (int c)
    {
        return ::tolower(c);
    });

    std::istringstream is(str);
    is >> res;

    if (is.fail())
    {
        is.clear();
        is >> std::boolalpha >> res;
    }

    return res;
}

}
