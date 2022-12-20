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
    
   
 
	while (3==fscanf(fp,"%d %d %d",&pIndex,&age,&time))//�ε���, ����, ���������� ���������� �ҷ����� 
	{
		{
	    	for(i=0;i<N_HISTORY;i++)
				fscanf(fp,"%d",&placeHist[i]);// �̵� ��� �ҷ����� 
				
		}
		ifct_element = ifctele_genElement(pIndex, age,time,placeHist);//����ü�� ���� 
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
                
            case MENU_PATIENT://Ư�� ȯ�ڿ� ���� ���� ���
            	
            	{
            		printf("\n������ ȯ�ڸ� �����Ͻÿ�.");
            		scanf("%d",&pIndex);
            		
            		ifct_element=ifctdb_getData(pIndex);//�Է¹��� ȯ������ �ҷ����� 
            		
					printf("\n--------[ȯ������]-------\n"); 
            		printf("Patient index : %d\n",pIndex);
					ifsele_printElement(ifct_element);//ȯ������ ���
					printf("\n-------------------------\n");
			       	
				}
            	
                break;
                
            case MENU_PLACE://Ư�� ��ҿ��� ������ Ȯ�ε� ȯ�� ���� ���� ���
            	{
					int Patient_num=0;//���� �����ϴ� ȯ�� �� 
					char input_place[100];//�Է¹��� ����̸� 
			        printf("\n����̸��� �Է��Ͻÿ�: ");
			        scanf("%s",&input_place);
			        
			        for(pIndex=0;pIndex<ifctdb_len();pIndex++)//ȯ�ڼ� ��ŭ �ݺ� 
            		{
            			ifct_element=ifctdb_getData(pIndex);//ȯ������ �ҷ����� 
            	
					    if (strcmp(input_place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,N_HISTORY-1)))==0) //�Է¹��� ��ҿ� ȯ�ڰ� ������ ��Ұ� ������ �� 
	            		{
	            			printf("\n--------[ȯ������]-------\n");//������ ȯ������ ��� 
	            		    printf("Patient index : %d\n",pIndex);
							ifsele_printElement(ifct_element);
							printf("\n-------------------------\n");
							Patient_num++; 
						}
						
					}
			        
			        printf("\nThere are %d patient detected in %s\n",Patient_num,input_place);
				}
			
			        

				
            	
                
                break;
                
            case MENU_AGE://Ư�� ������ ���̿� �ش��ϴ� ȯ�� ���� ���� ���
            		
				{
					
					int max_age, min_age;//�Է¹��� �ִ�,�ּ� ���� 
					int Patient_num=0; //���� �����ϴ� ȯ�� �� 
					
					printf("\n�ּҳ��̸� �Է��Ͻÿ�.");
					scanf("%d",&min_age);
	        		
					printf("\n�ִ볪�̸� �Է��Ͻÿ�.");
	        		scanf("%d",&max_age);
	        		
	        		for(pIndex=0;pIndex<ifctdb_len();pIndex++)//ȯ�ڼ� ��ŭ �ݺ� 
	        		{
	        			ifct_element=ifctdb_getData(pIndex);//ȯ�� ���� �ҷ����� 
						if(ifctele_getAge(ifct_element)>=min_age&&ifctele_getAge(ifct_element)<=max_age)//ȯ�� ���̰� �ּڰ� ���� ũ��, �ִ� ���� ������ 
		        		{
		        		    printf("\n--------[ȯ������]-------\n");//ȯ������ ��� 
		        		    printf("Patient index : %d\n",pIndex);
							ifsele_printElement(ifct_element);
							printf("\n-------------------------\n");
							
							Patient_num++;
						}
					
					}
					printf("\nThere are %d whose age is between %d and %d\n",Patient_num,min_age,max_age);
				}
				
					
                break;
                
            case MENU_TRACK://���� ��� �� ���� ������ ����
            	
            {
           	
           		int isMet(int Current_Patient,int Target_Patient)//����ȯ�ڿ� ���ȯ�ڰ� �������� ��� �Լ� 
                {
	                int i,j;//for�� ���� ���� 
					int place_i;//����ȯ���� i��° �̵���� 
					int place_j;// ���ȯ���� j��° ���
					int dtime_CP,dtime_TP;//(dtime=dtectedtime),(C=Current),(T=Target),(P=Patient) ����ȯ��,���ȯ�� ���� ���� 
					int meet_time=-1,time_i;
					
					for (i=0;i<N_HISTORY;i++)
					{
					    ifct_element=ifctdb_getData(Current_Patient);//����ȯ�� ���� �ҷ��� 
						dtime_CP = ifctele_getinfestedTime(ifct_element);//����ȯ�� �������� �ҷ��� 
						time_i= dtime_CP-(N_HISTORY-(i+1));//i��° ������� 
						place_i=ifctele_getHistPlaceIndex(ifct_element,i);//i��° �̵� ���
	    
	    			    ifct_element=ifctdb_getData(Target_Patient);// ��� ȯ�� ���� �ҷ���
						dtime_TP=ifctele_getinfestedTime(ifct_element);//��� ȯ�� �������� �ҷ��� 
			
		                for(j=N_HISTORY-2;j<N_HISTORY;j++) 
                        {
				            if(time_i==dtime_TP-(N_HISTORY-(j+1)))//����ȯ�ڿ� ���ȯ�� j��°���� ���� 
							{
					            place_j=ifctele_getHistPlaceIndex(ifct_element,j);// ���ȯ���� j��° ��� 
				
				                if (place_i == place_j)//����ȯ�ڿ� ���ȯ�ڰ� ���� ������ ���� 
				                {
                                    meet_time = time_i;
                                }
                            }
				        }	
                    }
	                
					return meet_time;
                }
			
			
				int min_metTime=10000;//���� ���� ���� �ð� 
				
				int trackInfester(int patient_no)
                {
   
                    int met_time; 
					int Propagator=-1;
                    int i;
                    
					for (i=0;i<ifctdb_len();i++)//ȯ�ڼ� ��ŭ �ݺ� 
					{
						if(i!=patient_no)//����ȯ�ڿ� ���ȯ�ڰ� �ٸ��� 
						{
                            met_time = isMet(patient_no,i);
                            if ( met_time > 0) //�����ٸ�
                            {
                                if (met_time<min_metTime)//���ݱ��� ȯ�� �� �����ð��� ���� �̸� ���� ã�� 
                                {
                                    Propagator= i;
                                    min_metTime = met_time;
                                }
                            }
                        }
                    }
                    
					return Propagator;
                }        
                
                int Met_place(int Patient)//���� ��� ��� 
                {
                	
                	ifct_element=ifctdb_getData(Patient); //ȯ�� ���� �ҷ����� 
					
					ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-(ifctele_getinfestedTime(ifct_element)-min_metTime+1));
                	//N_HISTORY-(ifctele_getinfestedTime(ifct_element)-min_metTime+1 = ���� ��¥�� ���° �̵�������� ��� 
				}
                
            	
            	int track_ID;//������ ȯ�� 
				int Current_Patient, Propagator, First_Preachers;//����ȯ��,������,���������� 
				 
				printf("\n������ ȯ�ڸ� �Է��Ͻÿ� : ");
            	scanf("%d",&track_ID);
				
				
				Current_Patient = track_ID;//������ ȯ�ڸ� ����ȯ�ڷ� ���� 
				
				while (Current_Patient>-1)//����ȯ�ڰ� �����ϸ� 
                {
                    Propagator = trackInfester(Current_Patient);
                    
					if (Propagator>-1)//�����ڰ� �����ϸ� 
                    printf("--> [TRACKING] patient %d is infected by %d(time : %d, place : %s)\n",Current_Patient,Propagator,min_metTime,ifctele_getPlaceName(Met_place(Current_Patient)));//������,��������,������� ��� 
						
						
                    
					else//������ �������� ������ 
                    {
                    	First_Preachers = Current_Patient;
						if(track_ID==First_Preachers)//������  ȯ�ڰ� ù��° �����ڸ� 
						printf("\n%d is the first infecter!!!\n",track_ID,First_Preachers);
						else//������ ȯ�ڰ� ù���� �����ڰ� �ƴϸ� 
						printf("\nThe infecter of %d is %d\n",track_ID,First_Preachers);
					}
					
					Current_Patient = Propagator;//�����ڸ� ����ȯ�ڷ� �����Ͽ� track �ݺ�	��	                    			
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
