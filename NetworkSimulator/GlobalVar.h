#pragma once
//-------------------------------------------------------------
namespace GlobalVar  
{
	enum ENUM_LinkMode
	{
		eLinkMode_UDP	= 0,
		eLinkMode_TCPClient	= 1,
		eLinkMode_TCPServer	= 2,

		eLinkMode_Max,
	};
	static ENUM_LinkMode	s_eLinkMode				= eLinkMode_UDP;
	static unsigned char    s_SendDataBuffer[4096]	= { 0 };
	static CString			s_Temp_Send_Sigel[2048];
	static bool				s_bIsUdpStart			= false;
	static bool				s_bIsTcpClientStart		= false;
	static bool				s_bIsTcpServerStart		= false;
	static CString			s_strServiceTypeName;
};

#define			WM_PROCESS_SHOW_RECEIVE 2001
#define			WM_TIMER_ID				1001

#define			TCP_SERVER_NAME			"TCP_Server"
#define			TCP_CLIENT_NAME			"TCP_Client"
#define			UDP_NAME				"UDP"

#define			DEFAULT_LISTEN_PORT		10086
#define			DEFAULT_REMOTE_PORT		9001
#define			DEFAULT_REMOTE_IP		"127.0.0.1"
#define			DEFAULT_PROTOCAL_TYPE	"TCP_Client"
#define			DEFAULT_AUTO_SEND_TIME	500
#define			DEFAULT_SEND_TYPE		1
#define			DEFAULT_RECEIVE_TYPE	1
#define			DEFAULT_SHOW_SEND_MSG	1

#define			DEFAULT_CONFIG_FILE		".\\Config\\NetworkSimulatorConfig.ini"