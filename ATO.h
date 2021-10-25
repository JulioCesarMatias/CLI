#pragma once

#include "stdint.h"

//CONVERTE STRING EM NÚMEROS INTEIROS OU FLOAT
int16_t ATO_Int(const char *Pointer)
{
  int16_t Signal = 1;
  int16_t Base = 0;
  int16_t IndexCounter = 0;

  while (Pointer[IndexCounter] == ' ')
  {
    IndexCounter++;
  }

  if (Pointer[IndexCounter] == '-' || Pointer[IndexCounter] == '+')
  {
    Signal = 1 - 2 * (Pointer[IndexCounter++] == '-');
  }

  while (Pointer[IndexCounter] >= '0' && Pointer[IndexCounter] <= '9')
  {
    if ((Base > (0x7fff / 10)) || (Base == (0x7fff / 10) && Pointer[IndexCounter] - '0' > 7))
    {
      if (Signal == 1)
      {
        return 0x7fff;
      }
      else
      {
        return (-0x7fff - 1);
      }
    }
    Base = 10 * Base + (Pointer[IndexCounter++] - '0');
  }
  return Base * Signal;
}
