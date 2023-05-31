#pragma once
#include<iostream>
#include<vector>
#include<map>
#include "speaker.h"
#include<string>
#include<algorithm>
#include<deque>
#include<numeric>
#include<fstream>
using namespace std;

class speechManager{
	public:
//		构造函数 
		speechManager();  //在speechManager.h中先声明，在speechManager.cpp中进行实现
		
//		菜单功能 
		void showMenu(); 
		
//		初始化属性
		void initSpeech(); 
		
		//创建选手
		void createSpeaker(); 
		
//		开始比赛
		void startSpeech(); 
		
//		抽签
		void speechDraw();
		
//		比赛函数
		void speechContest(); 
		
		//显示比赛结果
		void showScore();
		
		//保存文件记录
		void saveRecord(); 
		
		//查看记录
		void loadRecord(); 
		
		//显示往届得分
		void showRecord();
		
		//清空记录 
		void clearRecord();
		 
		//退出程序
		void exitSystem();
		
//		析构函数
		~speechManager(); 
		
		
//		成员属性 
//		第一轮比赛    十二个人
		vector<int> v1;
		
//		第二轮比赛六个人
		vector<int> v2;
		
//		赢家三个人 
		vector<int> vVictory; 
		
//		存放编号 以及对应选手  容器 
		map<int,speaker> m_speaker;
		
//		记录比赛轮数 
		int index; 
		
//		文件为空的标志
		bool fileIsEmpty;
		
		//往届记录
		map<int,vector<string> > m_Record; 
		
};
