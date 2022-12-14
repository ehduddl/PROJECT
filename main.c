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
//int trackInfester(int patient_no, int *detected_time, int *place);

int main(int argc, const char * argv[]) {
    
   
   
int i,j;
int menu_selection;
void *ifct_element;    
int pIndex, age, time;
int placeHist[N_HISTORY];
FILE*fp;
fp = fopen("patientInfo_sample.txt","r");




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
            		printf("������ ȯ�ڸ� �����Ͻÿ�.");
            		scanf("%d",&pIndex);
            		
            		ifct_element=ifctdb_getData(pIndex);
            		
					printf("\n--------[ȯ������]-------\n");
            		printf("Index : %d\n",pIndex);
            		ifsele_printElement(ifct_element);
			       	
				}
            	
                break;
                
            case MENU_PLACE:
            	{
            		int i,num,a;
					char entry_place[100];
			        printf("����̸��� �Է��Ͻÿ�: ");
			        scanf("%s",&entry_place);
			        
			        for(pIndex=0;pIndex<ifctdb_len();pIndex++)//================================================================
            		{
            			ifct_element=ifctdb_getData(pIndex);
            	
				    if (strcmp(entry_place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,N_HISTORY-1)))==0) 
            		{
            			printf("\n--------[ȯ������]-------\n");
            		    printf("Index : %d\n",pIndex);
						ifsele_printElement(ifct_element);
					}
				
					
					}
			        
			        
				}
			
			        

				
            	
                
                break;
                
            case MENU_AGE:
            		
					{
						
					int max_age, min_age,i; 
					printf("�ּҳ��̸� �Է��Ͻÿ�.");
					scanf("%d",&min_age);
            		printf("�ִ볪�̸� �Է��Ͻÿ�.");
            		scanf("%d",&max_age);
            		
            		for(pIndex=0;pIndex<ifctdb_len();pIndex++)//================================================================
            		{
            			ifct_element=ifctdb_getData(pIndex);
					if(ifctele_getAge(ifct_element)>=min_age&&ifctele_getAge(ifct_element)<=max_age)
            		{
            		    printf("\n--------[ȯ������]-------\n");
            		    printf("Index : %d\n",pIndex);
						ifsele_printElement(ifct_element);
					}
					
					}
					}
					
					
                break;
                
            case MENU_TRACK:
            	
            {
            	
            
			int isMet(int Patient,int Propagator)
                {
	                int i,j;
					int place_i,place_j;
					int a,b;//����ȯ��,���ȯ�� ���� ���� 
					int meet_time=-1,time;
					
					for (i=0;i<N_HISTORY;i++)
					{
					    ifct_element=ifctdb_getData(Patient);//����ȯ�� ���� �ҷ��� 
						a = ifctele_getinfestedTime(ifct_element);//����ȯ�� �������� �ҷ��� 
						time = a-(N_HISTORY-(i+1));//i��° ������� 
						place_i=ifctele_getHistPlaceIndex(ifct_element,i);//i��° �̵� ���
	    
	    
			            ifct_element=ifctdb_getData(Propagator);// ��� ȯ�� ���� �ҷ���
						b=ifctele_getinfestedTime(ifct_element);//��� ȯ�� �������� �ҷ��� 
			
		                for(j=N_HISTORY-2;j<N_HISTORY;j++) 
                        {
				            if(time==b-(N_HISTORY-(j+1)))//����ȯ�ڿ� ���ȯ�� j��°���� ���� 
				
				            {
					            place_j=ifctele_getHistPlaceIndex(ifct_element,j);// ���ȯ���� j��° ��� 
				
				                if (place_i == place_j)//����ȯ�ڿ� ���ȯ�ڰ� ���� ������ ���� 
				                {
                                    meet_time = time;
                                }
                                
                                
				            }
				
			            }	
                    }
	                
					return meet_time;
                }
			
			
				
				int trackInfester(int patient_no)
                {
   
                    int met_time,Propagator=-1;
                    int min_metTime=10000;
                    int i;
                    
					for (i=0;i<ifctdb_len();i++)
                    {
	                    {
                            met_time = isMet(patient_no,i);
                            if ( met_time > 0) //�����ٸ�
                            {
                                if (met_time<min_metTime)
                                {
                                    Propagator= i;
                                    min_metTime = met_time;
                                }
                            }
                        }
                
                    
					}
				
					return Propagator;
                }
        
            	
            	int track_ID;
				int Current_Patient, Propagator, First_Preachers;
				 
				printf("������ ȯ�ڸ� �Է��Ͻÿ� : ");
            	scanf("%d",&track_ID);
				
				
				Current_Patient = track_ID;
				
				//while (Current_Patient>=0)//while (Current_Patient!=NULL)
                {
                    Propagator = trackInfester(Current_Patient);
                    if (Propagator>0)
                    printf("%i ȯ�ڴ� %i ȯ�ڿ��� ���ĵ�\n",Current_Patient,Propagator);
                    
					else
                    First_Preachers =Current_Patient;
                    Current_Patient=Propagator;
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
