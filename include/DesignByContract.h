#ifndef PROJECTSE_DESIGNBYCONTRACT_H
#define PROJECTSE_DESIGNBYCONTRACT_H

#include <cassert>

#if defined(__unix__)
#define REQUIRE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__);

    #define ENSURE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__);
#elif defined(__APPLE__)
#define REQUIRE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__);

    #define ENSURE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__);

#elif defined(_WIN32) || defined(_WIN64)
#define REQUIRE(assertion, what) \
        if (!(assertion)) _assert (what, __FILE__, __LINE__);

#define ENSURE(assertion, what) \
        if (!(assertion)) _assert (what, __FILE__, __LINE__);

#endif
#endif //PROJECTSE_DESIGNBYCONTRACT_H