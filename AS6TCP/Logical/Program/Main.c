#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
#include <AsTCP.h>

UDINT bswap_32(UDINT val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF); 
	return (val << 16) | (val >> 16);
}

void _INIT ProgramInit(void) {
	tcpOpen.pIfAddr = 0; // Use default interface
	tcpOpen.port = 1234; // Set the desired port number here
	tcpOpen.options = tcpOPT_REUSEADDR;
}

static USINT recvBuffer[1];
static UDINT sendBuffer;
static BOOL ServerInitialized = 0;

void _CYCLIC ProgramCyclic(void)
{
	counterOPC++;
	// Initialize server if not done
	if (!ServerInitialized) {
		tcpOpen.enable = 1;
		TcpOpen(&tcpOpen);
	}

	if (tcpOpen.status == 0) {
		tcpServer.enable = 1;
		tcpServer.ident = tcpOpen.ident;
		TcpServer(&tcpServer);
		ServerInitialized = 1;
	}
	
	// Handle incoming connections and data
	if (tcpOpen.status == 0) { //tcpSTATUS_OK  //tcpERR_OK
		tcpRecv.enable = 1;
		tcpRecv.ident =  tcpServer.identclnt;
		tcpRecv.pData = (UDINT)&recvBuffer;
		tcpRecv.datamax = sizeof(recvBuffer);
		TcpRecv(&tcpRecv);
	
		if (tcpRecv.recvlen > 0 && recvBuffer[0] == 'R') {
			if (tcpRecv.status == 0) counter++;
			sendBuffer = counter; //UDINT
			sendBuffer = bswap_32(sendBuffer);
			tcpSend.enable = 1;
			tcpSend.ident = tcpServer.identclnt;
			tcpSend.pData = (UDINT)&sendBuffer;
			tcpSend.datalen = sizeof(sendBuffer);
			TcpSend(&tcpSend);
		}
	} // tcpOpen.status == 0
}
