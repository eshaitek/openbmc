#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
const char * sensor_path[]={	"/sys/class/hwmon/hwmon0/temp1_input"
						};

typedef uint8_t	bool8_t;

/** @enum MessageType
 *
 *  The different message types supported by the PLDM specification.
 */
typedef enum {
	PLDM_RESPONSE,		   //!< PLDM response
	PLDM_REQUEST,		   //!< PLDM request
	PLDM_RESERVED,		   //!< Reserved
	PLDM_ASYNC_REQUEST_NOTIFY, //!< Unacknowledged PLDM request messages
} MessageType;

enum pldm_supported_types {
	PLDM_BASE = 0x00,
	PLDM_PLATFORM = 0x02,
	PLDM_BIOS = 0x03,
	PLDM_FRU = 0x04,
	PLDM_FWU = 0x05,
	PLDM_OEM = 0x3F,
};


struct pldm_msg_hdr {

#if 1
	uint8_t instance_id : 5; //!< Instance ID
	uint8_t reserved : 1;	 //!< Reserved
	uint8_t datagram : 1;	 //!< Datagram bit
	uint8_t request : 1;	 //!< Request bit

	uint8_t type : 6;	 //!< PLDM type
	uint8_t header_ver : 2;	 //!< Header version
#else
	uint8_t request : 1;	 //!< Request bit
	uint8_t datagram : 1;	 //!< Datagram bit
	uint8_t reserved : 1;	 //!< Reserved
	uint8_t instance_id : 5; //!< Instance ID

	uint8_t header_ver : 2;	 //!< Header version
	uint8_t type : 6;	 //!< PLDM type

#endif
	uint8_t command; //!< PLDM command code
}__attribute__((packed));;

const size_t hdrSize = sizeof(struct pldm_msg_hdr);

struct pldm_msg {
	struct pldm_msg_hdr hdr; //!< PLDM message header
	uint8_t payload[1]; //!< &payload[0] is the beginning of the payload
}__attribute__((packed));

/** @struct pldm_header_info
 *
 *  The information needed to prepare PLDM header and this is passed to the
 *  pack_pldm_header and unpack_pldm_header API.
 */
struct pldm_header_info {
	MessageType msg_type;	 //!< PLDM message type
	uint8_t instance;	 //!< PLDM instance id
	uint8_t pldm_type;	 //!< PLDM type
	uint8_t command;	 //!< PLDM command code
	uint8_t completion_code; //!< PLDM completion code, applies for response
};


enum pldm_platform_commands {
	PLDM_GET_TERMINUS_UID = 0x03,
	//Terminus commands
	PLDM_SETTID = 0x1,
	PLDM_GETTID = 0x2,
	//Numeric Sensor commands
	PLDM_SET_NUMERIC_SENSOR_ENABLE = 0x10,
	PLDM_GET_SENSOR_READING = 0x11,
	PLDM_GET_SENSOR_THRESHOLD = 0x12,
	PLDM_SET_SENSOR_THRESHOLD = 0x13,
	PLDM_GET_SENSOR_HYSTERESIS = 0x15,
	PLDM_SET_SENSOR_HYSTERESIS = 0x16,
	PLDM_SET_STATE_SENSOR_ENABLE = 0x20,
	//State Sensor commands
	PLDM_GET_STATE_SENSOR_READINGS = 0x21,
	PLDM_SET_NUMERIC_EFFECTER_ENABLE = 0x30,
	//PLDM Effecter commands
	PLDM_SET_NUMERIC_EFFECTER_VALUE = 0x31,
	PLDM_GET_NUMERIC_EFFECTER_VALUE = 0x32,
	PLDM_SET_STATE_EFFECTER_ENABLE = 0x38,
	PLDM_SET_STATE_EFFECTER_STATES = 0x39,
	PLDM_GET_STATE_EFFECTER_STATES = 0x3A,
	PLDM_GET_PDR_REPOSITORY_INFO = 0x50,
	PLDM_GET_PDR = 0x51,
	PLDM_PLATFORM_EVENT_MESSAGE = 0x0A
};

/** @brief PLDM base codes
 */
enum pldm_completion_codes {
	PLDM_SUCCESS = 0x00,
	PLDM_ERROR = 0x01,
	PLDM_ERROR_INVALID_DATA = 0x02,
	PLDM_ERROR_INVALID_LENGTH = 0x03,
	PLDM_ERROR_NOT_READY = 0x04,
	PLDM_ERROR_UNSUPPORTED_PLDM_CMD = 0x05,
	PLDM_ERROR_INVALID_PLDM_TYPE = 0x20,
	PLDM_INVALID_TRANSFER_OPERATION_FLAG = 0x21
};

enum pldm_effecter_data_size {
	PLDM_EFFECTER_DATA_SIZE_UINT8,
	PLDM_EFFECTER_DATA_SIZE_SINT8,
	PLDM_EFFECTER_DATA_SIZE_UINT16,
	PLDM_EFFECTER_DATA_SIZE_SINT16,
	PLDM_EFFECTER_DATA_SIZE_UINT32,
	PLDM_EFFECTER_DATA_SIZE_SINT32
};

enum pldm_sensor_present_state {
	PLDM_SENSOR_UNKNOWN = 0x0,
	PLDM_SENSOR_NORMAL = 0x01,
	PLDM_SENSOR_WARNING = 0x02,
	PLDM_SENSOR_CRITICAL = 0x03,
	PLDM_SENSOR_FATAL = 0x04,
	PLDM_SENSOR_LOWERWARNING = 0x05,
	PLDM_SENSOR_LOWERCRITICAL = 0x06,
	PLDM_SENSOR_LOWERFATAL = 0x07,
	PLDM_SENSOR_UPPERWARNING = 0x08,
	PLDM_SENSOR_UPPERCRITICAL = 0x09,
	PLDM_SENSOR_UPPERFATAL = 0x0a
};

/** @brief PLDM sensor supported states
 */
enum pldm_sensor_operational_state {
	PLDM_SENSOR_ENABLED,
	PLDM_SENSOR_DISABLED,
	PLDM_SENSOR_UNAVAILABLE,
	PLDM_SENSOR_STATUSUNKOWN,
	PLDM_SENSOR_FAILED,
	PLDM_SENSOR_INITIALIZING,
	PLDM_SENSOR_SHUTTINGDOWN,
	PLDM_SENSOR_INTEST
};

enum pldm_sensor_event_message_enable {
	PLDM_NO_EVENT_GENERATION,
	PLDM_EVENTS_DISABLED,
	PLDM_EVENTS_ENABLED,
	PLDM_OP_EVENTS_ONLY_ENABLED,
	PLDM_STATE_EVENTS_ONLY_ENABLED
};

//---------------------------------------------------------------------------------
/* Maximum size for request */
#define PLDM_SET_STATE_EFFECTER_STATES_REQ_BYTES 19
#define PLDM_GET_STATE_SENSOR_READINGS_REQ_BYTES 4
#define PLDM_GET_NUMERIC_EFFECTER_VALUE_REQ_BYTES 2
#define PLDM_GET_SENSOR_READING_REQ_BYTES 3

/* Minimum response length */
#define PLDM_GET_PDR_MIN_RESP_BYTES 12
#define PLDM_GET_NUMERIC_EFFECTER_VALUE_MIN_RESP_BYTES 5
#define PLDM_GET_SENSOR_READING_MIN_RESP_BYTES 8
#define PLDM_GET_STATE_SENSOR_READINGS_MIN_RESP_BYTES 2
#define PLDM_GET_NUMERIC_SENSOR_THRESHOLD_MIN_RSP_BYTES 8
#define PLDM_GET_NUMERIC_SENSOR_HYSTERESIS_MIN_RSP_BYTES 3


#define PLDM_VERSION_0 0
#define PLDM_CURRENT_VERSION PLDM_VERSION_0

#define PLDM_RQ_D_MASK 0xC0
#define PLDM_RQ_D_SHIFT 0x06
#define PLDM_MSG_TYPE_MASK 0x3F
#define PLDM_TID_MAX 0xFF
#define PLDM_INSTANCE_ID_MASK 0x1F
#define PLDM_INSTANCE_MAX 32
#define PLDM_MAX_TYPES 64
#define PLDM_MAX_CMDS_PER_TYPE 256

/** @struct pldm_get_sensor_reading_req
 *
 *  Structure representing PLDM get sensor reading request
 */
struct pldm_get_sensor_reading_req {
	uint16_t sensor_id;
	bool8_t rearm_event_state;
}__attribute__((packed));;


/** @struct pldm_get_sensor_reading_resp
 *
 *  Structure representing PLDM get sensor reading response
 */
struct pldm_get_sensor_reading_resp {
	uint8_t completion_code;
	uint8_t sensor_data_size;
	uint8_t sensor_operational_state;
	uint8_t sensor_event_message_enable;
	uint8_t present_state;
	uint8_t previous_state;
	uint8_t event_state;
	uint8_t present_reading[1];
} __attribute__((packed));

int pack_pldm_header(const struct pldm_header_info *hdr,
		     struct pldm_msg_hdr *msg)
{
	if (msg == NULL || hdr == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (hdr->msg_type != PLDM_RESPONSE && hdr->msg_type != PLDM_REQUEST &&
	    hdr->msg_type != PLDM_ASYNC_REQUEST_NOTIFY) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (hdr->instance > PLDM_INSTANCE_MAX) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (hdr->pldm_type > (PLDM_MAX_TYPES - 1)) {
		return PLDM_ERROR_INVALID_PLDM_TYPE;
	}

	uint8_t datagram = (hdr->msg_type == PLDM_ASYNC_REQUEST_NOTIFY) ? 1 : 0;

	if (hdr->msg_type == PLDM_RESPONSE) {
		msg->request = PLDM_RESPONSE;
	} else if (hdr->msg_type == PLDM_REQUEST ||
		   hdr->msg_type == PLDM_ASYNC_REQUEST_NOTIFY) {
		msg->request = PLDM_REQUEST;
	}
	msg->datagram = datagram;
	msg->reserved = 0;
	msg->instance_id = hdr->instance;
	msg->header_ver = PLDM_CURRENT_VERSION;
	msg->type = hdr->pldm_type;
	msg->command = hdr->command;

	return PLDM_SUCCESS;
}

int read_sensor_data(int id, char *ptr, size_t size){
	FILE *pFile;
	int nr_set = ARRAY_SIZE(sensor_path);
	printf("sensor_path nr:%d\n",nr_set);
	if(id>nr_set) {
		printf("No such ID node\n");
		return -1;
	}
	pFile = fopen(sensor_path[id-1],"r");
	if (pFile == NULL) {
		printf("fopen fail:%d[%s](%s) \n",errno,strerror(errno),sensor_path[id-1]);
		return -1;
	}else if(size>0){
		fread(ptr, size, 1, pFile);
	}else{
		printf("clear %s\n",sensor_path[id-1]);
	}
	fclose(pFile);
	return 0;
}

//
int encode_get_sensor_reading_resp(
    uint8_t instance_id, uint8_t completion_code, uint8_t sensor_data_size,
    uint8_t sensor_operational_state, uint8_t sensor_event_message_enable,
    uint8_t present_state, uint8_t previous_state, uint8_t event_state,
    uint8_t *present_reading, struct pldm_msg *msg, size_t payload_length)
{
	struct pldm_header_info header;
	int rc = PLDM_SUCCESS;

	if (msg == NULL || present_reading == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (sensor_data_size > PLDM_EFFECTER_DATA_SIZE_SINT32) {
		return PLDM_ERROR_INVALID_DATA;
	}

	header.msg_type = PLDM_RESPONSE;
	header.instance = instance_id;
	header.pldm_type = PLDM_PLATFORM;
	header.command = PLDM_GET_SENSOR_READING;

	if ((rc = pack_pldm_header(&header, &(msg->hdr))) > PLDM_SUCCESS) {
		return rc;
	}

	struct pldm_get_sensor_reading_resp *response =
	    (struct pldm_get_sensor_reading_resp *)msg->payload;

	response->completion_code = completion_code;
	response->sensor_data_size = sensor_data_size;
	response->sensor_operational_state = sensor_operational_state;
	response->sensor_event_message_enable = sensor_event_message_enable;
	response->present_state = present_state;
	response->previous_state = previous_state;
	response->event_state = event_state;

	if (sensor_data_size == PLDM_EFFECTER_DATA_SIZE_UINT8 ||
	    sensor_data_size == PLDM_EFFECTER_DATA_SIZE_SINT8) {
		if (payload_length != PLDM_GET_SENSOR_READING_MIN_RESP_BYTES) {
			return PLDM_ERROR_INVALID_LENGTH;
		}
		response->present_reading[0] = *present_reading;

	} else if (sensor_data_size == PLDM_EFFECTER_DATA_SIZE_UINT16 ||
		   sensor_data_size == PLDM_EFFECTER_DATA_SIZE_SINT16) {
		if (payload_length !=
		    PLDM_GET_SENSOR_READING_MIN_RESP_BYTES + 1) {
			return PLDM_ERROR_INVALID_LENGTH;
		}
		uint16_t val = *(uint16_t *)present_reading;
		val = val;//htole16(val);
		memcpy(response->present_reading, &val, 2);

	} else if (sensor_data_size == PLDM_EFFECTER_DATA_SIZE_UINT32 ||
		   sensor_data_size == PLDM_EFFECTER_DATA_SIZE_SINT32) {
		if (payload_length !=
		    PLDM_GET_SENSOR_READING_MIN_RESP_BYTES + 3) {
			return PLDM_ERROR_INVALID_LENGTH;
		}
		uint32_t val = *(uint32_t *)present_reading;
		val = val;//htole32(val);
		memcpy(response->present_reading, &val, 4);
	}

	return PLDM_SUCCESS;
}

int decode_get_sensor_reading_req(const struct pldm_msg *msg,
				  size_t payload_length, uint16_t *sensor_id,
				  uint8_t *rearm_event_state)
{
	if (msg == NULL || sensor_id == NULL || rearm_event_state == NULL) {
		printf("PLDM_ERROR_INVALID_DATA\n");
		return PLDM_ERROR_INVALID_DATA;
	}

	if (payload_length != PLDM_GET_SENSOR_READING_REQ_BYTES) {
		printf("payload_length:%d,PLDM_ERROR_INVALID_LENGTH\n",payload_length);
		return PLDM_ERROR_INVALID_LENGTH;
	}
	printf("msg->payload:%d\n",payload_length);
	for(unsigned int i=0;i<payload_length;i++)
		printf("%02X ",msg->payload[i]);
	printf("\n");
	struct pldm_get_sensor_reading_req *request =
	    (struct pldm_get_sensor_reading_req *)msg->payload;

	*sensor_id = request->sensor_id;//htole16(request->sensor_id);
	*rearm_event_state = request->rearm_event_state;

	return PLDM_SUCCESS;
}

//
int get_numeric_sensor_reading(int forClientSockfd, struct pldm_msg *msg,
				  size_t payload_length, uint8_t mctp_header[], uint8_t instance_id)
{
	uint16_t retsensorId;
	bool8_t retrearmEventState;
	printf("get_numeric_sensor_reading::payload_length:%d\n",payload_length);
	int rc =
        decode_get_sensor_reading_req(msg, payload_length,
                                      &retsensorId, &retrearmEventState);
	if(rc!=PLDM_SUCCESS)
	{
		printf("decode_get_sensor_reading_req fail\n");
		return -1;
	}

	printf("sensorId:%04x\n",retsensorId);
	printf("rearmEventState:%02x\n",retrearmEventState);

	if(retsensorId==0x1)
	{
		char readdata[1024]={0};
		read_sensor_data(retsensorId,readdata,sizeof(readdata));
		printf("readdata-string:%s\n",readdata);
		printf("readdata-int:%d\n",atoi(readdata));

		uint8_t tmp_uint8_t = atoi(readdata);
		printf("tmp_uint8_t:%d\n",tmp_uint8_t);

		uint16_t tmp_uint16_t = atoi(readdata);
		printf("tmp_uint16_t:%d\n",tmp_uint16_t);

		uint32_t tmp_uint32_t = atoi(readdata);
		printf("tmp_uint32_t:%d\n",tmp_uint32_t);

		uint8_t responseSize = hdrSize + PLDM_GET_SENSOR_READING_MIN_RESP_BYTES +1;
		uint8_t responseMsg[1024] ={0};

		struct pldm_msg *response = (struct pldm_msg *)&responseMsg[5];

		uint8_t completionCode = 0;
		uint8_t sensor_dataSize = PLDM_EFFECTER_DATA_SIZE_UINT16;
		uint8_t sensor_operationalState = PLDM_SENSOR_ENABLED;
		uint8_t sensor_event_messageEnable = PLDM_NO_EVENT_GENERATION;
		uint8_t presentState = PLDM_SENSOR_NORMAL;
		uint8_t previousState = PLDM_SENSOR_NORMAL;
		uint8_t eventState = PLDM_SENSOR_UPPERWARNING;
		uint16_t presentReading = atoi(readdata);//uint8_t max:255,0xFF;uint16_t max:65535,0xFFFF

		printf("presentReading:%04X\n",presentReading);
		rc = encode_get_sensor_reading_resp(
			instance_id, completionCode, sensor_dataSize, sensor_operationalState,
			sensor_event_messageEnable, presentState, previousState, eventState,
			(uint8_t*)(&presentReading), response,
			responseSize - hdrSize);

		responseMsg[0] = mctp_header[0];
		responseMsg[1] = mctp_header[1];
		responseMsg[2] = mctp_header[2];
		responseMsg[3] = mctp_header[3];//(mctp_header[3]&0xF7) ;
		responseMsg[4] = 0x01;
		printf("Resp MCTP Header--Start\n");
		for(unsigned int i=0;i<4;i++)
			printf("responseMsg[%d]:%02X\n",i,responseMsg[i]);
		printf("Resp MCTP Header--End\n");
		responseSize+=5;
		printf("responseSize:%d\n",responseSize);
		send(forClientSockfd,&responseSize,sizeof(responseSize),0);
		send(forClientSockfd,responseMsg,responseSize,0);
	}
	else{
		printf("Non-Support sensorId:%d",retsensorId);
		uint8_t responseSize = 0;
		send(forClientSockfd,&responseSize,sizeof(responseSize),0);
	}

	return rc;
}

int get_state_effecter_reading(int forClientSockfd, struct pldm_msg *msg,
				  size_t payload_length, uint8_t mctp_header[], uint8_t instance_id)
{
	printf("get_state_effecter_reading is not implemented yet\n");
	uint8_t responseSize = 0;
	send(forClientSockfd,&responseSize,sizeof(responseSize),0);
	return 1;
}


int main(int argc , char *argv[])
{
	//size_t hdrSize = sizeof(struct pldm_msg_hdr);
	printf("version 3 hdrSize:%d\n",hdrSize);
    //socket的建立
	
    uint8_t pldmmsg[1024] = {0};
	struct pldm_msg_hdr *pldm_hdr = (struct pldm_msg_hdr *)pldmmsg;
	uint8_t mctp_header[4] ={0};
	uint8_t mctp_msg_type[1] ={0};
    //uint8_t message[] = {0x01, 0x08, 0x00, 0xC1, 0x01, 0x19, 0x02, 0x3A, 0x00, 0x01, 0x01, 0x02, 0x02};
//---------------------------------------------------------------------------
    int sockfd_in = 0,forClientSockfd_in = 0;
    sockfd_in = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd_in == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8700);
    bind(sockfd_in,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd_in,5);

//-------------------------------------------------------------------------------------
    int sockfd_out = 0,forClientSockfd_out = 0;
    sockfd_out = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd_out == -1){
        printf("Fail to create a sockfd_out.");
    }

    //socket的連線
    struct sockaddr_in serverInfo_out,clientInfo_out;
    int addrlen_out = sizeof(clientInfo_out);
    bzero(&serverInfo_out,sizeof(serverInfo_out));

    serverInfo_out.sin_family = PF_INET;
    serverInfo_out.sin_addr.s_addr = INADDR_ANY;
    serverInfo_out.sin_port = htons(8701);
    bind(sockfd_out,(struct sockaddr *)&serverInfo_out,sizeof(serverInfo_out));
    listen(sockfd_out,5);

    forClientSockfd_in = accept(sockfd_in,(struct sockaddr*) &clientInfo, (socklen_t*)&addrlen);
    printf("Port 8700 is connected\n");

    forClientSockfd_out = accept(sockfd_out,(struct sockaddr*) &clientInfo_out, (socklen_t*)&addrlen_out);
    printf("Port 8701 is connected\n");

    while(1){
		uint8_t size;
		int recv_cnt=0;
		printf("Wait for msg from mctpd\n");
        recv(forClientSockfd_in,&size,sizeof(uint8_t),0);
		printf("Server tell us msg size:%d\n",size);

		//MCTP HEADER
		int cnt = recv(forClientSockfd_in,mctp_header,sizeof(mctp_header),0);
		if(cnt !=sizeof(mctp_header) )
			printf("error when read mctp heaer\n");
		else{
			printf("MCTP HEADER Start\n");
			for(unsigned int i=0;i<sizeof(mctp_header);i++)
				printf("[%d]:%02X ",i,mctp_header[i]);
			printf("\nMCTP HEADER End\n");
		}
		size -= cnt;

		//MCTP MSG TYPE
		cnt = recv(forClientSockfd_in,mctp_msg_type,sizeof(mctp_msg_type),0);
		if(cnt !=sizeof(mctp_msg_type) )
			printf("error when read mctp heaer\n");
		else{
			printf("MCTP MSG TYPE:%02X ",mctp_msg_type[0]);
		}
		size -= cnt;

		while(recv_cnt<size)
		{
			int cnt = recv(forClientSockfd_in,pldmmsg+recv_cnt,sizeof(pldmmsg)-recv_cnt,0);
			recv_cnt += cnt;
			printf("cnt:%d total:%d\n",cnt,recv_cnt);
		}
		
		for(int i=0;i<recv_cnt;i++)
			printf("%02X ",pldmmsg[i]);
		printf("\n");
		printf("PLDM MSG----\n");
		printf("PLDM HEADER \n");
		printf("pldm_hdr.request:%x\n",pldm_hdr->request);
		printf("pldm_hdr.datagram:%x\n",pldm_hdr->datagram);
		printf("pldm_hdr.instance_id:%x\n",pldm_hdr->instance_id);
		printf("pldm_hdr.header_ver:%x\n",pldm_hdr->header_ver);
		printf("pldm_hdr.type:%x\n",pldm_hdr->type);
		printf("pldm_hdr.command:%x\n",pldm_hdr->command);
		//Numeric Sensor
		#define GetSensorReading 0x11

		//State Sensor
		//0x21 GetStateSensorReadings

		//State Effecter
		#define GetStateEffecterStates 0x3A
		if(pldm_hdr->command ==GetStateEffecterStates ){
			printf("PLDM cmd is GetStateEffecterStates\n");
			get_state_effecter_reading(forClientSockfd_out, (struct pldm_msg *)pldmmsg, recv_cnt-hdrSize, mctp_header, pldm_hdr->instance_id);
		}else if(pldm_hdr->command == GetSensorReading){
			printf("PLDM cmd is GetSensorReading\n");
			get_numeric_sensor_reading(forClientSockfd_out, (struct pldm_msg *)pldmmsg, recv_cnt-hdrSize, mctp_header, pldm_hdr->instance_id);
		}else{
			printf("Non-Support PLDM cmd\n");
			uint8_t responseSize = 0;
			send(forClientSockfd_out,&responseSize,sizeof(responseSize),0);
		}

	}
	return 0;
}