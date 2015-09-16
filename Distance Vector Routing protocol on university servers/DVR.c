#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

int number_of_neighbors(char topology[255])
{
	char no_of_nbors;
	no_of_nbors='\0';
	no_of_nbors=topology[2];	
	int x=atoi(&no_of_nbors);
	return x;
	
}	


int number_of_servers(char topology[255])
{
	char no_of_servs;
	no_of_servs='\0';
	no_of_servs=topology[0];	
	int x=atoi(&no_of_servs);
	return x;
	
}	




main(int argc, char *argv[])
{

FILE *f;
FILE *f1;
int i,count_of_sentences;
int j;
count_of_sentences=0;
f1 = fopen("topology_example.txt","r");


f = fopen("topology_example.txt","r");
char topology[255];

memset(topology,'\0', 255);
//bzero(topology, 255);
int count_endline=0;

int a11,b11,c;
c=5;

for(i=0; i<255 ; i++)
topology[i]=fgetc(f);


printf("%s\n\n", topology);

printf("%d \n", count_of_sentences);

printf(" Number of servers : %d \n", number_of_servers(topology));

printf(" Number of neighbors : %d \n", number_of_neighbors(topology));



// Converting Topology file string into the sentences
/*------------------sn array of structs contains all the senences parsed --------------*/
int y=0;
y=2+number_of_neighbors(topology)+number_of_servers(topology);


char token1[25];
//memset(token1, '\0', 25);
char *token2;

struct sentence
{
		char *sen;	
};	
struct sentence sn[10];

y=2+number_of_neighbors(topology)+number_of_servers(topology);

printf("Using fgets \n\n");

	for(i=0;i<y;i++)
	{

		sn[i].sen=(char *) malloc(25);
		bzero(sn[i].sen,25);
		fgets(sn[i].sen, 25, f1);

	}


printf("Using fgets: \n\n");
for(i=0;i<y;i++)
printf("%s",(sn[i].sen));


/*------------------sn array of structs contains all the WORDS parsed --------------*/


struct words
{
		char *first;
		char *second;
		char *third;	
};

struct words w[y];

	for(i=0;i<y;i++)
	{

		w[i].first=(char *) malloc(50);
		bzero(w[i].first,50);
		w[i].second=(char *) malloc(50);
		bzero(w[i].second,50);
		w[i].third=(char *) malloc(50);
		bzero(w[i].third,50);	
	}


int xyz;
xyz=0;
const char s[] = " ";
   char *token;
	for(i=2;i<y;i++)
	{
	/* get the first token */
	token = strtok(sn[i].sen, s);
	w[i].first = token ;
	printf("%s  \n", token);
		/* walk through other tokens */
		while( token != NULL ) 
		{
      		token = strtok(NULL, s);
			
			if(token==NULL)
				break;
			else if(xyz==0)
			{
				w[i].second=token;
				xyz=1;	
				continue;
			}
			else 
			{
				w[i].third=token;
				xyz=0;	
				continue;
			}
		}
		
	printf("\n\n");
	}
	
	
printf("Words: \n\n");
for(i=2;i<y;i++)
{
//printf("%d i : \n", i);
printf("%s \n",(w[i].first));	
printf("%s \n",(w[i].second));
printf("%s",(w[i].third));  
printf("\n\n");
}   

struct cost_to_neighbor
{
	uint16_t neighbor_id;
	uint16_t cost;
};
int num_neighbors = number_of_neighbors(topology);
int num_servers = number_of_servers(topology);

struct cost_to_neighbor ctn[num_neighbors];


for(i= num_servers + 2 ; i < (num_neighbors + num_servers + 2); i++)
{
	ctn[i-2-num_servers].neighbor_id = atoi(w[i].second);
	ctn[i-2-num_servers].cost=atoi(w[i].third);
	printf("Neighbor ID : %d    Cost : %d \n", ctn[i-2-num_servers].neighbor_id, ctn[i-2-num_servers].cost);
}
	 printf("\n\n\n");

struct cost_to_neighbor temp_ctn;
for(i=0; i<num_neighbors-1; i++)
for(j=0; j<num_neighbors-1-i; j++) 
{
	if(ctn[j].neighbor_id > ctn[j+1].neighbor_id)
	{
		temp_ctn=ctn[j];
		ctn[j]=ctn[j+1];
		ctn[j+1]=temp_ctn;	
	}		
}

uint16_t is_node_a_neighbor(uint16_t n)
{
	int i;
	uint16_t x;
	for(i=0; i<num_neighbors; i++)
	{
		if(ctn[i].neighbor_id == n)
			break;
		
	}	
	
	if(i!=num_neighbors)
		return ctn[i].cost;	
	else
		return 65535;
}	


struct server
{
	uint16_t server_id;
	char *ip;
	uint16_t port;
};


struct server ser[num_servers];
struct server buf1;
buf1.ip= (char *) malloc (25);

for(i= 2 ; i < (num_servers + 2); i++)
{
	ser[i-2].server_id = atoi(w[i].first);
	ser[i-2].ip = (char *) malloc (25);
	bzero(ser[i-2].ip, 25);
	ser[i-2].ip = w[i].second;
	ser[i-2].port=atoi(w[i].third);
	printf("Server ID : %d    IP : %s      Port : %d\n", ser[i-2].server_id, ser[i-2].ip, ser[i-2].port);
}
	 printf("\n\n\n");

struct server temp_server;
for(i=0; i<num_servers-1; i++)
for(j=0; j<num_servers-1-i; j++) 
{
	if(ser[j].server_id>ser[j+1].server_id)
	{
		temp_server=ser[j];
		ser[j]=ser[j+1];
		ser[j+1]=temp_server;	
	}		
}	

printf("Costs to neighbor from the host node : \n");
for(i=0;i<num_neighbors;i++)
printf("Neighbor ID : %d    Cost : %d \n", ctn[i].neighbor_id, ctn[i].cost);

printf("\n\n");

printf("Information about servers : \n");
for(i=0;i<num_servers;i++)
printf("Server ID : %d    IP : %s      Port : %d\n", ser[i].server_id, ser[i].ip, ser[i].port);



fclose(f1); 
fclose(f);

/*------------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------
 * ----------------T H I S    I S    E N D    O F    F I L E     P A R S I N G -------
 * -----------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------
 */

struct distance 
{
	uint16_t server_ID; 	// Server ID of the neighbor 
	uint16_t cost_of_path[num_servers];   // Cost from the neighbor to each of the servers
};

struct distance dv[num_neighbors+1];




/* -----Initializing Distance Vector table---- */

// Getting server ID of the host node
// dv[num_neighbor] contains the distance vector of the host

int temp3;
temp3=atoi(w[num_servers+2].first);

int count3;
count3=0; 
 
for( i=0; i<num_neighbors; i++)
{
	dv[i].server_ID = ctn[i].neighbor_id;
	for(j=0; j< num_servers; j++)
	{	
		dv[i].cost_of_path[j]=65535;		
	}
}	

dv[num_neighbors].server_ID = temp3; 

for(i=0;i<num_servers;i++)
{
	uint16_t x=is_node_a_neighbor(ser[i].server_id);
	dv[num_neighbors].cost_of_path[i]=x;
	
	if(ser[i].server_id == dv[num_neighbors].server_ID)
		dv[num_neighbors].cost_of_path[i]=0;

}	


printf("\n\nInitial Distance Vector Routing Table : \n");
for(i=0;i<=num_neighbors;i++)
{
	for(j=0; j<num_servers; j++)
		printf("%d %d = %d     ", dv[i].server_ID, ser[j].server_id, dv[i].cost_of_path[j]);		
	printf("\n");
}	

/*--------------------packing----------------------------*/
char packet[68];
bzero(packet,68);

uint16_t sid;
i=0;
while(i<num_servers)
	if( ser[i].server_id == dv[num_neighbors].server_ID)
		break;

if(i== num_servers)
	printf("----ERROR--->>> Topology file corrupt \n");
else
	sid = i;
	
uint16_t noupf=num_servers;
uint16_t sp1= ser[i].port;
uint16_t zero_padding=00;

//Converts IP Address in 32 bit format 
struct sockaddr_in sadr;
sadr.sin_addr.s_addr=htonl(inet_addr(ser[0].ip));
printf("Raw s_addr : %u \n", sadr.sin_addr.s_addr);

char *ptr = (char *) malloc(4);
ptr=inet_ntoa(sadr.sin_addr);

printf("Pakcet IP : %s size: %d \n", ptr, sizeof(ptr));

memcpy(packet,&noupf,2);
memcpy(packet + 2 ,&sp1, 2);
memcpy(packet + 4 ,&sadr.sin_addr.s_addr, 4);

j=8;
// Main Packing FOR loop
for(i=0;i<num_servers; i++)
{	
	
	sadr.sin_addr.s_addr=htonl(inet_addr(ser[i].ip));
	memcpy(packet + j, &sadr.sin_addr.s_addr, 4);	
	j=j+4;
	memcpy(packet + j, &ser[i].port, 2);
	j=j+2;
	memcpy(packet + j, &zero_padding, 2);
	j=j+2;
	memcpy(packet + j, &ser[i].server_id, 2);
	j=j+2;
	memcpy(packet + j, &dv[num_neighbors].cost_of_path[i] ,2);
	j=j+2;
}

//printf(" uint values %u, %u", noupf, sp1);

for(i=0; i<68; i++)
{
if(packet[i]==NULL)
	printf("NULL Character \n");
else	
	printf("Byte : %d Value :%c \n", (i+1), packet[i]);
}	

/*--------------------------unpacking------------------------*/
uint16_t t1,t2;
//memcpy(&t1, packet, sizeof(t1));
//memcpy(&t2, packet+sizeof(t1), sizeof(t2));
memcpy(&t1, packet, 2);
memcpy(&t2, packet+2, 2);
printf("Test Value 1 : %u \n", t1);
printf("Test Value 2 : %u \n", t2);

struct sockaddr_in sadr1;
//memcpy(&sadr1.sin_addr.s_addr ,packet+sizeof(t2), sizeof(sadr1.sin_addr.s_addr));
memcpy(&sadr1.sin_addr.s_addr ,packet+4, 4);

sadr1.sin_addr.s_addr=ntohl(sadr1.sin_addr.s_addr);
printf("size of ip in network : %d \n", sizeof(sadr1.sin_addr.s_addr) );


char *ptr1 = (char *) malloc(8);
ptr1=inet_ntoa(sadr1.sin_addr);
printf("Test Value 3 : %s size: %d \n\n", ptr1, sizeof(ptr1));


j=8;
// Main UnPacking FOR loop
for(i=0;i<num_servers; i++)
{	
	
	
	memcpy(&sadr1.sin_addr.s_addr ,packet + j, 4);
	sadr1.sin_addr.s_addr=ntohl(sadr1.sin_addr.s_addr);
	
	char *ptr1 = (char *) malloc(8);
	ptr1=inet_ntoa(sadr1.sin_addr);
	printf("Server IP : %s \n", ptr1);
	j=j+4;
	
	memcpy(&t2, packet + j, 2);
	j=j+2;
	printf("Port Number : %u \n", t2);
	
	memcpy(&t2, packet + j, 2);
	j=j+2;
	printf("Padding : %u \n", t2);
	
	memcpy(&t2, packet + j, 2);
	j=j+2;
	printf("Server ID : %u \n", t2);

	memcpy(&t2, packet + j, 2);
	j=j+2;
	printf("Cost : %u \n\n", t2);


}



/*------------------------------------T E S T I N G   O F ------------------------------
 * --------------------------------------P A C K E T S----------------------------------
 */
/*
char buf[256];

char *temp;
char *one;
char *two;

temp=argv[1];
one="1";
two="2";

if(strcmp(temp,one)==0)
{
int listenfd;
struct sockaddr_in servaddr;

listenfd = socket(AF_INET, SOCK_DGRAM, 0);


	bzero((char *) &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(3500);

bind(listenfd, (const struct sockaddr*) &servaddr, sizeof(servaddr));

printf("Server Listening.....\n");

//recvfrom(listenfd, &buf1, sizeof(buf1) , 0, (struct sockaddr *)&servaddr, sizeof(servaddr));	
recvfrom(listenfd, &buf, sizeof(buf) , 0, (struct sockaddr *)&servaddr, sizeof(servaddr));	

printf("%s \n", buf);
//printf("IP : %s  Port : %d  Server ID : %d  \n", buf1.ip,buf1.port,buf1.server_id);

printf("Message received correctly..");
}

else if(strcmp(temp,two)==0)
{
int sockfd;
struct sockaddr_in targetaddr;

sockfd = socket(AF_INET, SOCK_DGRAM, 0);


	bzero((char *) &targetaddr, sizeof(targetaddr));
	targetaddr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1", &targetaddr.sin_addr);
	targetaddr.sin_port = htons(3500);

bind(sockfd, (const struct sockaddr*) &targetaddr, sizeof(targetaddr));

printf("Client Sending.....\n");
	
sendto(sockfd, topology, 255 , 0, (struct sockaddr *)&targetaddr, sizeof(targetaddr));	
//sendto(sockfd, &ser[0], sizeof(ser[0]) , 0, (struct sockaddr *)&targetaddr, sizeof(targetaddr));	

	
}	




*/

}
