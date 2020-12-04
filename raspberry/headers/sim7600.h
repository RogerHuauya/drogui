#ifndef sim7x00_h
#define sim7x00_h
#include <string>
/* Sim7x00 Class */
class Sim7600 {

public:
	// Pin definition
	static int powerkey;
	int userkey;
	
	Sim7600();
	~Sim7600();

	// SIM query
	void PowerOn(int PowerKey);

	// Phone calls
	void PhoneCall(const char* PhoneNumber);

	// SMS sending and receiving message 
	bool SendingShortMessage(const char* PhoneNumber,const char* Message);
	bool ReceivingShortMessage();

	// GPS positoning
	float Lat,Log;
	float pos_x, pos_y;
	float offset_x,offset_y;
	bool GPSPositioning();
	void GPSStart();
	bool GPSGet();
	//TCP and UDP communication
//	bool PDPSetting(const char* APN);
//	bool TCPClientCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode
//	bool UDPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //UDP Client Command Mode
//	bool TCPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode

	// Other functions.
	char sendATcommand(const char* ATcommand, unsigned int timeout, bool debug);
	char sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout, bool debug);
	char sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout);
};

extern Sim7600 sim7600;

#endif
