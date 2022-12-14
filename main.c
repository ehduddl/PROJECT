//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define N_HISTORY           5
#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2




int isMet(int Patient,int Propagator);

int trackInfester(int patient_no);

int Met_place(int Patient);


int main(int argc, const char * argv[]) {
    
   
   
int i,j;
int menu_selection;
void *ifct_element;    
int pIndex, age, time;
int placeHist[N_HISTORY];
FILE*fp;
fp = fopen("patientInfo_prb5.txt","r");




//------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
   
 
	while (3==fscanf(fp,"%d %d %d",&pIndex,&age,&time))
	{
		{
	    	for(i=0;i<N_HISTORY;i++)
				fscanf(fp,"%d",&placeHist[i]);
				
		}
		ifct_element = ifctele_genElement(pIndex, age,time,placeHist);
		ifctdb_addTail(ifct_element);
	}

		
    fclose(fp);
    

    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
            	
            	{
            		printf("\n조사할 환자를 선택하시오.");
            		scanf("%d",&pIndex);
            		
            		ifct_element=ifctdb_getData(pIndex);
            		
					printf("\n--------[환자정보]-------\n");
            		printf("Patient index : %d\n",pIndex);
					ifsele_printElement(ifct_element);
					printf("\n-------------------------\n");
			       	
				}
            	
                break;
                
            case MENU_PLACE:
            	{
            		int i,num=0,a;
					char entry_place[100];
			        printf("\n장소이름을 입력하시오: ");
			        scanf("%s",&entry_place);
			        
			        for(pIndex=0;pIndex<ifctdb_len();pIndex++)
            		{
            			ifct_element=ifctdb_getData(pIndex);
            	
				    if (strcmp(entry_place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,N_HISTORY-1)))==0) 
            		{
            			printf("\n--------[환자정보]-------\n");
            		    printf("Patient index : %d\n",pIndex);
						ifsele_printElement(ifct_element);
						printf("\n-------------------------\n");
						num++;
					}
					
					}
			        
			        printf("\nThere are %d patient detected in %s\n",num,entry_place);
				}
			
			        

				
            	
                
                break;
                
            case MENU_AGE:
            		
					{
						
					int max_age, min_age,i;
					int num=0; 
					printf("\n최소나이를 입력하시오.");
					scanf("%d",&min_age);
            		printf("\n최대나이를 입력하시오.");
            		scanf("%d",&max_age);
            		
            		for(pIndex=0;pIndex<ifctdb_len();pIndex++)
            		{
            			ifct_element=ifctdb_getData(pIndex);
					if(ifctele_getAge(ifct_element)>=min_age&&ifctele_getAge(ifct_element)<=max_age)
            		{
            		    printf("\n--------[환자정보]-------\n");
            		    printf("Patient index : %d\n",pIndex);
						ifsele_printElement(ifct_element);
						printf("\n-------------------------\n");
						num++;
					}
					
					}
					printf("\nThere are %d whose age is between %d and %d\n",num,min_age,max_age);
					}
					
					
                break;
                
            case MENU_TRACK:
            	
            {
            	
           
			int isMet(int Patient,int Propagator)
                {
	                int i,j;
					int place_i,place_j;
					int a,b;//현재환자,대상환자 감염 시점 
					int meet_time=-1,time;
					
					for (i=0;i<N_HISTORY;i++)
					{
					    ifct_element=ifctdb_getData(Patient);//현재환자 정보 불러옴 
						a = ifctele_getinfestedTime(ifct_element);//현재환자 감염시점 불러옴 
						time = a-(N_HISTORY-(i+1));//i번째 시점계산 
						place_i=ifctele_getHistPlaceIndex(ifct_element,i);//i번째 이동 장소
	    
	    
			            ifct_element=ifctdb_getData(Propagator);// 대상 환자 정보 불러옴
						b=ifctele_getinfestedTime(ifct_element);//대상 환자 감염시점 불러옴 
			
		                for(j=N_HISTORY-2;j<N_HISTORY;j++) 
                        {
				            if(time==b-(N_HISTORY-(j+1)))//현재환자와 대상환자 j번째에서 만남 
				
				            {
					            place_j=ifctele_getHistPlaceIndex(ifct_element,j);// 대상환자의 j번째 장소 
				
				                if (place_i == place_j)//현재환자와 대상환자가 같은 공간에 있음 
				                {
                                    meet_time = time;
                    
                                }
                                
                                
				            }
				
			            }	
                    }
	                
					return meet_time;
                }
			
			
				int min_metTime=10000;
				
				int trackInfester(int patient_no)
                {
   
                    int met_time,Propagator=-1;
                    int i;
                    
					for (i=0;(i<ifctdb_len());i++)
					{
						if(i!=patient_no)
						{
	                    {
                            met_time = isMet(patient_no,i);
                            if ( met_time > 0) //만났다면
                            {
                                if (met_time<min_metTime)
                                {
                                    Propagator= i;
                                    min_metTime = met_time;
                                }
                            }
                        }
                
                    
					}
					}
                    
				
					return Propagator;
                }        
                
                int Met_place(int Patient)
                {
                	
                	ifct_element=ifctdb_getData(Patient); 
					
					ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-(ifctele_getinfestedTime(ifct_element)-min_metTime+1));
                	
				}
                
            	
            	int track_ID;
				int Current_Patient, Propagator, First_Preachers;
				 
				printf("\n조사할 환자를 입력하시오 : ");
            	scanf("%d",&track_ID);
				
				
				Current_Patient = track_ID;
				
				while (Current_Patient>-1)
                {
                    Propagator = trackInfester(Current_Patient);
                    
					if (Propagator>-1)
                    printf("--> [TRACKING] patient %d is infected by %d(time : %d, place : %s)\n",Current_Patient,Propagator,min_metTime,ifctele_getPlaceName(Met_place(Current_Patient)));
								
                    
					else
                    {
                    	First_Preachers = Current_Patient;
						if(track_ID==First_Preachers)
						printf("\n%d is the first infecter!!!\n",track_ID,First_Preachers);
						else
						printf("\nThe infecter of %d is %d\n",track_ID,First_Preachers);
					}
					
					Current_Patient = Propagator;
					
					
                    
				
                }
            
            	
        
				
				}	
			
            	
				                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}
