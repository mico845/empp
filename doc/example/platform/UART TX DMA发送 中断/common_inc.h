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

    #include "board.hpp"
inline volatile bool uart_flag = false;
#endif
