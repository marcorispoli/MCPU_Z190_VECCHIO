#include "api_log.h"
#include "api_socket.h"

namespace API
{
	namespace RTCS
	{
		#define WATCHDOG_TIMEOUT_R2CP_READ_TASK		100
	
		#define SIZEOF_BUFFER_NODECODER				ipconfigTCP_MSS
		#define SIZEOF_BUFFER_R2CP					100
		#define HEADER_SIZEOF_R2CP					8
		#define HEADER_LENGH_R2CP					7
	
		inline word MESSAGE_LENGTH(byte *header)
		{
			return ((header[HEADER_LENGH_R2CP - 1] & 0xff00) << 8) | (header[HEADER_LENGH_R2CP]  & 0x00ff);
		}

#ifdef 	__MQX__
		void AcceptTask(dword pParam)
#elif  WIN32
		DWORD AcceptTask(LPVOID pParam)
#else
		void AcceptTask( void * pParam )
#endif
		{
			
			CaSocket *pSocket = reinterpret_cast<CaSocket*> (pParam);
			
			Socket_t serviceSocket;
		
			pSocket->m_acceptThreadState = RUNNING;
			do {
				//memset(&pSocket->m_remoteAddress, 0, sizeof(OS::sockaddr_str));
				serviceSocket = FreeRTOS_accept( pSocket->m_sock , nullptr  , 0);			
				if( serviceSocket == nullptr )
				{
					break;
				}
				//Accepts a new connection
				if(!pSocket->OnAccept(serviceSocket))
				{
					FreeRTOS_shutdown( serviceSocket, FLAG_ABORT_CONNECTION );
				}
			} while(pSocket->m_acceptThreadState == RUNNING);
		
			pSocket->m_acceptThreadState = CLOSING;
			if(pSocket->m_socketState > DISCONNECTING)
			{
				pSocket->Disconnect();
				pSocket->OnClose(EVN_SERVERDOWN);
			}	
			// Self terminate the thread
			pSocket->m_acceptThreadState = CLOSED;

		#if  WIN32
			return true;
		#endif
		}
		
		
		// Read any incoming data as well as detect the connected lost/reset
#ifdef __MQX__
		void ReadTask(dword pParam)
#elif	WIN32
		 DWORD ReadTask(LPVOID pParam)
#else
		void ReadTask( void *pParam ) 
#endif

		{
			//Pointer to object
			CaSocket 	*pSocket = (CaSocket*) pParam;
			//Byte reads from the recv function
			int16_t 	bytesRead; 
			//Total message length 
			dword		message_len = 0;
			//Flag to detect any error reading the buffer
			bool		Error_buffer_sizeof = false;
			//Pointer to buffer
			byte 		*buf = pSocket->mBuffer;
			while(1)
			{
				//Change socket status
				pSocket->m_readThreadState = RUNNING;

				do 
				{
					//Blocking receive
					switch(pSocket->m_type_decoder)
					{
					case CaSocket::R2CP_DECODER:
						bytesRead = FreeRTOS_recv( pSocket->m_socket_private , (char *)buf , HEADER_SIZEOF_R2CP , 0 );
						if(bytesRead <= 0)
						{
							printf("\r\n SOCKET HEADER ERROR [ %d ]" , bytesRead );
							break;
						}
						
						message_len = MESSAGE_LENGTH( buf );
						if( message_len > ( pSocket->m_sizeof_buffer - HEADER_SIZEOF_R2CP ) )
						{
							Error_buffer_sizeof = true;
							
							printf("\r\n SOCKET MESSAGE LEN ERROR [ %d ]" , bytesRead );
						}
						
						while( message_len > 0 )
						{
							if( Error_buffer_sizeof )
								bytesRead = FreeRTOS_recv( pSocket->m_socket_private , (char *)buf , pSocket->m_sizeof_buffer , 0 );
							else
								bytesRead = FreeRTOS_recv( pSocket->m_socket_private , (char *)&buf[ HEADER_SIZEOF_R2CP ] , message_len , 0 );
							
							if (bytesRead <= 0)
							{
								Api::Log::Print( Api::Log_level::Error, " SOCKET MESSAGE LEN ERROR [ %d ]" , bytesRead );
								break;
							}
							
							message_len -= bytesRead;
						}
						break;
					default://NONE_DECODER
						bytesRead = FreeRTOS_recv(pSocket->m_socket_private, (char *)buf, pSocket->m_sizeof_buffer, 0);
						break;
					}
					
					//If there was an error exit thread and notify owner with OnClose
					if( bytesRead <= 0 )
					{
						//Tenemos que salir
						break;
					}
			
					if (!Error_buffer_sizeof)
					{
						pSocket->OnReceive( buf , bytesRead );
					}
					else
					{
						Error_buffer_sizeof = false;
						pSocket->OnError();
					}
		
				} while(pSocket->m_readThreadState == RUNNING);
						
				pSocket->Disconnect();
				
				pSocket->m_readThreadState = CLOSED;
					
				vTaskSuspend( pSocket->m_read_handle );	
			}
		}
		
		CaSocket::CaSocket(etype_decode deco_type):m_sock (NULL), m_socket_private(NULL),m_timeout_rcv(0),m_read_handle(0),m_readThreadState(CLOSED)
		{
			m_socketState 	= NONE;

			m_type_decoder  = deco_type;
			
			switch(m_type_decoder)
			{
			case R2CP_DECODER:
				m_sizeof_buffer = SIZEOF_BUFFER_R2CP;
				break;;
			default://NONE_DECODER
				m_sizeof_buffer = SIZEOF_BUFFER_NODECODER;
				break;
			}
			
			mBuffer = new byte[m_sizeof_buffer];
#if 0	
			/* Initialize the mutex */ 
			OS::Mutex::Create(m_send_socket_mutex);   
#endif
		}
		
		CaSocket::~CaSocket() 
		{
#if 0
			OS::Mutex::Destroy(m_send_socket_mutex);
#endif	
			
			if(m_readThreadState != CLOSED)
			{
#ifdef SOCKET_WATCHDOG_TASK
				Comp::Watchdog::Remove(m_Wdog_index);
#endif
				OS::Thread::Destroy(m_read_handle);
			}
		
			delete [] mBuffer;
		}
		
		int16_t CaSocket::Create(void){
			 
			m_sock = FreeRTOS_socket( AF_INET, SOCK_STREAM, 0 );
			if (m_sock == SOCKET_ERROR)
			{
				return CASOCKET_ERROR;
			} 
			m_socketState = CREATED;
			return true;
		}
		
		int16_t CaSocket::Accept( word localPort, byte maxConn,int16_t timeout_rcv )
		{
			
			int16_t 			retval;
			OS::sockaddr		addr;
			
			if(m_socketState < CREATED)
			{
				return CASOCKET_ERROR ;
			}
			
			m_timeout_rcv = timeout_rcv;
			
			addr.sin_family 		= AF_INET;
			addr.sin_port 			= localPort;
			addr.sin_addr 			= INADDR_ANY;
			
			retval = FreeRTOS_bind(m_sock, (OS::sockaddr *)&addr, sizeof(addr));
			if (retval != RTCS_OK)
			{
				return CASOCKET_ERROR ;
			}
			
			retval = FreeRTOS_listen(m_sock, maxConn); 
			if (retval != RTCS_OK)
			{
				return CASOCKET_ERROR;
			}
			if (((m_sock == SOCKET_ERROR)) || (retval != RTCS_OK))
			{      
				return CASOCKET_ERROR;
			}
			
			m_socketState = ACCEPTING;
		
			OS::thread_handle_t accept_handle;	/*Accept task*/
#if 0
			(void) OS::Thread::Create(  AcceptTask,
										(char *)"accept_TCP",
										reinterpret_cast<void *>(this),
										1000,
										OS::HIGH_TASK_PRIORITY,
										&accept_handle);
			
			OS::Thread::Resume( accept_handle );
#endif
			return true;
			
		}
		
		int16_t CaSocket::Connect(word remotePort, dword remoteaddr , dword rx_Size , dword tx_Size, byte nattemps)
		{	
			int16_t rvalue = false;
		    uint32_t option;
			
			//Check port values
			if(remotePort > 65535)
			{
				return rvalue;
			}
			
			OS::sockaddr		addr = { 0 };
			int32_t 			retval;
			byte 				num_retry = 0;
			
			memset( &addr, 0 ,sizeof(OS::sockaddr));
			addr.sin_family 		= AF_INET;
			addr.sin_port   		= FreeRTOS_htons( remotePort ); 
			addr.sin_addr 			= remoteaddr;
						
			m_sock = FreeRTOS_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP ); 
			if (m_sock == SOCKET_ERROR )
			{
				printf("\n\rError: Unable to create socket for IPv4 connections.");
				return false;
			}
#if 0
			retval = FreeRTOS_setsockopt(m_sock, /*SOL_TCP*/0, FREERTOS_SO_SNDBUF ,(char *)&tx_Size,sizeof(tx_Size));
			if( retval != RTCS_OK){

				Api::Log::Print( Api::Log_level::Debug,"\nERROR OPT_TBSIZE");
				continue;
			}				
			retval = FreeRTOS_setsockopt(m_sock, /*SOL_TCP*/0, FREERTOS_SO_RCVBUF ,(char *)&rx_Size,sizeof(rx_Size));
			if( retval != RTCS_OK)
			{

				Api::Log::Print( Api::Log_level::Debug,"\nERROR OPT_TBSIZE");
				continue;
			}
#endif

			option = TIME_WAIT_CONNECT / portTICK_PERIOD_MS;
			retval = FreeRTOS_setsockopt(m_sock, 0 , FREERTOS_SO_RCVTIMEO , (void *)&option , 0 );
			if( retval != RTCS_OK)
			{
				printf("\n\rERROR FREERTOS_SO_RCVTIMEO ");
			}

			while( num_retry++ < nattemps )
			{
				if( FreeRTOS_connect(m_sock, (OS::sockaddr *)&addr, sizeof(addr)) != 0 )
				{
					FreeRTOS_shutdown( m_sock , FREERTOS_SHUT_RDWR );
					printf("\n\r ___________ERROR CONNECT___________ ");
					continue;
				}
				else//Connection created successfully
				{
					printf("\n\r ___________Connected___________ ");
					m_socket_private = m_sock;
					m_socketState = CONNECTED;
					return true;
					break;
				}
			}
			
			FreeRTOS_closesocket( m_sock );
			return false;
		}
		
		int16_t CaSocket::Send(byte *buf , word len)
		{
			
			int dataPtr = 0;
			int sentBytes = 0;

			//We cannot send data if the socket's not connected
			if(m_socketState < CONNECTED)
			{
				return CASOCKET_ERROR;
			}
		
			//Have we a valid buffer?
			if( buf == nullptr || len <= 0)
			{
				return CASOCKET_ERROR;
			}
		
			while(len > 0)
			{
				if( /*m_socketState == CONNECTED*/1)
					sentBytes = FreeRTOS_send(m_sock, (char *)&buf[dataPtr], len, 0);
				else
					sentBytes = FreeRTOS_send(m_socket_private, (char *)&buf[dataPtr], len, 0);
		
				if( sentBytes <= 0 )
				{
					return CASOCKET_ERROR;
				}
				dataPtr += sentBytes;
				len -= sentBytes;
			}
	
			return dataPtr;			
		}
		
		bool CaSocket::AcceptServiceSocket(Socket_t serviceSocket)
		{
			if(serviceSocket == NULL)
				serviceSocket = m_sock;
			
			OS::sockaddr peerName;
			uint16_t nameSize = sizeof(OS::sockaddr);
#if 0
			time_t					tv;		
			if(m_timeout_rcv == 0)
				tv = TIME_WAIT_RECEIVE_AND_LOST;
			else
				tv = m_timeout_rcv;

			if(setsockopt(serviceSocket, SOL_TCP, OPT_RECEIVE_TIMEOUT, (char *)&tv, sizeof(tv)))
			{

				Api::Log::Print( Api::Log_level::Debug,"\nFatal Error: Unable to set sock_receivedet options.");
				return false;
			}
#endif
			m_socket_private = serviceSocket;
		
			m_socketState = CONNECTED_ACCP;
			
			if( m_readThreadState == CLOSED )
			{
				if( m_read_handle )
				{
					vTaskResume( m_read_handle );
				}
				else
				{
					if( m_sizeof_buffer == SIZEOF_BUFFER_R2CP )
					{
						(void) OS::Thread::Create( 	ReadTask,
													(char *)"read_TCP",
													reinterpret_cast<void *>(this),
													1000,
													OS::VERY_HIGH_TASK_PRIORITY,
													&m_read_handle);
					}
					else
					{
						//Si no ponemos como REAL TIME TASK la bajada de ficheros, 
						//no se guardan en RAM ( 0xx14000000 )
						//De momento lo dejamos asi, hasta ver que implica esto
						(void) OS::Thread::Create( 	ReadTask,
													(char *)"read_TCP",
													reinterpret_cast<void *>(this),
													1000,
													OS::REAL_TIME_TASK_PRIORITY,
													&m_read_handle);
					}
				}
			}
			
			OS::Thread::Resume( m_read_handle );
						
			return true;
		
		}
		
		void CaSocket::Shutdown( void )
		{
			//Con esto liberamos la tarea de readtask
			FreeRTOS_shutdown( m_sock , FREERTOS_SHUT_RDWR );
		}
		
		int16_t CaSocket::Disconnect(void)
		{
			int ret;
			int maxAttemps = 0;
			
			/* signalling that we want to gracefully break the connection up */
			FreeRTOS_shutdown( m_sock , FREERTOS_SHUT_RDWR );
			
			/* waits at least one ticks*/
			 vTaskDelay(pdMS_TO_TICKS(5));
			 
			/* get rid of any accumulated data */
			if( (ret = FreeRTOS_recv( m_sock , mBuffer , m_sizeof_buffer , 0)) >= 0 )
			{
				printf("\n\r[ Disconnect ]READING BYTES [ %d ] ", ret );
			    if ( ret == 0 )
			    {
			        vTaskDelay(pdMS_TO_TICKS(5));
			    }			    
			}
			
			Api::Log::Print( Api::Log_level::Info," [ Disconnect ] ERRNO [ %d ] ", ret );
			if ( ret == -pdFREERTOS_ERRNO_EINVAL )
			{
				printf("\n\r[ Disconnect ] FREERTOS_ERRNO_EINVAL BYTES " );
			    return false;
			}
			
			if( ret == -pdFREERTOS_ERRNO_ENOTCONN || ret == -pdFREERTOS_ERRNO_ENOMEM )
			{
				printf("\n\r[ Disconnect ] CLOSE SOCKET " );
			    FreeRTOS_closesocket(m_sock);
			}
			else 
			{
				printf("\n\rFreeRTOS_closesocket CRASS [ %d ]", ret );
			    FreeRTOS_closesocket(m_sock);
			}
#if 0
			//Se bloquea el disconnect y el shutdown
			int16_t rval = 1;
			if( FreeRTOS_shutdown( m_sock , FLAG_ABORT_CONNECTION ) == 0)
			{
		        /* Wait for the socket to disconnect gracefully (indicated by FreeRTOS_recv() returning a FREERTOS_EINVAL error) before closing the socket. */
				Api::Log::Print( Api::Log_level::Info," WAITING SOCKET DISCONNECTED BY READ TASK ");
				return true;
			}
			else
			{
				Api::Log::Print( Api::Log_level::Info,"\nError FreeRTOS_shutdown\n");
				rval = 0;
			}
			
			if( FreeRTOS_closesocket( m_sock ) != RTCS_OK )
			{
				Api::Log::Print( Api::Log_level::Info,"\nError FreeRTOS_closesocket\n");
				rval = 0;
			}
				
#endif
			m_socketState = DISCONNECTING;
			OnClose( EVN_CONNCLOSED );
			return false;
		}
	}//RTCS
}//API
