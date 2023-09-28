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

  size_t ComandStartTime = 0; //0 means no comand running 

  size_t TimeoutTime = 100; // in miliseconds 

  public:
  ComandInterpreter(int EngAPinA, int EngAPinB, int SteeringPinAArg, int SteeringPinBArg):
  EngineAPinA(EngAPinA), EngineAPinB(EngAPinB),SteeringPinA(SteeringPinAArg),SteeringPinB(SteeringPinBArg)
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
  String PopToken(String &StringToPop)
  {
    long IndexOfSeparator = StringToPop.indexOf(" ");
    
    if (-1 == IndexOfSeparator)
    { 
      if(StringToPop.indexOf("\n") != -1)
        {
          IndexOfSeparator = StringToPop.indexOf("\n");
        }
        else{
          return "";
        }
    }
    //Serial.println(IndexOfSeparator);
    String token = StringToPop.substring(0,IndexOfSeparator);
    StringToPop.remove(0,IndexOfSeparator+1);
    return token;
  }
  void Tokenize(String StringToTokenize)
  {
    for(int i=0;i<3;i++)
    {
      TokenVectorStorage[i] = PopToken(StringToTokenize);
    }
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

  }
  void ExecuteComand(String ComandToParse, WiFiClient &client )
  {
    Tokenize(ComandToParse);
    EngineAThrotle = TokenVectorStorage[0].toInt();
    SteeringEngine = TokenVectorStorage[1].toInt();
    if (TokenVectorStorage[2][0] == '0')
    {
       HeadLight = false;
    }
    if (TokenVectorStorage[2][0] == '1')
    {
       HeadLight = true;
    }

    if (TokenVectorStorage[2][1] == '0')
    {
       BackLight = false;
    }
    if (TokenVectorStorage[2][1] == '1')
    {
       BackLight = true;
    }

    UpdateHardware();
    ComandStartTime = millis();
    client.print(String(EngineAThrotle)+" "+String(SteeringEngine)+
    " "+String(HeadLight)+" "+String(BackLight)+" "+String(ComandStartTime)+"\r\n" );
  }
  void CheckComandTimeout()
  {
    if ((millis() - ComandStartTime) > TimeoutTime)
    {
      Reset();
    }
  }
};

