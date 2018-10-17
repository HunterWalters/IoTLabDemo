#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

/* File: presentationDemo.c
 * Author: Hunter Walters
 * Course: Internet of Things
 * Date: 10/15/2018
 * Description: 
 */

const int RED_LED = 14;
const int YELLOW_LED = 15;
const int GREEN_LED = 18;
const int BUTTON = 7;

void flashLEDs(int numOfFlashes, int delayTime);

int main(void){

	if(wiringPiSetupGpio() == -1){
		printf("wiringPi setup failed\n");
		exit(1);
	}

	pinMode(RED_LED, OUTPUT);
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(BUTTON, INPUT);

	int delayTime = 100;

	while(1){
		if(digitalRead(BUTTON) == PUD_OFF){

			CURL * handle;
        		CURLcode response;

        		handle = curl_easy_init();

        		if(!handle){
                		printf("curl handle failure\n");
                		exit(1);
			}

			curl_easy_setopt(handle, CURLOPT_URL, "https://maker.ifttt.com/trigger/buttonPressed/with/key/elimBRB7rLrFHexV4lxP8");
			response = curl_easy_perform(handle);

			if(response != CURLE_OK){
				printf("CURL response invalid, perform failed maybe?\n");
				exit(1);
			}

			flashLEDs(5, delayTime);
			curl_easy_cleanup(handle);
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
