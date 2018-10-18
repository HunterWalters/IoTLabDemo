#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

/* File: presentationDemo.c
 * Author: Hunter Walters
 * Course: Internet of Things
 * Date: 10/18/2018
 * Description: When you press the button, it calls your phone. More specifically,
 * it uses libcurl to trigger a webhook that calls whatever phone is connected
 * to the IFTTT account associated with the URL.
 * Sources: I don't usually included this, but the documentation @curl.haxx.se/libcurl/c/
 * was a massive help in undertstanding libcurl and how to use it.
 */

//Global LED pins, and button
const int RED_LED = 14;
const int YELLOW_LED = 15;
const int GREEN_LED = 18;
const int BUTTON = 7;

//Flashes all LEDs a given # of times, for a given amount of time
void flashLEDs(int numOfFlashes, int delayTime);
/*Does all libcurl things, including initializationa and cleanup. For extended use,
it should probably take the URL as a parameter, but its hardcoded for our purposes.
Detailed comments inside the function */
void performPhoneCall();

int main(void){

	//wiringPi error handling
	if(wiringPiSetupGpio() == -1){
		printf("wiringPi setup failed\n");
		exit(1);
	}

	//Setting pinModes for everything
	pinMode(RED_LED, OUTPUT);
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(BUTTON, INPUT);

	int delayTime = 100;

	//Loops forever. The equivelant of photons "void loop()"
	while(1){
		if(digitalRead(BUTTON) == PUD_OFF){	//If button is pressed
			flashLEDs(1, delayTime);
			performPhoneCall();
			flashLEDs(5, delayTime);
		}
	}
}

void flashLEDs(int numOfFlashes, int delayTime){
	for(int i = 0; i < numOfFlashes; i++){
		digitalWrite(RED_LED, HIGH);
		digitalWrite(YELLOW_LED, HIGH);
		digitalWrite(GREEN_LED, HIGH);
		delay(delayTime);
		digitalWrite(RED_LED, LOW);
		digitalWrite(YELLOW_LED, LOW);
		digitalWrite(GREEN_LED, LOW);
		delay(delayTime);
	}
}

void performPhoneCall(){
	//Creating a handle
	CURL * handle;
	CURLcode response;

	//Initializing the easy_handle, NULL return means an error, hince the error handling
	handle = curl_easy_init();
	if(!handle){
		printf("curl handle failure\n");
		exit(1);
	}


	//Sets up the curl command, using the URL you give. CURLOPT_URL is mandatory and from libcurl
	curl_easy_setopt(handle, CURLOPT_URL, "https://maker.ifttt.com/trigger/buttonPressed/with/key/elimBRB7rLrFHexV4lxP8");

	/*Performs the curl command as if you did it from the command line. Response holds potential error codes.
	Should return the equivelant of CURLE_OK, if not, something went wrong */
	response = curl_easy_perform(handle);
	if(response != CURLE_OK){
		printf("CURL response invalid, perform failed maybe?\n");
		exit(1);
	}

	//Cleans up the mess this no doubt made, freeing that memory for future use
	curl_easy_cleanup(handle);
}
