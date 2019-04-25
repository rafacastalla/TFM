/**
 * Usage, according to documentation(https://www.firediy.fr/files/drone/HW-01-V4.pdf) : 
 *     1. Plug your Arduino to your computer with USB cable, 
 *     open terminal, then type 1 to send max throttle to every 
 *     ESC to enter programming mode
 *     2. Power up your ESCs. You must hear "beep1 beep2 beep3" 
 *     tones meaning the power supply is OK
 *     3. After 2sec, "beep beep" tone emits, meaning the throttle 
 *     highest point has been correctly confirmed
 *     4. Type 0 to send min throttle
 *     5. Several "beep" tones emits, which means the 
 *     quantity of the lithium battery cells (3 beeps for a 3 cells LiPo)
 *     6. A long beep tone emits meaning the throttle 
 *     lowest point has been correctly confirmed
 *     7. Type 2 to launch test function. 
 *     This will send min to max throttle to ESCs to test them
 *
 * @author lobodol <grobodol@gmail.com>
 */
// ---------------------------------------------------------------------------
char data;
// ---------------------------------------------------------------------------
const byte pwm[4] = {6,7,8,9};
const float cv = 0.4096; // 4096 / 10.000 us

const int minPWM = 1000;
const int maxPWM = 2000;
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void writePWM(int p, float us){
  if(us > maxPWM) us = maxPWM;
  else if(us < minPWM) us = minPWM; 

  analogWrite(p, cv * us);
}

/**
 * Initialisation routine
 */
void setup() {  
    analogWriteResolution(12);
    Serial.begin(115200);
    while(!Serial) {}    
    displayInstructions();
}

/**
 * Main function
 */
void loop() {
    if (Serial.available()) {
        data = Serial.read();

        switch (data) {
            // 0
            case 48 : Serial.println("Sending minimum throttle");
                      writePWM(pwm[0], minPWM);
                      writePWM(pwm[1], minPWM);
                      writePWM(pwm[2], minPWM);
                      writePWM(pwm[3], minPWM);
            break;

            // 1
            case 49 : Serial.println("Sending maximum throttle");
                      writePWM(pwm[0], maxPWM);
                      writePWM(pwm[1], maxPWM);
                      writePWM(pwm[2], maxPWM);
                      writePWM(pwm[3], maxPWM);
            break;

            // 2
            case 50 : Serial.print("Running test in 3");
                      delay(1000);
                      Serial.print(" 2");
                      delay(1000);
                      Serial.println(" 1...");
                      delay(1000);
                      test();
            break;
        }
    }
    

}

/**
 * Test function: send min throttle to max throttle to each ESC.
 */
void test()
{
    for (int i = minPWM; i <= maxPWM; i += 5) {
        Serial.print("Pulse length = ");
        Serial.println(i);
        writePWM(pwm[0], i);
        writePWM(pwm[1], i);
        writePWM(pwm[2], i);
        writePWM(pwm[3], i);        
        delay(200);
    }

    Serial.println("STOP");
    writePWM(pwm[0], minPWM);
    writePWM(pwm[1], minPWM);
    writePWM(pwm[2], minPWM);
    writePWM(pwm[3], minPWM);
}

/**
 * Displays instructions to user
 */
void displayInstructions()
{  
    Serial.println("READY - PLEASE SEND INSTRUCTIONS AS FOLLOWING :");
    Serial.println("\t0 : Send min throttle");
    Serial.println("\t1 : Send max throttle");
    Serial.println("\t2 : Run test function\n");
}
