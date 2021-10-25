#include "CLI.h"

void setup() {
  Serial.begin(115200);

  PARAM.Initialization();
}

void loop() {
  PARAM.Update();
}
