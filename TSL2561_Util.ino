bool initLight()
{
  bool status = false;
  light.begin();
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
    status = true;
  }
  else
  {
    byte error = light.getError();
    printError(error);
    status = false;
  }

  gain = 0;
  unsigned char time = 2;
  
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);
  
  Serial.println("Powerup...");
  light.setPowerUp();
  return status;

}

double readLight()
{
  double output = 0;
  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {  
    boolean good;  // True if neither sensor is saturated
    good = light.getLux(gain,ms,data0,data1,output);
    //if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }

  return output;
}

void printError(byte error)
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}