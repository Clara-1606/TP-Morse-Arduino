//TP réalisé par Clara Vesval et Giovanni Ventoso B2B

//Broches PWM
const uint8_t PWM_PINS[6] = {3,5,6,9,10,11};

//Broches ADC
const uint8_t ADC_PINS[6] = {A0,A1,A2,A3,A4,A5};

//Alphabet
const char ARR_LETTER[27] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};

//Correspondance en Morse
const String ARR_MORSE[27] = {
        ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","/"};


class Timer{
  unsigned long startTime;
  bool paused = false;
  unsigned long pauseTime;

  public :
  Timer(): paused(false),pauseTime(0){
    startTime=millis();
  }

  float getSeconds(){
    if(paused){
      return ((pauseTime -startTime)/1000.0f);
    }
    unsigned long currentTime = millis();
    return ((currentTime-startTime)/1000.0f);
  }

  void reset(){
      paused=false;
      startTime = millis();
      pauseTime = 0;
  }
};

class Led{
  uint8_t pin;
  bool allume;

  public : 
  Led():pin(0), allume(false){};

  Led(uint8_t pin) : pin(pin), allume(false){
    pinMode(pin,OUTPUT);
  }

  void on(){
    if(pin!= 0){
      digitalWrite(pin,HIGH);
    }
    allume=true;
  }

  void off(){
    if(pin!= 0){
      digitalWrite(pin,LOW);
    }
    allume=false;
  }

};

class Button{
  uint8_t pin;
  bool pressed;

  public : 
  Button() : pressed(false), pin(0){}
  Button(uint8_t pin) : pressed(false), pin(pin){
    pinMode(pin, INPUT_PULLUP);
    pressed = digitalRead(pin) == LOW;
  }

  bool isDown(){
    if(pin!= 0){
      pressed = digitalRead(pin) == LOW;
    }
    return pressed;
  }
};

class AnalogIn {
    uint8_t pin;

    public : 
    AnalogIn(uint8_t pin) : pin(pin){}

    float getValue(){
        if(!isADC(pin)){
            return 0;
        }
        int val = analogRead(pin);
        return val/ 1023.0f;
    }
};

class MorseConverter{

    String messageMorse;
    String message;

    public: 
    MorseConverter():messageMorse(""),message("") {};
    MorseConverter(String messageMorse,String message) : messageMorse(messageMorse),message(message){}


//Mettre en majuscule le message entré
    char upperCase(int letterAscii){
        if(letterAscii >= 'a' && letterAscii <= 'z'){
            letterAscii = letterAscii - 'a'; 
            letterAscii = letterAscii + 'A'; 
        }
        return letterAscii;
    }

//Conversion en morse
    bool converter(char letter){
        for (int i=0;i < 27; i++){
            if(ARR_LETTER[i]==letter){
                messageMorse += ARR_MORSE[i];
                messageMorse += " ";
                return true;
            }
        }
        
        return false;
    }

//Retourne le message en morse
    String printMessageMorse () {
        return messageMorse;
    }

//On réinitialise le message morse
    void resetMessageMorse(){
        messageMorse ="";
        
    }

//On réinitialise le message 
    void resetMessage(){
        message="";
    }

//On ajoute le signe dans le message
    void add(String morse){
        message += morse;
    }

//On récupère le message en morse qu'on va traduire en message avec les lettres
    void getMessage(){
        for (int i=0;i < 27; i++){
                if(ARR_MORSE[i]==message){
                     Serial.print(ARR_LETTER[i]);
                }
            }
           
    }
};

bool isPWM(uint8_t pin){
    for (int i = 0; i<6; i++){
        if(pin == PWM_PINS[i]){
            return true;
        }
    }
    return false;
}

bool isADC(uint8_t pin){
    for (int i = 0; i<6; i++){
        if(pin == ADC_PINS[i]){
            return true;
        }
    }
    return false;
}


Led led1;
Button button1;
Timer t;
MorseConverter morse;
AnalogIn analog(A0);
float time;
bool espace;
bool haveMessage;


void setup()
{
  led1=Led(11);
  button1=Button(8);
  Serial.begin(9600);
  time=analog.getValue()*100;
  espace =false;
  haveMessage=false;
}

void loop()
{
    int letterAscii;
    char letter;
    float cadence;
    static bool PreviousPressed; // Le bouton était-il pressé pendant la dernière boucle
    static bool CurrentlyPressed;
    String messageMorse;
    float potentiometer= analog.getValue()*1000;

//On réinitilise le messgage morse
    morse.resetMessageMorse();

//On règle le potentiomètre
    if(potentiometer!=time){
        time=potentiometer;
        for(int i = 0; i<3; i++){
            led1.on();
            delay(time);
            t.reset();
            led1.off();
            delay(time);
            cadence = t.getSeconds(); 
        }
    }
    

//Si l'utilisateur rentre quelques choses dans le moniteur série
     if(Serial.available() >0){
        led1.off();
        delay(1000);
        int lengthSerial = Serial.available(); //On détermine la taille du message à traduire

        for (int i=0;i < lengthSerial; i++){
            letterAscii=Serial.read();
            letter = (char)morse.upperCase(letterAscii);

            if(!morse.converter(letter)){ //Si il entre un caractère non supporté
                for(int i = 0; i<8; i++){
            		led1.on();
            		delay(200);
            		led1.off();
            		delay(200);
                }
            }
        }

        //On récupère le message traduit
        messageMorse = morse.printMessageMorse();

// On fait clignoté la led en lisant le message en Morse
        for(int i=0; i<messageMorse.length(); i++){
            
            if (messageMorse[i] == '.'){
                led1.on();
                delay(time);
            }
            else if(messageMorse[i] == '-'){
                    led1.on();
                    delay(3*time);
                    
            }
            else if(messageMorse[i] == '/'){
                delay(6*time); 
            }
            else{
                delay(0.5*time);
            }
            led1.off();
          delay(0.5*time);
            
        }
    } 
    else{
        PreviousPressed = CurrentlyPressed; // On prend la valeur actuelle du bouton
        CurrentlyPressed = button1.isDown();

// Si le bouton est pressé
        if (CurrentlyPressed){ 
            led1.on();
            espace=false;
            haveMessage=true;

// Si le bouton n'était pressé juste avant
            if (!PreviousPressed){
                t.reset();
            } 
        }

// S'il n'est pas pressé
        else{ 
            led1.off();

            // Si il ne l'était pas non plus avant
            if (!PreviousPressed){ 
            
                //Pour affiché la lettre
                if(t.getSeconds()>2*cadence && t.getSeconds()<3*cadence){
                    morse.getMessage();
                    morse.resetMessage();
                }

                //Pour marqué un espace
                else if(t.getSeconds()>3*cadence && t.getSeconds()<7*cadence){
                //Pour éviter de mettre plusieurs espace
                   if(!espace){
                        espace=true;
                        morse.add("/");
                    	morse.getMessage();
                     	morse.resetMessage();
                   }
                    
                }
                //Marque la fin du message
                else if (t.getSeconds()>7*cadence){
                    if(haveMessage){
                        Serial.println("");
                        Serial.println("Fin message");
                        haveMessage=false;
                    }
                    t.reset();
                }
                
                }
            else{ // Si il l'était avant
                if(t.getSeconds()<2*cadence){
                    morse.add(".");
                }
                else{
                    morse.add("-");
                }
                t.reset();
                }
        }
    }
    delay(5);
}
