/*
	Andrew Daws
	4/21/2014
	Rev. 3.0

	Purpose:
	The objective of this project was to simulate the process of bottling wine.
	This is achieved through four stations.

	The filling module is the first station that the bottle encounters.
	The objective of this station is to fill the bottle with the correct wine.
	In order to do so, a color sensor is used to determine weather the bottle is
	made of clear glass, or dark green glass. The dark green glass is used for
	red wine, while the clear glass contains white wine.

	After the bottle is filled the conveyor belt moves the bottle to the next
	station. At the corking station an actuator seals the bottle by placing a
	cork in it.

	When this is completed the conveyor belt moves the bottle to the necking
	station. At this station label is placed on the neck of the bottle. A color
	sensor is used in order to ensure the correct label is placed on the bottle.

	Once this is completed the conveyor belt moves the bottle to the final
	station. At this station a label is placed on the main body of the bottle.
	A color sensor is also used at this station to ensure the correct label is
	placed the bottle.
*/

#include <vxWorks.h> /* Always include this as the first thing in every program */
#include <logLib.h> /* Use logMsg */
#include <semLib.h> /* Use semaphores */
#include <stdio.h> /* Use printf */
#include <sysLib.h> /* Use sysClk... */
#include <taskLib.h> /* Use tasks */
#include <time.h> /* Use clock_gettime */
#include <wdLib.h> /* Use watchdog */


/*Globals*/
semID semBin /*Id for the semaphore*/
WDOG_ID wdID; /* Id for the watchdog used*/
#define AUTO 0 /*Allows for auto mode*/
#define MANUAL 1 /*Allows for manual mode*/
#define CLOCK_REALTIME 0 /*To keep track of system time*/

/*Variables */
int conveyer = 1; /*1 to start, 0 to stop*/
int Sensors; /*Initalize sensor value to zero*/
int Actuators;

struct timespec{ /*timeSpec structure to allow for timestamps*/
	time_t tv_sec
	time_t tv_nsec
} tstamp;

run (){//ask for mode 0 auto 1 maunual
	/*Create a semaphor and Watchdog to control the conveyer variable in Auto mode*/
	semBin=SemBCreate(0,0);
	wdId = wdCreate();
	initiateProcess();

	/*Spawn all manual tasks*/
	taskSpawn("manual",100,0x100,2000,(FUNCPTR)station1,AUTO,0,0,0,0,0,0,0,0);

	/*Spawn all Auto Tasks*/
	taskSpawn("autoFill",100,0x100,2000,(FUNCPTR)station1,AUTO,0,0,0,0,0,0,0,0);
	taskSpawn("autoCork",100,0x100,2000,(FUNCPTR)station2,AUTO,0,0,0,0,0,0,0,0);
	taskSpawn("autoNeck",100,0x100,2000,(FUNCPTR)station3,AUTO,0,0,0,0,0,0,0,0);
	taskSpawn("autoLabel",100,0x100,2000,(FUNCPTR)station4,AUTO,0,0,0,0,0,0,0,0);
	taskSpawn("autoBelt",100,0x100,2000,(FUNCPTR)belt,AUTO,0,0,0,0,0,0,0,0);

	/*Start in Auto Mode, so suspend all manual tasks immediately*/
	taskSuspend manual;


	/*Use an interrupt to allow switch between Auto mode and Manual Mode*/
	/*****use scanf to enable inputs through the keyboard******/

	/*spawn random bottle*/

}

/*fill*/
station1(int auto){
	while(1){
		/*Check bottle color at station one:
		Sensor Value 0x0011 Dark Bottle, 0x0021 Light Bottle*/
		getSensorValues();
		if(Sensors == 0x0011 || Sensors == 0x0021) {
			semTake(semBin); /*Protect the conveyer variable*/
			conveyer = 0; /*Stop Conveyer belt*/
			/*Fill bottle: Output 0x0001 Fill Red Wine, 0x0002 Fill White Wine*/
			if(Sensors == 0x0011){
				Actuators = 0x0001;
				calculateAndSetActuators(Actuators);
			}
			else if (Sensors == 0x0012){
				Actuators = 0x0002;
				calculateAndSetActuators(Actuators);
			}
		}
		/*When finished: Sensor Value 0x0051 and 0x0061*/
		if (Sensors == 0x0051 || Sensors == 0x0061){
			conveyer = 1; /*Start conveyer belt*/
			semGive(semBin);
			taskDelay(60); /*Delay for a full second*/
		}
	}
}



/*cork*/
station2(int auto){
	while(1){
		/*Bottle color irrelivant: Sensor Value 0x0012 and 0x0022*/
		getSensorValues();
		if(Sensors == 0x0012 || Sensors == 0x0022) {
			semTake(semBin); /*Protect the conveyer variable*/
			conveyer = 0; /*Stop Conveyer belt*/
			/*Cork Bottle: Output 0x0004*/
			Actuators = 0x0004;
			calculateAndSetActuators(Actuators);
			/*When Finished: Sensor Value 0x0052 and 0x0062*/
		}
		if(Sensors == 0x0052 || Sensors == 0x0062){
			conveyer = 1;
			semGive(semBin);
			taskDelay(60); /*Delay by a full second*/
		}
	}
}

/*neck label*/
station3(int auto){
	while(1){
		/*Check bottle color at station:
		Sensor Value 0x0014 Dark bottle, 0x0024 Light bottle*/
		getSensorValues();
		if(Sensors == 0x0014 || Sensors == 0x0024) {
			semTake(semBin); /*Protect the conveyer variable*/
			conveyer = 0; /*Stop Conveyer belt*/
		/*Neck label: Output 0x0008 for Red Wine, 0x0010 for White Wine*/
			if(Sensors == 0x0014){
				Actuators = 0x0008;
				calculateAndSetActuators(Actuators);
			}
			else if (Sensors == 0x0024){
				Actuators = 0x0010;
				calculateAndSetActuators(Actuators);
			}
		}
		/*When finished: Sensor Value 0x0054 and 0x0064*/
		if (Sensors == 0x0054 || Sensors == 0x0064){
			conveyer = 1; /*Start conveyer belt*/
			semGive(semBin);
			taskDelay(60); /*Delay for a full second*/
		}
	}
}


station4(int){ /*Bottle Label*/

	while(1){
		/*Check bottle color at station:
		Sensor Value 0x0018 Dark bottle, 0x0028 Light bottle*/
		getSensorValues();
		if(Sensors == 0x0018 || Sensors == 0x0028) {
			semTake(semBin); /*Protect the conveyer variable*/
			conveyer = 0; /*Stop Conveyer belt*/
		/*Bottle label: Output 0x0020 for Red Wine, 0x0040 for White Wine*/
			if(Sensors == 0x0018){
				Actuators = 0x0020;
				calculateAndSetActuators(Actuators);
			}
			else if (Sensors == 0x0028){
				Actuators = 0x0040;
				calculateAndSetActuators(Actuators);
			}
		}
		/*When finished: Sensor Value 0x0058 and 0x0068*/
		if (Sensors == 0x0058 || Sensors == 0x0068){
			conveyer = 1; /*Start conveyer belt*/
			semGive(semBin);
			taskDelay(60); /*Delay for a full second*/
		}
	}
}


belt(int){
	while(1){
		/*Move the conveyer belt: When conveyer = 1, Output 0x0080*/
		if (conveyer == 1){
			Actuators = 0x0080;
			calculateAndSetActuators(Actuators);
		}
		else {

		}
	}
}

void setActuatorValues(int Actuators){
  sysOutByte(0x184,0x01); /* Re enable outputs */
  sysOutByte(0x183,(Actuators & 0xFF00) >> 8);
  sysOutByte(0x181,(Actuators & 0x00FF));
}

void calculateAndSetActuators(int inputs){
  Actuators = inputs;
  setActuatorValues(Actuators);
  printf(" Actuators %04x",Actuators);
}

int getSensorValues() {
  int Sensors = 0;
  sysOutByte(0x184,0x00); /* Disable outputs */
  Sensors = (sysInByte(0x182) << 8) | (sysInByte(0x180));
  sysOutByte(0x184,0x01); /* Re enable outputs */
  return Sensors;
}

void initiateProcess(){
  sysOutByte(0x180,0x00); /* Zero out the four channels */
  sysOutByte(0x181,0x00);
  sysOutByte(0x182,0x00);
  sysOutByte(0x183,0x00);
  sysOutByte(0x184,0x01); /* Enable Output */
  Actuators = 0;
}
