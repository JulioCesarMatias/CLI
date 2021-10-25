#pragma once

#include <inttypes.h>

#define VAR1_ADDR 1
#define VAR2_ADDR 2
#define VAR3_ADDR 3

typedef struct CLI_Param_Adjustable_Struct {
  uint8_t Var1;
  uint8_t Var2;
  uint8_t Var3;
} CLI_Param_Adjustable;
