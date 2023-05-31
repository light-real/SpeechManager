#include<iostream>
using namespace std;
#include "speechManager.h"
#include<map>
#include<string> 
#include<ctime>
int main(){
	
	//添加一个随机数种子
	srand((unsigned int)time(NULL)); 
	speechManager sm;

	int choice; 
	while(true){
		sm.showMenu();
		cout<<"请输入你的选择："<<endl;
		cin>>choice;
		switch(choice){
			case 1:   //开始演讲比赛 
				sm.startSpeech();
				break;
			case 2:  //查看往届记录 
				sm.showRecord();
				break;
			case 3:  //清空比赛记录 
				sm.clearRecord();
				break;	
			case 0:  //退出比赛程序 
				sm.exitSystem(); 
				break;  	
			default:
				system("cls");
				break;	
		}

	}
	
	
	system("pause");
	return 0;
} 
