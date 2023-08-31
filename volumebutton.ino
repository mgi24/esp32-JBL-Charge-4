#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "startup.h"
#include "shutdown.h"
#include "maxvol.h"

BluetoothA2DPSink a2dp_sink;
MemoryStream startupsound(startup, sizeof(startup));
MemoryStream sdsound(shutdown, sizeof(shutdown));
MemoryStream maxsound(maxvol, sizeof(maxvol));
I2SStream i2s;  // PWM output 
MP3DecoderHelix helix;
EncodedAudioStream out(&i2s, &helix); // output to decoder
StreamCopy copierstart(out, startupsound);
StreamCopy copiersd(out, sdsound);
StreamCopy copiermax(out, maxsound);

#define btled 4
#define powerled 15
#define poweren 18
#define lamp 2
#define voldown 32
#define volup 33
#define playpause 35
#define powerbtn 34
#define mute 19
#define boost 14
#define boosterpin1 23
#define boosterpin2 21
//#define voltagesens 4
int volume = 0;
int addvolume=0;
int presstime = 0;
int presstimepp = 0;
int ledstate= 0;
int presstimedown =0;
int presstimeboost =0;
int lowervolume=0;
int pausestate=0;
int boostate=0;
int isplayed=0;


  
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(lamp, OUTPUT);
  pinMode(btled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(poweren, OUTPUT);
  pinMode(mute, OUTPUT);
  pinMode(boosterpin1, OUTPUT);
  pinMode(boosterpin2, OUTPUT);
  pinMode(voldown, INPUT);
  pinMode(volup, INPUT);
  pinMode(playpause, INPUT);
  pinMode(powerbtn, INPUT);
  pinMode(boost, INPUT);
  //pinMode(voltagesens, INPUT);
  Serial.begin(9600);
  digitalWrite(btled, HIGH);
  digitalWrite(powerled, HIGH);
  digitalWrite(poweren, HIGH);
  digitalWrite(boosterpin1, HIGH);
  digitalWrite(boosterpin2, HIGH);
  digitalWrite(mute, HIGH);
  auto cfg = i2s.defaultConfig();
  cfg.sample_rate = 44100;
  cfg.channels = 1;
  i2s.begin(cfg);
  out.begin();
  Serial.println("TURN ON");
  startupsound.begin(); // restart source 
  helix.begin(); // so that we can repeatedly call this method
  
  copierstart.copyAll();
  helix.end(); // flush output
  i2s.end();
  startupsound.end();
  out.end();
  digitalWrite(mute, LOW);

    // i2s.begin(cfg);
    // out.begin();
    // Serial.println("TURN OFF");
    // sdsound.begin(); // restart source 
    // helix.begin(); // so that we can repeatedly call this method
    // copiersd.copyAll();
    // helix.end(); // flush output
    // i2s.end();
    // sdsound.end();
    // out.end();

  delay(200);
  i2s_pin_config_t my_pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 22,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    a2dp_sink.set_pin_config(my_pin_config);
    static i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100, // updated automatically by A2DP
      .bits_per_sample = (i2s_bits_per_sample_t)16,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = true,
      .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
  };

  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.set_channels(I2S_CHANNEL_MONO);
    a2dp_sink.start("JBL CHARGE 4");
    
    
}



// the loop function runs over and over again forever
void loop() {
  if (a2dp_sink.is_connected()){
    volume = a2dp_sink.get_volume();
    digitalWrite(btled, LOW);

  }
  else{
    
    digitalWrite(btled, HIGH);

  }
  //digitalWrite(lamp, HIGH);
  
  int state = analogRead(volup);
  float voltageup = (state * 5.0) / 1023.0;
  // Serial.print("VOLUP: ");
  // Serial.print(voltageup);
  int statedown = analogRead(voldown);
  float voltagedown = (statedown * 5.0) / 1023.0;
  // Serial.print(" VOLDOWN: ");
  // Serial.print(voltagedown);
  int statepp = analogRead(playpause);
  float voltagepp = (statepp * 5.0) / 1023.0;
  //  Serial.print(" PLAYPAUSE: ");
  //  Serial.println(voltagepp);
  int statepower = analogRead(powerbtn);
  float voltagepower = (statepower * 5.0) / 1023.0;
  int stateboost = analogRead(boost);
  float voltageboost = (stateboost * 5.0) / 1023.0;

  
  
   if (voltageup>1){

    if (presstime>1&&presstime<900){
      Serial.println("NEXT TRACK");
            a2dp_sink.next();
            
    }
    
    presstime=0;
    addvolume=0;
    
   
    

  }
  if (voltagedown>1){
    if (presstimedown>1&&presstimedown<900){
      Serial.println("PREV TRACK");
      a2dp_sink.previous();
    }
     presstimedown=0;
      lowervolume=0;
      

  }

  if (voltageup>1&&voltagedown>1){
      ledstate=0;
      digitalWrite(lamp, LOW);
  }



  if (voltageup<1){//pressed 1
    
    int voltresh=1000*(addvolume+1);

    if (presstime>voltresh){
      addvolume++;
      
      if (volume<130){
        volume=volume+10;
        a2dp_sink.set_volume(volume);
        
      }
      if (volume>130){
        volume=130;
  
            
        if (ledstate==0){
          ledstate = 1;
          //digitalWrite(LED_BUILTIN, LOW);
        }
        else{
          
           ledstate=0;
           //digitalWrite(LED_BUILTIN, HIGH);
            
        }
        Serial.println("MAX VOLUME!!!!");
        
      }
      Serial.print("VOLUME = ");
      Serial.print(volume);
      Serial.print("LED STATE = ");
      Serial.println(ledstate);
    
    }
    delay (1);
    presstime++;
    
    
    
   
    
    if (ledstate==0){
          
          digitalWrite(lamp, HIGH);
        }
        if (ledstate==1){
           
           digitalWrite(lamp, LOW);
           
        } 
      
      
      
  }
//////////VOLDOWN///////////////////////
  if (voltagedown<1){//pressed 1
    
    int voltresh=1000*(lowervolume+1);

    if (presstimedown>voltresh){
      lowervolume++;
      
      if (volume>0){
        volume=volume-10;
        if (volume<=0){
        volume=0;}
        a2dp_sink.set_volume(volume);
        
      }
      if (volume<=0){
        
        if (ledstate==0){
          ledstate = 1;
          //digitalWrite(LED_BUILTIN, LOW);
        }
        else{
           ledstate=0;
           //digitalWrite(LED_BUILTIN, HIGH);
           
        }
        Serial.println("LOWEST VOLUME!!!!");
        
      }
      Serial.print("VOLUME = ");
      Serial.print(volume);
      Serial.print("LED STATE = ");
      Serial.println(ledstate);
    
    }
    delay (1);
    presstimedown++;
    
    
    
   
    
    if (ledstate==0){
          
          digitalWrite(lamp, HIGH);
        }
        if (ledstate==1){
           
           digitalWrite(lamp, LOW);
           
        } 
      
      
      
  }
  //PLAY PAUSE BTN//
  if (voltagepp>1){

    if (presstimepp>10){
      if (pausestate==0){
        pausestate=1;
        Serial.println("PAUSE");
        a2dp_sink.pause();
      }
      else{
        pausestate=0;
        Serial.println("PLAY");
        a2dp_sink.play();
      }
      
    }
    
    presstimepp=0; 
   
    

  }
  if (voltagepp<1){
    presstimepp++;
  }
  
//ON OFF BUTTON//
  if (voltagepower>1){
    
    a2dp_sink.stop();
    digitalWrite(mute, HIGH);
    i2s.end();
    
    auto cfg = i2s.defaultConfig();
    cfg.sample_rate = 44100;
    cfg.channels = 1;
    i2s.begin(cfg);
    out.begin();
    Serial.println("TURN OFF");
    sdsound.begin(); // restart source 
    helix.begin(); // so that we can repeatedly call this method
    copiersd.copyAll();
    helix.end(); // flush output
    i2s.end();
    startupsound.end();
    out.end();
            



    
    
    digitalWrite(poweren, LOW);
  }

  //BOOSTERPIN//

  if (voltageboost<1){
    presstimeboost++;
  }
  

  if (voltageboost>1){

    if (presstimeboost>10){
      if (boostate==0){
        boostate++;
        Serial.println("+6Db");
        digitalWrite(boosterpin1, LOW);
        digitalWrite(boosterpin2, HIGH);
      }
      else{
        if (boostate==1){
          boostate++;
          digitalWrite(boosterpin1, HIGH);
          digitalWrite(boosterpin2, LOW);
          Serial.println("+12Db");
        }
        else{
          boostate=0;
          digitalWrite(boosterpin1, HIGH);
          digitalWrite(boosterpin2, HIGH);
        }
        
      }
      
    }
    
    presstimeboost=0; 
   
    

  }
////MUTE PIN////
  if (a2dp_sink.get_audio_state()==2){
    digitalWrite(mute, HIGH);
  }
  else{
    digitalWrite(mute, LOW);
  }
  
  

}
