
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;


#define TCPIP_INTERFACE_RESET_SECONDS_TIME		(5)		//If interface is not connected for # seconds cause a reset of the interface to ensure it will reconnect to new connections
#define TCPIP_INTERFACE_CHECK_SECONDS_TIME		2				//Check the conencterion every # seconds (so we can flag to our applicaiton if it is connected or not)

int wlan0_is_connected = 0;
int wlan0_connection_check_state_1sec_timer = 3;			//Initial check delay after startup
int wlan0_connection_do_reset_1sec_timer = TCPIP_INTERFACE_RESET_SECONDS_TIME;

string do_console_command_get_result (char* command)
{
	FILE* pipe = popen(command, "r");
	if (!pipe)
		return "ERROR";
	
	char buffer[128];
	string result = "";
	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return(result);
}


int main(){
while(1){
    sleep(1);
	//##### ADD TO 1 SECOND HEARTBEAT #####
	if (wlan0_connection_check_state_1sec_timer)
		wlan0_connection_check_state_1sec_timer--;

	if (wlan0_connection_do_reset_1sec_timer)
		wlan0_connection_do_reset_1sec_timer--;
	
	
	//----------------------------------
	//----------------------------------
	//----- CHECK WLAN0 CONNECTION -----
	//----------------------------------
	//----------------------------------
	if (wlan0_connection_check_state_1sec_timer == 0)
	{
		//--------------------------------------
		//----- CHECK THE CONNECTION STATE -----
		//--------------------------------------
		wlan0_connection_check_state_1sec_timer = TCPIP_INTERFACE_CHECK_SECONDS_TIME;

		string CommandResult = do_console_command_get_result((char*)"cat /sys/class/net/wlan0/operstate");
		if (CommandResult.find("up") == 0)		//If first character is '1' then interface is connected (command returns: '1', '0' or a 'not found' error message)
		{
			if (!wlan0_is_connected)
				cout << "wlan0 is now connected" << endl;
			
			wlan0_is_connected = 1;
			wlan0_connection_do_reset_1sec_timer = TCPIP_INTERFACE_RESET_SECONDS_TIME;		//We don't want to reset the connection while it is connected
		}
		else
		{
			if (wlan0_is_connected)
			{
				cout << "wlan0 is no longer connected" << endl;
				wlan0_connection_do_reset_1sec_timer = 0;									//<Do a reset quite quickly after a good connection is lost
			}
			wlan0_is_connected = 0;
		}
	}
	
	/*if (wlan0_connection_do_reset_1sec_timer == 0)
	{
		//------------------------------------------
		//----- NOT CONNECTED - RESET INTERACE -----
		//------------------------------------------
		wlan0_connection_do_reset_1sec_timer = TCPIP_INTERFACE_RESET_SECONDS_TIME;
		wlan0_connection_check_state_1sec_timer = TCPIP_INTERFACE_CHECK_SECONDS_TIME;
		
		std::cout << "Resetting wlan0 interface (currently not connected, ensure it will re-connect when possible)" << std::endl;
		system("sudo ip link set wlan0 down &");			//'&' means do in background
		system("sudo ip link set wlan0 up &");			//'&' means do in background
		std::cout << "Reset wlan0 was started" << std::endl;

		//The wlan0 will now pick up a new connection when next able to (it tries, then sleeps, but keeps checking for a known WiFi access point).
		//We do this reset every now and then just to be sure it the interface is always trying
	}*/
	}
}