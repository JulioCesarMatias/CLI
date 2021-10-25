#ifndef PARAM_H_
#define PARAM_H_
#include "Arduino.h"
#include "PARAM.h"

#if defined(__AVR_ATmega2560__)

#define FINAL_ADDR_OF_MICROC 4095

#else

#define FINAL_ADDR_OF_MICROC 1023

#endif

typedef enum
{
  VAR_8BITS,
  VAR_16BITS,
  VAR_32BITS
} VarType;

typedef struct Resources_Of_Param_Struct {
  const char *Param_Name;
  int32_t Address;
  uint8_t Variable_Type;
  void* Ptr;
  int32_t Value_Min;
  int32_t Value_Max;
  int32_t DefaultValue;
} Resources_Of_Param;

extern CLI_Param_Adjustable_Struct CLI_Param;

class ParamClass
{
  public:
    void Initialization(void);
    void Update(void);

  private:
    bool PrintMessage = false;
    char SerialBuffer[48];
    uint32_t SerialBufferIndex = 0;
    uint32_t PreviousMillis = 0;
    void Default_List(void);
    void Load_Sketch(void);
    void Process_Command(char *TerminalCommandLine);
};
extern ParamClass PARAM;
#endif
