#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 0

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg {
  char data[20];
  };

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
   int seqnum;
   int acknum;
   int checksum;
   char payload[20];
    };



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* Statistics 
 * Do NOT change the name/declaration of these variables
 * You need to set the value of these variables appropriately within your code.
 * */
int A_application = 0;
int A_transport = 0;
int B_application = 0;
int B_transport = 0;

/* Globals 
 * Do NOT change the name/declaration of these variables
 * They are set to zero here. You will need to set them (except WINSIZE) to some proper values.
 * */
float TIMEOUT = 0.0;
int WINSIZE;         //This is supplied as cmd-line parameter; You will need to read this value but do NOT modify it; 
int SND_BUFSIZE = 0; //Sender's Buffer size
int RCV_BUFSIZE = 0; //Receiver's Buffer size

int number_of_unacked_packets; // Current no. of unacked packets
int base; // base of window
int nextseqnum; // Next Sequence Number - Used at sender
int expectedseqnum; //Next Expected Sequence Number - Used at receiver 
int i;  
int count;  // Used to adding packets to buffer. buffer is used in possible cases of retransmission of selected packets.
int rcv_count;
int rcv_base;
struct pkt packet_most_recently_acked;

struct buffer
{
	struct pkt packet;
	int acknowledged; // 0 if Not, 1 if Yes
	int sent; // 0 if Not, 1 if Yes
};	

struct buffer buf[1000];

struct buffer rcv_buf[1000];



 


/*
 This struct is basically will be used for implementing multiple timers as in Selective Repeat, we need one timer
 per packet being sent in the window but we have only one hardware timer available. So, it is not possible to actually
 calculate the starttime and stoptimer for every packet. Instead what sceheme implemented here is that an array of 
 structure timer_multiple is created of size that of window size. Basically, each entry corresponds to a packet in the
 window. Then when we call starttimer, we make the starttimer field for that packet 1. The main task here is to 
 predict when A_timerinterrupt is called, for which packet in the window is it being called. So, as per this logic
 if starttimer is 1 for a packet and that packet has the lowest sequence number amongst all the packets of which timer
 is started already, it is the packet for which stoptimer is called. When A_timerinterrupt is called, we first decide
 the packet whose timer just timed out and then reset the starttimer and stoptimer fields to 1 and 0 respectively as
 we will have to retransmit that packet when A_timerinterrupt occurs. 
 When a packet is received successfully, and ACK for the same is received, we reflect the changes in our timer_multiple
 array making the starttimer=0 for that packet again. 
  
 The problem with this logic is that if we just keep switching the values of starttimer between 0 and 1, if packet 
 with seq num 0, times out twice, the next stoptimer will be detected for it only, instead of packet with seq num 1.
 Hence, we will increment the startimer by 1 every time A_timerinterrupt is called.  And when stoptimer is called
 manually (Case of succesful delivery), starttimer and stoptimer is set to 0. 1 value for stoptimer basically indicates
 that timeouts are happening and not successful delivery. So, now when deciding the packet for the on stoptimer, 
 product of index of the array (seq num) and the value of starttimer is taken into consideration.


*/

int tm[1000];
int timer_count=0;



void starttimer(int AorB,float increment);
void stoptimer(int AorB);
void tolayer3(int AorB,struct pkt packet);
void tolayer5(int AorB,char datasent[20]);

void tm_stop(int sqn)
{
	int pos=timer_count;
	int z2;
	for(i=0 ; i<timer_count; i++)
	{
		if(tm[i]== sqn)
		{pos=i;
		z2=tm[pos];
		break;
		}
	}
	printf("TM_STOP->>>>> Old Count Timer : %d \n", timer_count);
	for(i=pos; i<timer_count-1; i++)
	{
	tm[i]=tm[i+1];
	}
	timer_count--;
	
	printf("TM_STOP->>>>> Stopping timer for packet %d %d pos : %d \n", sqn, z2, pos);
	printf("TM_STOP->>>>> New Count Timer : %d \n", timer_count);
	printf("TM_STOP->>>>>  ");
	for(i =0; i<timer_count; i++)
	printf("%d ",tm[i]);
	printf("\n\n");
}	

void tm_start(int sqn)
{
	int z3;
	tm[timer_count]=sqn;
	z3=tm[timer_count];
	printf("TM_START->>>>> Old Count Timer : %d \n", timer_count);
	timer_count++;
	printf("TM_START->>>>> Starting timer for packet %d %d\n", sqn, z3);
	printf("TM_START->>>>> New Count Timer : %d \n", timer_count);
	printf("TM_START->>>>>  ");
	for(i =0; i<timer_count; i++)
	printf("%d ",tm[i]);
	printf("\n\n");


}	

/* called from layer 5, passed the data to be sent to other side */
void A_output(message)
  struct msg message;
{
	
	
	if( nextseqnum < base + WINSIZE )
		{
			A_application++;

			
		 	struct pkt packet;
		 	packet.checksum = 0;
		 	
		 	packet.seqnum = count;
		 	packet.acknum = 0;
		 	for ( i=0; i<20; i++)
		 	{
				packet.payload[i] = message.data[i];
				packet.checksum = packet.checksum + message.data[i];
		 	}
		 	
		 	packet.checksum = packet.seqnum + packet.acknum + packet.checksum;
		 	
		 	buf[count].packet= packet;
		 	buf[count].acknowledged = 0;
		 	buf[count].sent = 0;
		 	printf("--------------------------------------------------------\n");
		 	printf("A_OUTPUT->>>>%d %d \n", count, buf[count].packet.seqnum );
						printf("A_OUTPUT->>>>> A_application : %d\n", A_application);
		 	
		 			 	
				printf("A_OUTPUT->>>>While sending packet for first time as it is in window %d\n", buf[count].packet.seqnum);
				tolayer3(0, buf[count].packet);
				A_transport++;
				printf("A_OUTPUT->>>>> A_transport : %d\n\n", A_transport);				
			
				starttimer(0, TIMEOUT);
				tm_start(buf[count].packet.seqnum);
				
				nextseqnum = ( (nextseqnum + 1)  );	
				buf[count].sent=1;		
				buf[count].acknowledged=0;
			
				
				count++;
			}
			
		 	
		 	/*
		 	tolayer3(0, packet);
			A_transport++;
			
			*/
			


			

		else
		{
		
			A_application++;
		 	printf("A_OUTPUT->>>>> A_application : %d\n\n", A_application);
			
		 	
		 	struct pkt packet;
		 	packet.checksum = 0;
		 	
		 	packet.seqnum = count;
		 	packet.acknum = 0;
		 	for ( i=0; i<20; i++)
		 	{
				packet.payload[i] = message.data[i];
				packet.checksum = packet.checksum + message.data[i];
		 	}
		 	
		 	packet.checksum = packet.seqnum + packet.acknum + packet.checksum;
		 	
		 	buf[count].packet= packet;
		 	buf[count].acknowledged = 0;
		 	buf[count].sent = 0;
		 	
		 	count++;
		 	
		 	printf("A_OUTPUT->>>>> In else (Incoming msg out of window --) %d %d \n\n", count, base);
		}	
		

}

void B_output(message)  /* need be completed only for extra credit */
  struct msg message;
{

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	int cs=packet.acknum + packet.seqnum;
	for( i=0; i<20; i++)
	cs= cs + packet.payload[i];	
	printf("A_INPUT->>>>>Checksum at sender = %d  Checksum at receiver = %d \n", cs, packet.checksum);
	printf("A_INPUT->>>>> Packet.ACK Number = %d  Base : %d Nextseqnum = %d \n\n", packet.acknum,base, (nextseqnum) ); 
	if(cs == packet.checksum && (buf[packet.acknum].packet.seqnum == packet.acknum) )
		{
		printf("A_INPUT->>>>> In If \n\n");
		//If packet falls in window mark it acknowledged
		if(packet.acknum>=base && packet.acknum <(base + WINSIZE))
		buf[packet.acknum].acknowledged = 1;	
		
		//If packet's sequence number is send_base then move base to unacked packet with smallest sequence number
		int t4=0;
		
		for(t4 = base ; t4<nextseqnum; t4++)
			if(buf[t4].acknowledged==0)
				break;
		
		if(packet.acknum==base)
		base = t4 ;
		/*
		if ( base ==  nextseqnum)
			{
			stoptimer(0);	
			tm_stop(packet.acknum);
				
			}
		else 
			{
			printf("A_INPUT->>>> In else of first if \n\n");
			stoptimer(0);
				tm_stop(packet.acknum);
			starttimer(0, TIMEOUT);
				tm_start(packet.acknum);
			}
			*/
			printf("A_INPUT->>>>> In If - Sender Base : %d\n\n", base);

			if(timer_count!=0)
			{
				stoptimer(0);	
				tm_stop(packet.acknum);
			}
			
				
			for ( i=base; i< base+WINSIZE ; i++)
		 	{
			
			if(base > 1000 - WINSIZE-1)
			continue;			 			 	

			if(nextseqnum <base + WINSIZE)
			{
				//printf("A_INPUT->>>>In for while sending \n\n");
				if(buf[i].acknowledged==0 && buf[i].packet.seqnum!=0 )
				{
				printf("A_INPUT->>>>In for - In if while sending packet %d\n", buf[i].packet.seqnum);
				
				tolayer3(0, buf[i].packet);
				A_transport++;
				printf("A_INPUT->>>>> A_transport : %d\n\n", A_transport);				
				
				/*
				int y,k;
				k=i;
				for(y=0; y<timer_count; y++)
				{
				
					if(tm[y]==k)
						{		
						printf("A->>>>> k = %d \n", k);			
						tm_stop(k);
						continue;
						}
				}
				printf("A->>>>> k = %d \n", k);	
				starttimer(0, TIMEOUT);
				tm_start(k);
				*/
				if(buf[i].sent==0)
				nextseqnum = ( (nextseqnum + 1) % (1000) );	
				//if((nextseqnum % 50) == 0)
				//TIMEOUT = TIMEOUT* ((float)B_application /  (float)A_application);
				buf[i].sent=1;		
				buf[i].acknowledged=0;
				//break;
				}
			}
								
			
			}	

				
				
		}	
	else
	{
	printf("A_INPUT->>>>> Waiting for A_TIMERINTERRUPT \n\n");
	}	

}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	int t1;
	/*
	int temp=tm[0]; // Popping out the least recently added element in the queue 
					// i.e. Stopping the timer for the packet which started it. 
					// As it is obvious that timer of packet which first started timer will timeout first
					// Now, we also have to shift the queue. And add this element to the rear of the queue
					// because it will now be retransmitted. So, we have to start it's timer again.
	for( i=0; i<timer_count-1 ; i++)
	{
		tm[i]=tm[i+1]; // Since we have already copied first element, we dont need swapping.	
	}
	
	tm[timer_count-1]=temp; // Added at pos. timer_count - 1, as no new packet is starting the timer
							// Just one of the packets is re-starting. As, After new starttimer, we increment
							// timer_count, the position at which a new packet's seqnum will be added when it
							// starts its timer.
		
		
	*/
	
	printf("A_TIMERINTERRUPT->>>>> \n");
	if(buf[tm[0]].acknowledged==0)
	
	{int c=tm[0];
	tm_stop(c);
	tm_start(c);
	starttimer(0,TIMEOUT);
			
		/*	
		for(i=base; i< base+WINSIZE ; i++)
		{
		if(buf[i].acknowledged==0)	
			{
			t1=i;
			
			break;
			}
		}
		*/
		
		printf("Retransmitting packet with sequence number : %d\n", c);	
	
		tolayer3( 0, buf[c].packet);
		buf[c].sent = 1;
		A_transport++;
		printf("A_INPUT->>>>> A_transport : %d\n\n", A_transport);
	}
	else if(buf[tm[0]].acknowledged==1)
		{
		int c=tm[0];
		tm_stop(c);
		starttimer(0,TIMEOUT);
			
		}	
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{

	nextseqnum = 0;
	SND_BUFSIZE = 1000;
	number_of_unacked_packets = 0;
	base = 0;
	TIMEOUT = 20.0;
	count = 0;
	for(i =0 ; i<1000; i++)
	buf[i].sent=0;

	for(i=0; i<1000; i++)
	tm[i]=0;

}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
	int checksum;
	int move_base_by = 0;
	checksum = packet.seqnum + packet.acknum;
	for(i=0;i<20;i++)
	checksum = checksum + packet.payload[i];
	printf("B_INPUT->>>>>Checksum at sender = %d  Checksum at receiver = %d\n", checksum, packet.checksum);
	printf("B_INPUT->>>>> Packet.SEQ Number = %d  \n\n", packet.seqnum ); 
	if ((checksum  == packet.checksum)  && (packet.seqnum >= rcv_base) && (packet.seqnum < (rcv_base +WINSIZE - 1)))
		{
			printf("B_INPUT->>>>In If - Packet correctly received and falls within receiver window \n");
	
			struct pkt p;
	
			p.seqnum = 0;
			p.acknum = packet.seqnum;
			p.checksum = p.seqnum + p.acknum; 
			for(i = 0; i<20; i++)
			{
				p.payload[i] = packet.payload[i];
				p.checksum = packet.payload[i] + p.checksum;
			}
	
			//packet_most_recently_acked=p;

			printf("B_INPUT->>>>>SENDING ACK TO HOST A's A_INPUT \n\n");
			tolayer3(1, p);
			
			
			// If the packet was not previously received, adding it to the buffer.	
			int z=0;
			for(i=0; i<rcv_count; i++)
				if(rcv_buf[i].packet.seqnum==packet.seqnum)
					z=1;
			
			if(z==0)
			{
			printf("B_INPUT->>>>> Packet not in buffer. Adding now\n");
			rcv_buf[packet.seqnum].packet=packet;
			rcv_buf[packet.seqnum].acknowledged = 1;
			rcv_count++; 
			}	
			
			// If the packet has sequence number = base of window, Then deliver the packets in buffer
			// starting from base till they are in consecutive order. Window is moved forwared appropriately.
				
			if(packet.seqnum == rcv_base)
			{	
				printf("B_INPUT->>>>> Packet.seqnum = RCV_BASE. \n");

				i=rcv_base;
				while(rcv_buf[i].acknowledged == 1)
				{
					printf("B_INPUT->>>>> Sending to apllication layer \n");
					B_application++;
					B_transport++;
					printf("B_INPUT->>>>> B_application : %d\n", B_application);
					printf("B_INPUT->>>>> B_transport : %d\n\n", B_transport);
					tolayer5(1, packet.payload); 
					i++;
					move_base_by++;
				}
				rcv_base = (rcv_base + move_base_by) ;	
			
								
			//expectedseqnum = (expectedseqnum + 1) % 1000;
			
			}
			
		}	
	
	
	else if ((checksum  == packet.checksum) /*  && (packet.seqnum >= rcv_base/* - WINSIZE + 1 ) */ && (packet.seqnum < (rcv_base )))
		{
			printf("B_INPUT->>>>In If - Packet correctly received and falls behnind receiver window.\n It is already ACKed but must be ACKed again \n");	
			
				struct pkt p;
	
			p.seqnum = 0;
			p.acknum = packet.seqnum;
			p.checksum = p.seqnum + p.acknum; 
			for(i = 0; i<20; i++)
			{
				p.payload[i] = packet.payload[i];
				p.checksum = packet.payload[i] + p.checksum;
			}
			B_transport++;
			printf("B_INPUT->>>>> B_transport : %d\n\n", B_transport);


			printf("B_INPUT->>>>>SENDING ACK TO HOST A's A_INPUT \n\n");
			tolayer3(1, p);

	
		}

	else
		{
			printf("B_INPUT->>>> In else - Corrupt packet. Ignoring.. Waiting for timerinterrupt. \n");
			B_transport++;
			printf("B_INPUT->>>>> B_transport : %d\n\n", B_transport);
			//printf("B_INPUT->>>>>SENDING ACK OF MOST RECENT ACKed PACKET TO HOST A's A_INPUT \n\n");			
			//tolayer3(1, packet_most_recently_acked);
		}

}

/* called when B's timer goes off */
void B_timerinterrupt()
{
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	expectedseqnum=0;
	
	rcv_count=0;
	RCV_BUFSIZE = 1000;
	number_of_unacked_packets = 0;
	rcv_base = 0;
	//TIMEOUT = 47.0;
		for(i =0 ; i<1000; i++)
	rcv_buf[i].acknowledged=0;
		
}

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
  - emulates the tranmission and delivery (possibly with bit-level corruption
    and packet loss) of packets across the layer 3/4 interface
  - handles the starting/stopping of a timer, and generates timer
    interrupts (resulting in calling students timer handler).
  - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct pkt *pktptr;     /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
 };
struct event *evlist = NULL;   /* the event list */

//forward declarations
void init();
void generate_next_arrival();
void insertevent(struct event*);

/* possible events: */
#define  TIMER_INTERRUPT 0  
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1



int TRACE = 1;             /* for my debugging */
int nsim = 0;              /* number of messages from 5 to 4 so far */ 
int nsimmax = 0;           /* number of msgs to generate, then stop */
float time = 0.000;
float lossprob = 0.0;	   /* probability that a packet is dropped */
float corruptprob = 0.0;   /* probability that one bit is packet is flipped */
float lambda = 0.0; 	   /* arrival rate of messages from layer 5 */
int ntolayer3 = 0; 	   /* number sent into layer 3 */
int nlost = 0; 	  	   /* number lost in media */
int ncorrupt = 0; 	   /* number corrupted by media*/

/**
 * Checks if the array pointed to by input holds a valid number.
 *
 * @param  input char* to the array holding the value.
 * @return TRUE or FALSE
 */
int isNumber(char *input)
{
    while (*input){
        if (!isdigit(*input))
            return 0;
        else
            input += 1;
    }

    return 1;
}

int main(int argc, char **argv)
{
   struct event *eventptr;
   struct msg  msg2give;
   struct pkt  pkt2give;
   
   int i,j;
   char c;

   int opt;
   int seed;

   //Check for number of arguments
   if(argc != 5){
   	fprintf(stderr, "Missing arguments\n");
	printf("Usage: %s -s SEED -w WINDOWSIZE\n", argv[0]);
   	return -1;
   }

   /* 
    * Parse the arguments 
    * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html 
    */
   while((opt = getopt(argc, argv,"s:w:")) != -1){
       switch (opt){
           case 's':   if(!isNumber(optarg)){
                           fprintf(stderr, "Invalid value for -s\n");
                           return -1;
                       }
                       seed = atoi(optarg);
                       break;

           case 'w':   if(!isNumber(optarg)){
                           fprintf(stderr, "Invalid value for -w\n");
                           return -1;
                       }
                       WINSIZE = atoi(optarg);
                       break;

           case '?':   break;

           default:    printf("Usage: %s -s SEED -w WINDOWSIZE\n", argv[0]);
                       return -1;
       }
   }
  
   init(seed);
   A_init();
   B_init();
   
   while (1) {
        eventptr = evlist;            /* get next event to simulate */
        if (eventptr==NULL)
           goto terminate;
        evlist = evlist->next;        /* remove this event from event list */
        if (evlist!=NULL)
           evlist->prev=NULL;
        if (TRACE>=2) {
           printf("\nEVENT time: %f,",eventptr->evtime);
           printf("  type: %d",eventptr->evtype);
           if (eventptr->evtype==0)
	       printf(", timerinterrupt  ");
             else if (eventptr->evtype==1)
               printf(", fromlayer5 ");
             else
	     printf(", fromlayer3 ");
           printf(" entity: %d\n",eventptr->eventity);
           }
        time = eventptr->evtime;        /* update time to next event time */
        if (nsim==nsimmax)
	  break;                        /* all done with simulation */
        if (eventptr->evtype == FROM_LAYER5 ) {
            generate_next_arrival();   /* set up future arrival */
            /* fill in msg to give with string of same letter */    
            j = nsim % 26; 
            for (i=0; i<20; i++)  
               msg2give.data[i] = 97 + j;
            if (TRACE>2) {
               printf("          MAINLOOP: data given to student: ");
                 for (i=0; i<20; i++) 
                  printf("%c", msg2give.data[i]);
               printf("\n");
	     }
            nsim++;
            if (eventptr->eventity == A) 
               A_output(msg2give);  
             else
               B_output(msg2give);  
            }
          else if (eventptr->evtype ==  FROM_LAYER3) {
            pkt2give.seqnum = eventptr->pktptr->seqnum;
            pkt2give.acknum = eventptr->pktptr->acknum;
            pkt2give.checksum = eventptr->pktptr->checksum;
            for (i=0; i<20; i++)  
                pkt2give.payload[i] = eventptr->pktptr->payload[i];
	    if (eventptr->eventity ==A)      /* deliver packet by calling */
   	       A_input(pkt2give);            /* appropriate entity */
            else
   	       B_input(pkt2give);
	    free(eventptr->pktptr);          /* free the memory for packet */
            }
          else if (eventptr->evtype ==  TIMER_INTERRUPT) {
            if (eventptr->eventity == A) 
	       A_timerinterrupt();
             else
	       B_timerinterrupt();
             }
          else  {
	     printf("INTERNAL PANIC: unknown event type \n");
             }
        free(eventptr);
        }

terminate:
	//Do NOT change any of the following printfs
	printf(" Simulator terminated at time %f\n after sending %d msgs from layer5\n",time,nsim);
	
	printf("\n");
	printf("Protocol: SR\n");
	printf("[PA2]%d packets sent from the Application Layer of Sender A[/PA2]\n", A_application);
	printf("[PA2]%d packets sent from the Transport Layer of Sender A[/PA2]\n", A_transport);
	printf("[PA2]%d packets received at the Transport layer of Receiver B[/PA2]\n", B_transport);
	printf("[PA2]%d packets received at the Application layer of Receiver B[/PA2]\n", B_application);
	printf("[PA2]Total time: %f time units[/PA2]\n", time);
	printf("[PA2]Throughput: %f packets/time units[/PA2]\n", B_application/time);
	return 0;
}



void init(int seed)                         /* initialize the simulator */
{
  int i;
  float sum, avg;
  float jimsrand();
  
  
   printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
   printf("Enter the number of messages to simulate: ");
   scanf("%d",&nsimmax);
   printf("Enter  packet loss probability [enter 0.0 for no loss]:");
   scanf("%f",&lossprob);
   printf("Enter packet corruption probability [0.0 for no corruption]:");
   scanf("%f",&corruptprob);
   printf("Enter average time between messages from sender's layer5 [ > 0.0]:");
   scanf("%f",&lambda);
   printf("Enter TRACE:");
   scanf("%d",&TRACE);

   srand(seed);              /* init random number generator */
   sum = 0.0;                /* test random number generator for students */
   for (i=0; i<1000; i++)
      sum=sum+jimsrand();    /* jimsrand() should be uniform in [0,1] */
   avg = sum/1000.0;
   if (avg < 0.25 || avg > 0.75) {
    printf("It is likely that random number generation on your machine\n" ); 
    printf("is different from what this emulator expects.  Please take\n");
    printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
    exit(0);
    }

   ntolayer3 = 0;
   nlost = 0;
   ncorrupt = 0;

   time=0.0;                    /* initialize time to 0.0 */
   generate_next_arrival();     /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand() 
{
  double mmm = 2147483647;   /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
  float x;                   /* individual students may need to change mmm */ 
  x = rand()/mmm;            /* x should be uniform in [0,1] */
  return(x);
}  

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/
 
void generate_next_arrival()
{
   double x,log(),ceil();
   struct event *evptr;
    //char *malloc();
   float ttime;
   int tempint;

   if (TRACE>2)
       printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");
 
   x = lambda*jimsrand()*2;  /* x is uniform on [0,2*lambda] */
                             /* having mean of lambda        */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + x;
   evptr->evtype =  FROM_LAYER5;
   if (BIDIRECTIONAL && (jimsrand()>0.5) )
      evptr->eventity = B;
    else
      evptr->eventity = A;
   insertevent(evptr);
} 


void insertevent(p)
   struct event *p;
{
   struct event *q,*qold;

   if (TRACE>2) {
      printf("            INSERTEVENT: time is %lf\n",time);
      printf("            INSERTEVENT: future time will be %lf\n",p->evtime); 
      }
   q = evlist;     /* q points to header of list in which p struct inserted */
   if (q==NULL) {   /* list is empty */
        evlist=p;
        p->next=NULL;
        p->prev=NULL;
        }
     else {
        for (qold = q; q !=NULL && p->evtime > q->evtime; q=q->next)
              qold=q; 
        if (q==NULL) {   /* end of list */
             qold->next = p;
             p->prev = qold;
             p->next = NULL;
             }
           else if (q==evlist) { /* front of list */
             p->next=evlist;
             p->prev=NULL;
             p->next->prev=p;
             evlist = p;
             }
           else {     /* middle of list */
             p->next=q;
             p->prev=q->prev;
             q->prev->next=p;
             q->prev=p;
             }
         }
}

void printevlist()
{
  struct event *q;
  int i;
  printf("--------------\nEvent List Follows:\n");
  for(q = evlist; q!=NULL; q=q->next) {
    printf("Event time: %f, type: %d entity: %d\n",q->evtime,q->evtype,q->eventity);
    }
  printf("--------------\n");
}



/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(AorB)
int AorB;  /* A or B is trying to stop timer */
{
 struct event *q,*qold;

 if (TRACE>2)
    printf("          STOP TIMER: stopping timer at %f\n",time);
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
 for (q=evlist; q!=NULL ; q = q->next) 
    if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
       /* remove this event */
       if (q->next==NULL && q->prev==NULL)
             evlist=NULL;         /* remove first and only event on list */
          else if (q->next==NULL) /* end of list - there is one in front */
             q->prev->next = NULL;
          else if (q==evlist) { /* front of list - there must be event after */
             q->next->prev=NULL;
             evlist = q->next;
             }
           else {     /* middle of list */
             q->next->prev = q->prev;
             q->prev->next =  q->next;
             }
       free(q);
       return;
     }
  printf("Warning: unable to cancel your timer. It wasn't running.\n");
}


void starttimer(AorB,increment)
int AorB;  /* A or B is trying to stop timer */
float increment;
{

 struct event *q;
 struct event *evptr;
 //char *malloc();

 if (TRACE>2)
    printf("          START TIMER: starting timer at %f\n",time);
 /* be nice: check to see if timer is already started, if so, then  warn */
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
   for (q=evlist; q!=NULL ; q = q->next)  
    if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
      printf("Warning: attempt to start a timer that is already started\n");
      return;
      }
 
/* create future event for when timer goes off */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + increment;
   evptr->evtype =  TIMER_INTERRUPT;
   evptr->eventity = AorB;
   insertevent(evptr);
} 


/************************** TOLAYER3 ***************/
void tolayer3(AorB,packet)
int AorB;  /* A or B is trying to stop timer */
struct pkt packet;
{
 struct pkt *mypktptr;
 struct event *evptr,*q;
 //char *malloc();
 float lastime, x, jimsrand();
 int i;


 ntolayer3++;

 /* simulate losses: */
 if (jimsrand() < lossprob)  {
      nlost++;
      if (TRACE>0)    
	printf("          TOLAYER3: packet being lost\n");
      return;
    }  

/* make a copy of the packet student just gave me since he/she may decide */
/* to do something with the packet after we return back to him/her */ 
 mypktptr = (struct pkt *)malloc(sizeof(struct pkt));
 mypktptr->seqnum = packet.seqnum;
 mypktptr->acknum = packet.acknum;
 mypktptr->checksum = packet.checksum;
 for (i=0; i<20; i++)
    mypktptr->payload[i] = packet.payload[i];
 if (TRACE>2)  {
   printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
	  mypktptr->acknum,  mypktptr->checksum);
    for (i=0; i<20; i++)
        printf("%c",mypktptr->payload[i]);
    printf("\n");
   }

/* create future event for arrival of packet at the other side */
  evptr = (struct event *)malloc(sizeof(struct event));
  evptr->evtype =  FROM_LAYER3;   /* packet will pop out from layer3 */
  evptr->eventity = (AorB+1) % 2; /* event occurs at other entity */
  evptr->pktptr = mypktptr;       /* save ptr to my copy of packet */
/* finally, compute the arrival time of packet at the other end.
   medium can not reorder, so make sure packet arrives between 1 and 10
   time units after the latest arrival time of packets
   currently in the medium on their way to the destination */
 lastime = time;
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
 for (q=evlist; q!=NULL ; q = q->next) 
    if ( (q->evtype==FROM_LAYER3  && q->eventity==evptr->eventity) ) 
      lastime = q->evtime;
 evptr->evtime =  lastime + 1 + 9*jimsrand();
 


 /* simulate corruption: */
 if (jimsrand() < corruptprob)  {
    ncorrupt++;
    if ( (x = jimsrand()) < .75)
       mypktptr->payload[0]='Z';   /* corrupt payload */
      else if (x < .875)
       mypktptr->seqnum = 999999;
      else
       mypktptr->acknum = 999999;
    if (TRACE>0)    
	printf("          TOLAYER3: packet being corrupted\n");
    }  

  if (TRACE>2)  
     printf("          TOLAYER3: scheduling arrival on other side\n");
  insertevent(evptr);
} 

void tolayer5(AorB,datasent)
  int AorB;
  char datasent[20];
{
  int i;  
  if (TRACE>2) {
     printf("          TOLAYER5: data received: ");
     for (i=0; i<20; i++)  
        printf("%c",datasent[i]);
     printf("\n");
   }
  
}