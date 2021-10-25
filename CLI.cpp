#include "CLI.h"
#include "ATO.h"

ParamClass PARAM;

CLI_Param_Adjustable_Struct CLI_Param;

const Resources_Of_Param Params_Table[] = {
  //NOME                          ENDEREÇO NA EEPROM             TIPO                    VARIAVEL                            MIN             MAX         VALOR PADRÃO
  {"Var1",                        VAR1_ADDR,                     VAR_8BITS,              &CLI_Param.Var1,                     0,             255,             25},
  {"Var2",                        VAR2_ADDR,                     VAR_8BITS,              &CLI_Param.Var2,                     0,             255,             50},
  {"Var3",                        VAR3_ADDR,                     VAR_8BITS,              &CLI_Param.Var3,                     0,             255,             10},

};

#define TABLE_COUNT (sizeof(Params_Table) / sizeof(Resources_Of_Param))

void ParamClass::Initialization(void)
{
  PARAM.Load_Sketch();
}

uint8_t STORAGEMANAGER_Read_8Bits(int16_t Address)
{
  return eeprom_read_byte((const uint8_t *)Address);
}

int16_t STORAGEMANAGER_Read_16Bits(int16_t Address)
{
  return eeprom_read_word((const uint16_t *)Address);
}

int32_t STORAGEMANAGER_Read_32Bits(int16_t Address)
{
  return eeprom_read_dword((const uint32_t *)Address);
}

void STORAGEMANAGER_Write_8Bits(int16_t Address, int8_t Value)
{
  if (STORAGEMANAGER_Read_8Bits(Address) == Value)
  {
    return;
  }
  eeprom_write_byte((uint8_t *)Address, Value);
}

void STORAGEMANAGER_Write_16Bits(int16_t Address, int16_t Value)
{
  if (STORAGEMANAGER_Read_16Bits(Address) == Value)
  {
    return;
  }
  eeprom_write_word((uint16_t *)Address, Value);
}

void STORAGEMANAGER_Write_32Bits(int16_t Address, int32_t Value)
{
  if (STORAGEMANAGER_Read_32Bits(Address) == Value)
  {
    return;
  }
  eeprom_write_dword((uint32_t *)Address, Value);
}

void ParamClass::Default_List(void)
{
  for (uint32_t Table_Counter = 0; Table_Counter < TABLE_COUNT; Table_Counter++)
  {

    switch (Params_Table[Table_Counter].Variable_Type)
    {

      case VAR_8BITS:
        *(uint8_t *)Params_Table[Table_Counter].Ptr = Params_Table[Table_Counter].DefaultValue;
        STORAGEMANAGER_Write_8Bits(Params_Table[Table_Counter].Address, Params_Table[Table_Counter].DefaultValue);
        break;

      case VAR_16BITS:
        *(int16_t *)Params_Table[Table_Counter].Ptr = Params_Table[Table_Counter].DefaultValue;
        STORAGEMANAGER_Write_16Bits(Params_Table[Table_Counter].Address, Params_Table[Table_Counter].DefaultValue);
        break;

      case VAR_32BITS:
        *(int32_t *)Params_Table[Table_Counter].Ptr = Params_Table[Table_Counter].DefaultValue;
        STORAGEMANAGER_Write_32Bits(Params_Table[Table_Counter].Address, Params_Table[Table_Counter].DefaultValue);
        break;
    }
  }
}

void ParamClass::Load_Sketch(void)
{

  if (STORAGEMANAGER_Read_8Bits(FINAL_ADDR_OF_MICROC) != 0x7F)
  {
    Serial.println("Restaurando os valores de fabrica dos parametros...");
    for (uint8_t IndexCount = 0; IndexCount < 5; IndexCount++) //FORÇA UMA REPETIÇÃO DE 5 VEZES
    {
      PARAM.Default_List();
      STORAGEMANAGER_Write_8Bits(FINAL_ADDR_OF_MICROC, 0x7F);
    }
    Serial.println("Ok...Parametros reconfigurados!");
    Serial.println("\n");
  }

  for (uint32_t Table_Counter = 0; Table_Counter < TABLE_COUNT; Table_Counter++)
  {

    switch (Params_Table[Table_Counter].Variable_Type)
    {

      case VAR_8BITS:
        *(uint8_t *)Params_Table[Table_Counter].Ptr = STORAGEMANAGER_Read_8Bits(Params_Table[Table_Counter].Address);
        break;

      case VAR_16BITS:
        *(int16_t *)Params_Table[Table_Counter].Ptr = STORAGEMANAGER_Read_16Bits(Params_Table[Table_Counter].Address);
        break;

      case VAR_32BITS:
        *(int32_t *)Params_Table[Table_Counter].Ptr = STORAGEMANAGER_Read_32Bits(Params_Table[Table_Counter].Address);
        break;
    }
  }
}

static void Param_Set_And_Save_Value(const Resources_Of_Param *VariablePointer, const int32_t NewValue)
{
  switch (VariablePointer->Variable_Type)
  {

    case VAR_8BITS:
      *(uint8_t *)VariablePointer->Ptr = (uint8_t)NewValue;
      STORAGEMANAGER_Write_8Bits(VariablePointer->Address, NewValue);
      break;

    case VAR_16BITS:
      *(int16_t *)VariablePointer->Ptr = (int16_t)NewValue;
      STORAGEMANAGER_Write_16Bits(VariablePointer->Address, NewValue);
      break;

    case VAR_32BITS:
      *(int32_t *)VariablePointer->Ptr = (int32_t)NewValue;
      STORAGEMANAGER_Write_32Bits(VariablePointer->Address, NewValue);
      break;
  }
}

static void Param_Print_Value(const Resources_Of_Param *VariablePointer)
{
  int32_t New_Value = 0;

  switch (VariablePointer->Variable_Type)
  {

    case VAR_8BITS:
      New_Value = STORAGEMANAGER_Read_8Bits(VariablePointer->Address);
      break;

    case VAR_16BITS:
      New_Value = STORAGEMANAGER_Read_16Bits(VariablePointer->Address);
      break;

    case VAR_32BITS:
      New_Value = STORAGEMANAGER_Read_32Bits(VariablePointer->Address);
      Serial.println(New_Value);
      return;
  }
  Serial.println(New_Value);
}

void ParamClass::Process_Command(char *ParamCommandLine)
{
  const Resources_Of_Param *ParamValue;
  char *PtrInput = NULL;
  int32_t New_Value = 0;
  uint32_t Table_Counter;
  uint32_t StringLength;

  while (*ParamCommandLine == ' ')
  {
    ++ParamCommandLine;
  }

  StringLength = strlen(ParamCommandLine);

  if (StringLength == 0)
  {
    return;
  }
  else if (strncasecmp(ParamCommandLine, "ajuda", 5) == 0)
  {
    for (Table_Counter = 0; Table_Counter < TABLE_COUNT; Table_Counter++)
    {
      ParamValue = &Params_Table[Table_Counter];
      Serial.println(Params_Table[Table_Counter].Param_Name);
    }
    Serial.println("\n");
  }
  else if ((PtrInput = strstr(ParamCommandLine, "=")) != NULL)
  {
    PtrInput++;
    New_Value = ATO_Int(PtrInput);
    for (Table_Counter = 0; Table_Counter < TABLE_COUNT; Table_Counter++)
    {
      ParamValue = &Params_Table[Table_Counter];
      if (strncasecmp(ParamCommandLine, Params_Table[Table_Counter].Param_Name, strlen(Params_Table[Table_Counter].Param_Name)) == 0)
      {
        if (New_Value >= Params_Table[Table_Counter].Value_Min && New_Value <= Params_Table[Table_Counter].Value_Max)
        {
          Param_Set_And_Save_Value(ParamValue, New_Value);
          Serial.print(Params_Table[Table_Counter].Param_Name);
          Serial.print(" setado para ");
          Param_Print_Value(ParamValue);
          Serial.println("\n");
        }
        else if (New_Value < Params_Table[Table_Counter].Value_Min)
        {
          Serial.print("O valor setado esta fora do limite minimo!");
          Serial.println("\n");
        }
        else if (New_Value > Params_Table[Table_Counter].Value_Max)
        {
          Serial.print("O valor setado esta fora do limite maximo!");
          Serial.println("\n");
        }
        return;
      }
    }
    Serial.print("Parametro nao encontrado na lista");
    Serial.println("\n");
  }
  else if (strncasecmp(ParamCommandLine, "relatorio", 9) == 0)
  {
    for (Table_Counter = 0; Table_Counter < TABLE_COUNT; Table_Counter++)
    {
      ParamValue = &Params_Table[Table_Counter];
      Serial.print(Params_Table[Table_Counter].Param_Name);
      Serial.print(" = ");
      Param_Print_Value(ParamValue);
    }
    Serial.println("\n");
  }
  else if (strncasecmp(ParamCommandLine, "formatar", 8) == 0)
  {
    Serial.println("Restaurando os valores de fabrica dos parametros...");
    PARAM.Default_List();
    Serial.println("Ok...Parametros reconfigurados!");
    Serial.println("\n");
  }
  else
  {
    Serial.print("Comando invalido!");
    Serial.println("\n");
  }
}

void ParamClass::Update(void)
{
  if (millis() - PARAM.PreviousMillis < 20) //ROTINA DE 50Hz
  {
    return;
  }

  PARAM.PreviousMillis = millis();

  if (!PARAM.PrintMessage)
  {
    Serial.println("Comandos:");
    Serial.println("Ajuda; para listar os parametros disponiveis.");
    Serial.println("Relatorio; para listar todos os parametros com os valores atuais.");
    Serial.println("Formatar; para voltar todos os parametros ao padrao de fabrica.");
    Serial.println("\n");
    PARAM.PrintMessage = true;
  }

  while (Serial.available())
  {
    uint8_t SerialReadCommand = Serial.read();

    PARAM.SerialBuffer[PARAM.SerialBufferIndex++] = SerialReadCommand; //BUFFER

    if (PARAM.SerialBufferIndex && ((strstr(PARAM.SerialBuffer, ";")) != NULL))
    {
      PARAM.SerialBuffer[PARAM.SerialBufferIndex] = 0;
      PARAM.Process_Command(PARAM.SerialBuffer);
    }
  }

  if (PARAM.SerialBufferIndex > 0)
  {
    PARAM.SerialBuffer[PARAM.SerialBufferIndex--] = 0;
  }
}
