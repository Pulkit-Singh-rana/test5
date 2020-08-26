#include "kyron_input.h"
#include "kyron_display.h"

int aState;
int aLastState;

uint8_t system_setting_mode=NONE;


void SetupInputs(void)
{
	pinMode (ROTARY_A_PIN,INPUT);
	pinMode (ROTARY_B_PIN,INPUT);
  pinMode (ROTARY_SELECT_PIN,INPUT);
  pinMode (SET_FLOW_BUTTON,INPUT);
  pinMode (SET_O2_BUTTON,INPUT);
	aLastState = digitalRead(ROTARY_A_PIN); 
}

bool InSettingMode(void)
{
	if(system_setting_mode==NONE)
	{
		if(digitalRead(SET_FLOW_BUTTON)==LOW) //For Pull UP type
		{
			system_setting_mode=SYSTEM_FLOW;
			StartDisplayBlink(FLOW_DISPLAY);
			return true;
		}
		if(digitalRead(SET_O2_BUTTON)==LOW)//For Pull UP type
		{
			system_setting_mode=SYSTEM_O2;
			StartDisplayBlink(O2_DISPLAY);
			return true;
		}
		return false;
	}
	else
		return true;

}

void SetSystemTarget(uint8_t &target_flow, uint8_t &target_o2)
{

	aState = digitalRead(ROTARY_A_PIN); // Reads the "current" state of the outputA
	// If the previous and the current state of the outputA are different, that means a Pulse has occured
	if (aState != aLastState)
	{     
		// If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
		if (digitalRead(ROTARY_B_PIN) != aState)
		{
      if(system_setting_mode == SYSTEM_FLOW)
      {
        if(target_flow<=MIN_FLOW)  target_flow=MIN_FLOW;
        else target_flow--;
      }
      else if(system_setting_mode == SYSTEM_O2)
		  {
        if(target_o2<=MIN_O2)target_o2=MIN_O2;
        else target_o2--;
		  }

			
		} else
		{
			if(system_setting_mode == SYSTEM_FLOW)
      {
        if(target_flow>=MAX_FLOW)  target_flow=MAX_FLOW;
        else target_flow++;
      }
      else if(system_setting_mode == SYSTEM_O2)
      {
        if(target_o2>=MAX_O2)    target_o2=MAX_O2;
        else target_o2++;
      }
		}
       //Serial.print("Target Flow : ");//Serial.println(target_flow);
      //Serial.print("Target O2 : ");//Serial.println(target_o2);

	} 
	aLastState = aState; // Up
}

bool SystemTargetSetComplete(void)
{
  
	if(digitalRead(ROTARY_SELECT_PIN)==LOW)
	{
		system_setting_mode=NONE;
		StopDisplayBlink();
		return true;
	}
	else
		return false;
}
