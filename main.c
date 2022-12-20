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
    
   
 
	while (3==fscanf(fp,"%d %d %d",&pIndex,&age,&time))//인덱스, 나이, 감염시점을 정상적으로 불러오면 
	{
		{
	    	for(i=0;i<N_HISTORY;i++)
				fscanf(fp,"%d",&placeHist[i]);// 이동 장소 불러오기 
				
		}
		ifct_element = ifctele_genElement(pIndex, age,time,placeHist);//구조체로 저장 
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
                
            case MENU_PATIENT://특정 환자에 대한 정보 출력
            	
            	{
            		printf("\n조사할 환자를 선택하시오.");
            		scanf("%d",&pIndex);
            		
            		ifct_element=ifctdb_getData(pIndex);//입력받은 환자정보 불러오기 
            		
					printf("\n--------[환자정보]-------\n"); 
            		printf("Patient index : %d\n",pIndex);
					ifsele_printElement(ifct_element);//환자정보 출력
					printf("\n-------------------------\n");
			       	
				}
            	
                break;
                
            case MENU_PLACE://특정 장소에서 감염이 확인된 환자 관련 정보 출력
            	{
					int Patient_num=0;//조건 만족하는 환자 수 
					char input_place[100];//입력받은 장소이름 
			        printf("\n장소이름을 입력하시오: ");
			        scanf("%s",&input_place);
			        
			        for(pIndex=0;pIndex<ifctdb_len();pIndex++)//환자수 만큼 반복 
            		{
            			ifct_element=ifctdb_getData(pIndex);//환자정보 불러오기 
            	
					    if (strcmp(input_place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,N_HISTORY-1)))==0) //입력받은 장소와 환자가 감염된 장소가 같은지 비교 
	            		{
	            			printf("\n--------[환자정보]-------\n");//같으면 환자정보 출력 
	            		    printf("Patient index : %d\n",pIndex);
							ifsele_printElement(ifct_element);
							printf("\n-------------------------\n");
							Patient_num++; 
						}
						
					}
			        
			        printf("\nThere are %d patient detected in %s\n",Patient_num,input_place);
				}
			
			        

				
            	
                
                break;
                
            case MENU_AGE://특정 범위의 나이에 해당하는 환자 관련 정보 출력
            		
				{
					
					int max_age, min_age;//입력받을 최대,최소 나이 
					int Patient_num=0; //조건 만족하는 환자 수 
					
					printf("\n최소나이를 입력하시오.");
					scanf("%d",&min_age);
	        		
					printf("\n최대나이를 입력하시오.");
	        		scanf("%d",&max_age);
	        		
	        		for(pIndex=0;pIndex<ifctdb_len();pIndex++)//환자수 만큼 반복 
	        		{
	        			ifct_element=ifctdb_getData(pIndex);//환자 정보 불러오기 
						if(ifctele_getAge(ifct_element)>=min_age&&ifctele_getAge(ifct_element)<=max_age)//환자 나이가 최솟값 보다 크고, 최댓값 보다 작으면 
		        		{
		        		    printf("\n--------[환자정보]-------\n");//환자정보 출력 
		        		    printf("Patient index : %d\n",pIndex);
							ifsele_printElement(ifct_element);
							printf("\n-------------------------\n");
							
							Patient_num++;
						}
					
					}
					printf("\nThere are %d whose age is between %d and %d\n",Patient_num,min_age,max_age);
				}
				
					
                break;
                
            case MENU_TRACK://감염 경로 및 최초 전파자 추적
            	
            {
           	
           		int isMet(int Current_Patient,int Target_Patient)//현재환자와 대상환자가 만난시점 계산 함수 
                {
	                int i,j;//for문 돌릴 변수 
					int place_i;//현재환자의 i번째 이동장소 
					int place_j;// 대상환자의 j번째 장소
					int dtime_CP,dtime_TP;//(dtime=dtectedtime),(C=Current),(T=Target),(P=Patient) 현재환자,대상환자 감염 시점 
					int meet_time=-1,time_i;
					
					for (i=0;i<N_HISTORY;i++)
					{
					    ifct_element=ifctdb_getData(Current_Patient);//현재환자 정보 불러옴 
						dtime_CP = ifctele_getinfestedTime(ifct_element);//현재환자 감염시점 불러옴 
						time_i= dtime_CP-(N_HISTORY-(i+1));//i번째 시점계산 
						place_i=ifctele_getHistPlaceIndex(ifct_element,i);//i번째 이동 장소
	    
	    			    ifct_element=ifctdb_getData(Target_Patient);// 대상 환자 정보 불러옴
						dtime_TP=ifctele_getinfestedTime(ifct_element);//대상 환자 감염시점 불러옴 
			
		                for(j=N_HISTORY-2;j<N_HISTORY;j++) 
                        {
				            if(time_i==dtime_TP-(N_HISTORY-(j+1)))//현재환자와 대상환자 j번째에서 만남 
							{
					            place_j=ifctele_getHistPlaceIndex(ifct_element,j);// 대상환자의 j번째 장소 
				
				                if (place_i == place_j)//현재환자와 대상환자가 같은 공간에 있음 
				                {
                                    meet_time = time_i;
                                }
                            }
				        }	
                    }
	                
					return meet_time;
                }
			
			
				int min_metTime=10000;//가장 빨리 만난 시간 
				
				int trackInfester(int patient_no)
                {
   
                    int met_time; 
					int Propagator=-1;
                    int i;
                    
					for (i=0;i<ifctdb_len();i++)//환자수 만큼 반복 
					{
						if(i!=patient_no)//현재환자와 대상환자가 다를때 
						{
                            met_time = isMet(patient_no,i);
                            if ( met_time > 0) //만났다면
                            {
                                if (met_time<min_metTime)//지금까지 환자 중 만난시간이 가장 이른 시점 찾기 
                                {
                                    Propagator= i;
                                    min_metTime = met_time;
                                }
                            }
                        }
                    }
                    
					return Propagator;
                }        
                
                int Met_place(int Patient)//감염 장소 계산 
                {
                	
                	ifct_element=ifctdb_getData(Patient); //환자 정보 불러오기 
					
					ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-(ifctele_getinfestedTime(ifct_element)-min_metTime+1));
                	//N_HISTORY-(ifctele_getinfestedTime(ifct_element)-min_metTime+1 = 감염 날짜가 몇번째 이동장소인지 계산 
				}
                
            	
            	int track_ID;//조사할 환자 
				int Current_Patient, Propagator, First_Preachers;//현재환자,전파자,최초전파자 
				 
				printf("\n조사할 환자를 입력하시오 : ");
            	scanf("%d",&track_ID);
				
				
				Current_Patient = track_ID;//조사할 환자를 현재환자로 지정 
				
				while (Current_Patient>-1)//현재환자가 존재하면 
                {
                    Propagator = trackInfester(Current_Patient);
                    
					if (Propagator>-1)//전파자가 존재하면 
                    printf("--> [TRACKING] patient %d is infected by %d(time : %d, place : %s)\n",Current_Patient,Propagator,min_metTime,ifctele_getPlaceName(Met_place(Current_Patient)));//전파자,감염시점,감염장소 출력 
						
						
                    
					else//전파자 존재하지 않으면 
                    {
                    	First_Preachers = Current_Patient;
						if(track_ID==First_Preachers)//조사한  환자가 첫번째 전파자면 
						printf("\n%d is the first infecter!!!\n",track_ID,First_Preachers);
						else//조사한 환자가 첫번재 전파자가 아니면 
						printf("\nThe infecter of %d is %d\n",track_ID,First_Preachers);
					}
					
					Current_Patient = Propagator;//전파자를 현재환자로 설정하여 track 반복	ㅐ	                    			
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
