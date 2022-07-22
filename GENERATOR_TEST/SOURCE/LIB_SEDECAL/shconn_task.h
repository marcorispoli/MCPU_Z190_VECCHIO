#ifndef SHSOCKET_H_
#define SHSOCKET_H_

#include "../Platform/Interface_OS/os_event.h"
#include "../Api/api_socket.h"


#define MASTER_ETHR2CP_CLOSE_SOCKET_EVENT					0x00000001
#define MASTER_ETHR2CP_RESTART_CONNECTION_EVENT				0x00000002

/*!
 * \brief Main paramater to create a tcp/ip connection
 * \param	base_port: port where the connection has to be established on the remote host
 * \param	local_id:  	id local
 * \param   local_shid:	id SH
 * \param	sh_ip:		SH IP
 * \param	sh_hearbeattime: maximum time to receive a message before closing the socket
 */
typedef struct{
	word 	base_port;
	byte	local_id;
	byte 	local_shid;
	dword	sh_ip;	
	word	sh_hearbeattime;
	word	rx_size;
	word  	tx_size;
}tsmarthubinfo;

/*!
 * \brief Sockect status
 */
typedef enum{
	SH_DisConnected = 0,
	SH_Waiting2SendRqstConn,
	SH_Connected,
	SH_ConnectionLost
}tSHConnectionStatus;

/*!
 * \brief Thread to manage the smarthub connection
 */
#ifdef __MQX__
extern void smarthub_connection_task( uint32_t temp );
#elif WIN32
extern DWORD smarthub_connection_task( LPVOID temp );
#else
extern void smarthub_connection_task( void * temp );	
#endif

/*!
 * \brief Function to send data by the open sockect
 */
extern int16_t SendEthMsg(byte *msg, word n_msg);

/*!
 * \brief Class
 */
class CaSh_Connection : public API::RTCS::CaSocket{
	
public:
	/*!
	 * \brief Constructor
	 * \param[in]	Connection parameter 
	 * \param[in]	Funtion where the message received are going to be passed
	 */
	CaSh_Connection( void );
	/*!
	 * \brief Destructor
	 */
	~CaSh_Connection();
	
	static CaSh_Connection* GetInstance( void );
	/*!
	 * \brief	This method is employed to restart the connection from outside
	 */
	void Restart_Connection(void);

	/*!
	 * \brief This is used to send the connection request message to the generator and start the trade off
	 */
	void SendConnectionRequest(void);
	/*!
	 * \brief	Smarthub task
	 */
	#ifdef __MQX__
	friend void smarthub_connection_task(uint32_t temp);
	#elif WIN32
	friend DWORD smarthub_connection_task( LPVOID temp );
	#else
	friend void smarthub_connection_task( void* temp );
	#endif
	 /*!
	  *	Function to send message 
	  */
	 friend int16_t SendEthMsg(byte *msg, word n_msg);
	
	/*!
	 * \brief Methos to return the connection current status 
	 */
	static tSHConnectionStatus Get_SHConnectionStatus(void){ return m_p_instance ? m_p_instance->m_SHConnectionStatus : SH_DisConnected;}
   /*!
	*\brief Abort Connection
	*/
	static void Abort_Connection( void );
	/*!
	 * \brief Connection parameters
	 */
	static tsmarthubinfo  	m_sh_info;
	
	/*!
	 * \brief Enables hdl updating
	 */
	void EnableHdlUpdating( bool Enable );
	
private:
	/*!
	 * \brief Instance
	 */
	static CaSh_Connection *m_p_instance;
	/*!
	 * \brief This variable indicate the sh connection status
	 */
	tSHConnectionStatus m_SHConnectionStatus;
	/*!
	 * \brief Waiting Time for Event
	 */
	dword 			m_waiting_time_for_event;
	/*!
	 * \brief Flag to know if the hdl is being loaded
	 */
	bool			m_IsHdlUpdatingEnabled;
		
	/*!
	 * \brief Try to open a connection with the remote host
	 * \return	success:true any error:false
	 */
	bool TryConnect( tsmarthubinfo *pShInfo ); 
	/*!
	 * \brief 		This callback function is used to receive messages in case of it will be enabled
	 * \param[in]	buf: message pointer
	 * \param[in]	len: message length
	 * return		success:true any error: false
	 */
	virtual void OnReceive(byte* buf, dword len);
	/*!
	 * \brief		THis callback function is used to notify any remote connection error in case our system is working as Server
	 * \param[in]	serviceSocket: socket with the host remote 
	 * \return		success:true any error: false
	 */
	virtual bool OnAccept(dword serviceSocket);
	/*!
	 * \brief		THis callback function is used to notify any error on the socket connection
	 * \param[in]	closeEvent: error event
	 * \return		success:true any error: false
	 */
	virtual void OnClose(byte closeEvent);
	/*!
	 * \brief   	This is used to inform if any error happends on the socket connection
	 * \param[in]	errorEvent: error event
	 * \return		success:true any error: false
	 */
	virtual void OnError(byte errorEvent);
	/*!
	 * \brief	This event is used to synchronise the socket status with the  Smarthub_Task
	 */
	 OS::event_t 	event_socket;
};
#endif /* SHSOCKET_H_ */

