class ComandInterpreter{
  String TokenVectorStorage[3] = {""};
  int EngineAThrotle = 0; //positive forward, negative backward 
  int SteeringEngine = 0;
  bool HeadLight = false;
  bool BackLight = false;

  int EngineAPinA =0;
  int EngineAPinB =0;

  int SteeringPinA =0;
  int SteeringPinB =0;

  int HeadLightPin = 0;
  int BackLightPin = 0;

  size_t ComandStartTime = 0; //0 means no comand running 

  size_t TimeoutTime = 500; // in miliseconds 

  public:
  ComandInterpreter(int EngAPinA, int EngAPinB, int SteeringPinAArg, int SteeringPinBArg, int HeadLightPinArg, int BackLightPinArg):
    EngineAPinA(EngAPinA), EngineAPinB(EngAPinB),SteeringPinA(SteeringPinAArg),SteeringPinB(SteeringPinBArg)
    ,HeadLightPin(HeadLightPinArg),BackLightPin(BackLightPinArg)
  {
    ;
  }
  void Reset()
  {
    EngineAThrotle = 0;
    SteeringEngine = 0;
    HeadLight = false;
    BackLight = false;
    ComandStartTime = 0;

    UpdateHardware();
  }

  void UpdateHardware()
  {
    //engine forward and backward
    if (EngineAThrotle > 0)
    {
      digitalWrite(EngineAPinA,0);
      analogWrite(EngineAPinB,EngineAThrotle);
    }
    else if (EngineAThrotle < 0)
    {
      digitalWrite(EngineAPinB,0);
      digitalWrite(EngineAPinA ,EngineAThrotle*-1);
    }
    else
    {
      digitalWrite(EngineAPinA,0);
      digitalWrite(EngineAPinB,0);
    }
    //steering left and right 
    if (SteeringEngine > 0)
    {
      digitalWrite(SteeringPinA,0);
      analogWrite(SteeringPinB,SteeringEngine);
    }
    else if (SteeringEngine < 0)
    {
      digitalWrite(SteeringPinB,0);
      digitalWrite(SteeringPinA ,SteeringEngine*-1);
    }
    else
    {
      digitalWrite(SteeringPinA,0);
      digitalWrite(SteeringPinB,0);
    } 
    
    digitalWrite(HeadLightPin, HeadLight);    
    digitalWrite(BackLightPin, BackLight);    


  }

  bool interpretComand(byte *CommandBuffer)
  {
    EngineAThrotle = CommandBuffer[0];
    SteeringEngine = CommandBuffer[1];
    byte flags = CommandBuffer[2];

    if (bitRead(flags, 7)){
       EngineAThrotle *= -1; 
    }
    if (bitRead(flags, 6)){
       SteeringEngine *= -1; 
    }


    HeadLight = bitRead(flags, 1);

    BackLight = bitRead(flags, 0);
    
    UpdateHardware();
    ComandStartTime = millis();
  
    Serial.print(String(EngineAThrotle)+" "+String(SteeringEngine)+
    " "+String(HeadLight)+" "+String(BackLight)+" "+String(ComandStartTime)+"\r\n" );

    return true;
  }

  void CheckComandTimeout()
  {
    if ((millis() - ComandStartTime) > TimeoutTime)
    {
      Reset();
    }
  }
};

