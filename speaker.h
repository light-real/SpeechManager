#pragma once   //保证头文件只包含一次 
#include<iostream>
using namespace std;

//比赛流程
//抽签->开始演讲比赛->显示第一轮比赛结果->抽签->开始演讲比赛->显示前三名成绩->保存结果 

class speaker{
	public:
		string name;   //选手姓名
		double score[2];  //选手成绩  总共两轮比赛 
};

