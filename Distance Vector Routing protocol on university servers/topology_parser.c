#include <stdio.h>
#include <string.h>

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

int main()
{

FILE *f;
int i,count_of_sentences;
count_of_sentences=0;

f = fopen("topology_example.txt","r");

char topology[255];

memset(topology,'\0', 255);

for(i=0; i<255 ; i++)
topology[i]=fgetc(f);


   
printf("%s\n", topology );

printf("%d \n", count_of_sentences);

printf(" Number of servers : %d \n", number_of_servers(topology));

printf(" Number of neighbors : %d \n", number_of_neighbors(topology));




// Converting Topology file string into the sentences

int y=0;
y=2+number_of_neighbors(topology)+number_of_servers(topology);


char token1[25];
//memset(token1, '\0', 25);
char *token2;

struct sentence
{
		char *sen;	
};	

struct sentence sentences[10];
for(i=0;i<10;i++)
//memset(sentences[i].sen, '\0', 25);
sentences[i].sen="";
i=0;
const char s[2] = "\n";
   char *token;
   token="";
   /* get the first token */
   token = strtok(topology, s);
   
   /* walk through other tokens */
  while( token != NULL || y>0) 
   {
      //printf( "%s\n", token );
	  sentences[i].sen = token ;
      token = strtok(NULL, s);
	  i++;
	  y--;
   }
   
	i=0;

	for(i=0; i<10; i++)
	printf("%s \n",(sentences[i].sen));


printf("\n\n\n\n %s", sentences[0].sen);
printf("\n\n\n\n %s", sentences[1].sen);
printf("\n\n\n\n %s", sentences[2].sen);
printf("\n\n\n\n %s", sentences[3].sen);
printf("\n\n\n\n %s", sentences[4].sen);



/*
int y=0;
y=2+number_of_neighbors(topology)+number_of_servers(topology);

printf("y: %d \n", y);

struct sentence
{
char *sen;	
};	

struct sentence sentences[10];

for(i=0;i<10;i++)
sentences[i].sen='\0';

int count1,j;
count1=0;
j=0;
char *tmp;
tmp='\0';


char *append;
append='\0'; 
 
	while(y!=0)
	{
		if(topology[j]!='\0')
		{
			append=&topology[j];
			strcat(&tmp, &append);		
		}
		else
		{
			sentences[count1].sen=tmp;
			count1++;
			y--;	
		}			

	}
 
 
 for(i=0;i<10;i++)
printf("Sentence %d : %s \n", (i+1), sentences[i].sen);
*/ 
 
fclose(f);
}
