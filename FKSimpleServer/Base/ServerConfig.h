#pragma once
//-------------------------------------------------------------
// Ĭ��DB�ļ���
#define DEFAULT_DB_FILE						"Database\\FKExample.db3"
// Ĭ�ϼ����˿�
#define DEFAULT_LISTEN_PORT					9001
// ��ʱ����ʱ���ʱ��(ms)
#define DEFAULT_TIMER_INTERVAL				100
// ����������ʱ��(ms)
#define DEFAULT_CLIENT_TICK_INTERAL			1000
// GCʱ����(ms)
#define DEFAULT_GC_TICK_INTERAL				(1000*10)
// ������ݿ���¼��ʱ��
#define DEFAULT_CLIENT_DB_TICK_INTERAL		(60*1000)
// ����Client�����շ���Ϣ�Ļ�������С
#define DEFAULT_BUF_SIZE					(1024*10)
// DB����������
#define MAX_DB_JOB_NUM						127
// �����ȴ�Socket����
#define MAX_PENDING_ACCEPT_SOCKET_NUM		127
// Log·��
#define DEFAULE_SERVER_LOG_DIR				L".\\Log"
// ���������ñ��ļ�
#define DEFAULE_SERVER_CONFIG_FILE			".\\Config\\FKSimpleServerConfig.ini"
// ����Log�ļ�
#define DEFAULT_SERVER_CRASH_FILE			".\\Exception.log"
// Ĭ�Ϸ�������
#define DEFAULT_SERVER_NAME					L"��������ʿ��������"
// �߳��˳�����ʱ�䣨ms��
#define DEFAULT_THREAD_SAFT_TERMINATE_TIME	100
