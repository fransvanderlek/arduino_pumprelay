/* Sweep
using the digitalWrite to set/unset relay pins.
 */

int setPin = 12;
int unsetPin = 13;

void setup() {
	pinMode(setPin, OUTPUT);
	pinMode(unsetPin, OUTPUT);

	digitalWrite(setPin, LOW);
	digitalWrite(unsetPin, LOW);

}

void loop() {
	pumpOn();
	delay(3000);
	pumpOff();
	delay(5000);
}


void activateRelayPin(int pin){
	digitalWrite(pin, HIGH);
	delay(10);
	digitalWrite(pin, LOW);
}

void pumpOn(){
	activateRelayPin( setPin);
}

void pumpOff(){
	activateRelayPin( unsetPin);

}





