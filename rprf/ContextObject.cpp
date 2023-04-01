#include "ContextObject.h"

#include "RadeonProRender.h"
#include "Error.h"
#include <iostream>

namespace rprf
{
    void __rprObjectDelete(void* instance)
    {
        rpr_int status;
        status = rprObjectDelete(instance);
        check(status);
    }
    void __rprObjectSetName(void* node, const char* name)
    {
        rpr_int status;
        status = rprObjectSetName(node, name);
        check(status);
    }

    void __printError(const char* what)
    {
        std::cerr << what << "\n";
    }

    void __printError(const char* what, const char* err)
    {

        std::cerr << what << err << "\n";
    }
}
