#ifndef GSM_h
#define GSM_h

/* GSM Class */
class GSM {

public:
	// Pin definition
	static int powerkey;
	int userkey;

	GSM();
	~GSM();

	// SIM query
	void PowerOn(int PowerKey);

	// Phone calls
	void PhoneCall(const char* PhoneNumber);

	// SMS sending and receiving message 
	bool SendingShortMessage(const char* PhoneNumber,const char* Message);
	bool ReceivingShortMessage();

	// FTP download file to Module EFS or uploading EFS file to FTP
	void ConfigureFTP(const char* FTPServer,const char* FTPUserName,const char* FTPPassWord);
	void UploadToFTP(const char* FileName);
	void DownloadFromFTP(const char* FileName);

	// GPS positoning
	bool GPSPositioning();

	//TCP and UDP communication
//	bool PDPSetting(const char* APN);
//	bool TCPClientCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode
//	bool UDPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //UDP Client Command Mode
//	bool TCPServerCM(const char* ServerIP,const char* Port,const char* Message,const char* MessageSize);  //TCP Client Command Mode

	// Other functions.
	char sendATcommand(const char* ATcommand, unsigned int timeout);
	char sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout);
	char sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout);
};

extern GSM sim7600;

#endif
