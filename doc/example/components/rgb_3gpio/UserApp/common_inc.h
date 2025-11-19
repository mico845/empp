// common_inc.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

[[noreturn]] void Main();

#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/

    #include <empp.h>
using namespace empp::stm32h7xx;
using namespace empp::components;

#endif
