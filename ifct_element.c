//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

char* ifctele_getPlaceName(int placeIndex)//환자 이동장소(숫자) 문자열로 변환 
{
	return countryName[placeIndex];
}

typedef struct ifs_ele//구조체 형성 
{
	
	int      index;
	int      age;
	int      detected_time;
	place_t  place[N_HISTORY];
	 
} ifs_ele_t;



void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])//구조체에 정보 입력 
{
	ifs_ele_t* ptr;
	
	int i;
	ptr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t));
    ptr->index = index;
	ptr->age=age;
	ptr->detected_time=detected_time;
	
	for(i=0;i<N_HISTORY;i++)
	{
		ptr->place[i]=history_place[i];
	}
	
	return ptr;
}


int ifctele_getAge(void* obj)// 환자 나이 불러오기 
{
	ifs_ele_t*ptr = (ifs_ele_t*)obj;
	
	return(ptr->age);
	
}

unsigned int ifctele_getinfestedTime(void* obj)//환자 감염시점 불러오기 
{
	ifs_ele_t*ptr = (ifs_ele_t*)obj;
	
	return(ptr->detected_time);
	
}

int ifctele_getHistPlaceIndex(void* obj, int index)//환자 index번째 이동 장소 불러오기 
{
	ifs_ele_t*ptr = (ifs_ele_t*)obj;
	
	return(ptr->place[index]);
	
}


void ifsele_printElement(void*obj)//환자 정보 출력하기 
{
	int i;
	ifs_ele_t* ptr = (ifs_ele_t*)obj;
	
	printf("Patient age : %i\n",ptr->age);
	printf("Detected time : %i\n",ptr->detected_time);
	
	printf("Path History : ");
	for(i=0;i<N_HISTORY;i++)//이동장소 수 만큼 반복 
	{
		printf("%s(%d)  ",ifctele_getPlaceName(ptr->place[i]),ptr->detected_time-(N_HISTORY-(i+1)));//detected_time-(N_HISTORY-(i+1)=장소에 있던 시점 계산 
	}
	printf("\n");
}


