#pragma once
//-------------------------------------------------------------
// 默认DB文件名
#define DEFAULT_DB_FILE						"Database\\FKExample.db3"
// 默认监听端口
#define DEFAULT_LISTEN_PORT					9001
// 定时器定时间隔时间(ms)
#define DEFAULT_TIMER_INTERVAL				100
// 玩家心跳间隔时间(ms)
#define DEFAULT_CLIENT_TICK_INTERAL			1000
// GC时间间隔(ms)
#define DEFAULT_GC_TICK_INTERAL				(1000*10)
// 玩家数据库更新间隔时间
#define DEFAULT_CLIENT_DB_TICK_INTERAL		(60*1000)
// 单个Client对象收发消息的缓冲区大小
#define DEFAULT_BUF_SIZE					(1024*10)
// DB任务最大个数
#define MAX_DB_JOB_NUM						127
// 监听等待Socket个数
#define MAX_PENDING_ACCEPT_SOCKET_NUM		127
// Log路径
#define DEFAULE_SERVER_LOG_DIR				L".\\Log"
// 服务器配置表文件
#define DEFAULE_SERVER_CONFIG_FILE			".\\Config\\FKSimpleServerConfig.ini"
// 错误Log文件
#define DEFAULT_SERVER_CRASH_FILE			".\\Exception.log"
// 默认服务器名
#define DEFAULT_SERVER_NAME					L"【自由骑士服务器】"
// 线程退出缓冲时间（ms）
#define DEFAULT_THREAD_SAFT_TERMINATE_TIME	100
