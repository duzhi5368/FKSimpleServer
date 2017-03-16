#pragma once
//------------------------------------------------------------------------
#include "BaseDepend.h"
#include "ServerConfig.h"
//------------------------------------------------------------------------
#ifdef _DEBUG
#define EXCEPTION_ON
#endif
//------------------------------------------------------------------------
#ifndef EXCEPTION_ON
	#define TRY_BEGIN
	#define TRY_END
	#define TRY_END_NOTHIS
	#define	TRY_INIT
	#define	TRY_END_RETURN(ret)
#else
#pragma warning( disable : 4313 4297 4244 4355 4311 4312 4172 )
#define	TRY_INIT	{ \
								FILE * fp = ::fopen( DEFAULT_SERVER_CRASH_FILE, "a+" );\
								if( fp )\
																								{\
									SYSTEMTIME	stNow;\
									GetLocalTime( &stNow );\
									fprintf( fp, "\n[%04u-%02u-%02u %02u:%02u:%02u.%03u] [BeginToCache]\n",\
									stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds);\
									fclose( fp );\
																								}\
																												}
#define TRY_BEGIN try {
#define TRY_END	} \
				catch(...) \
																{ \
				FILE * fp = ::fopen( DEFAULT_SERVER_CRASH_FILE,"a+"); \
					if(fp) \
															{ \
						unsigned long nLen = __LINE__;\
						char temp[1024];\
						SYSTEMTIME	stNow;\
						GetLocalTime( &stNow );\
						::sprintf(temp,"[%04u-%02u-%02u %02u:%02u:%02u.%03u] %s %05d 0x%08x ", \
						stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds\
						,__FILE__, nLen, (LPVOID)this );\
						::fwrite(temp,strlen(temp),1,fp);\
						::fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						::fwrite("\r\n",2,1,fp); \
						::fclose(fp); \
															} \
					throw; \
																}
#define TRY_END_NOTHIS	} \
	catch(...) \
																{ \
				FILE * fp = ::fopen( DEFAULT_SERVER_CRASH_FILE,"a+"); \
				if(fp) \
													{ \
					unsigned long nLen = __LINE__;\
					char temp[1024];\
					SYSTEMTIME	stNow;\
					GetLocalTime( &stNow );\
					::sprintf(temp,"[%04u-%02u-%02u %02u:%02u:%02u.%03u] %s %05d ", \
					stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds\
					,__FILE__, nLen );\
					::fwrite(temp,strlen(temp),1,fp);\
					::fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
					::fwrite("\r\n",2,1,fp); \
					::fclose(fp); \
													} \
					throw; \
																}
#define TRY_END_RETURN(ret)	} \
				catch(...) \
																{ \
				FILE * fp = ::fopen( DEFAULT_SERVER_CRASH_FILE,"a+"); \
					if(fp) \
															{ \
						unsigned long nLen = __LINE__;\
						char temp[1024];\
						SYSTEMTIME	stNow;\
						GetLocalTime( &stNow );\
						::sprintf(temp,"[%04u-%02u-%02u %02u:%02u:%02u.%03u] %s %05d 0x%08x ", \
						stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds\
						,__FILE__, nLen, (LPVOID)this );\
						::fwrite(temp,strlen(temp),1,fp);\
						::fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						::fwrite("\r\n",2,1,fp); \
						::fclose(fp); \
															} \
					return (ret); \
																}
#endif
//------------------------------------------------------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
//------------------------------------------------------------------------
#define INFO_COLOR		RGB(0, 98, 250)
#define ERROR_COLOR		RGB(186, 40, 53)
//------------------------------------------------------------------------
void INFO_MSG(const TCHAR* x, ...);
void INFO_MSG_LOG(const TCHAR* x, ...);
void INFO_MSG_LOG_WITH_TIME(const TCHAR* x, ...);
void ERROR_MSG_LOG_WITH_TIME(const TCHAR* x, ...);
//------------------------------------------------------------------------
bool CONFIM_MESSAGE_BOX(TCHAR * pszCaption, TCHAR * pszText);
//------------------------------------------------------------------------
void SET_THREAD_NAME(DWORD dwThreadID, const char* threadName);
//------------------------------------------------------------------------
