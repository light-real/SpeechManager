#include "speechManager.h"

speechManager::speechManager(){
	
	
	this->initSpeech();   //在构造函数中调用初始化属性 
	
	this->createSpeaker(); //创建选手 
	
	this->loadRecord();
}


//菜单功能 
void speechManager::showMenu(){ 
	cout<<"---------------------------------"<<endl;
	cout<<"---------1.开始演讲比赛----------"<<endl;
	cout<<"---------2.查看往届记录----------"<<endl;
	cout<<"---------3.清空比赛记录----------"<<endl;
	cout<<"---------0.退出比赛程序----------"<<endl;
	cout<<"---------------------------------"<<endl;
}

//初始化属性
void speechManager::initSpeech(){
//	将三轮比赛中存放选手的容器清空 
	this->v1.clear();
	this->v2.clear();
	this->vVictory.clear();
	
	this->m_speaker.clear(); //存放选手编号 姓名的容器清空
	
	//初始化比赛论数 
	this->index = 1;   
	
	//初始化记录容器 
	this->m_Record.clear(); 
	
} 

//创建选手
void speechManager::createSpeaker(){
	string nameSeed = "ABCDEFGHIJKL";
	for(int i =0;i<12;i++){
		string name = "选手";
		name += nameSeed[i];
		
		speaker sp;
		sp.name = name;   //选手名字   "选手A"样式 
		
		for(int j = 0;j<2;j++){
			sp.score[j] = 0;   //将所有选手的初始成绩置0   ！！！别犯傻啊，这里是j，你写的i直接崩啊 
		}
		
		//将选手的编号放入第一轮比赛的容器中
		this->v1.push_back(i+10001);
		
//		将选手编号 姓名进行配对   key-value 形式 
		this->m_speaker.insert(make_pair(i+10001,sp));  
	}
} 


//开始比赛 比赛流程控制函数 
void speechManager::startSpeech(){
	
//	第一轮比赛 
//	1、抽签
	this->speechDraw();
//	2、比赛
	this->speechContest();
//	3、显示比赛结果 
	this->showScore();
//	第二轮比赛
	this->index++;
//	1、抽签
	this->speechDraw();
//	2、比赛
	this->speechContest();
//	3、显示最终比赛结果 
	this->showScore();
//	4、保存最终分数到文件中 
	this->saveRecord();
	
	
	//重置比赛，获取记录 
	this->initSpeech();   //在构造函数中调用初始化属性 
	
	this->createSpeaker(); //创建选手 
	
	this->loadRecord();
}

void speechManager::speechDraw(){
	cout<<"第"<<this->index<<"轮比赛正在进行"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<"抽签顺序如下："<<endl;
	
	if(this->index == 1){  //如果是第一轮比赛 
		random_shuffle(v1.begin(),v1.end());  //将v1容器中的元素进行打乱
		for(vector<int>::iterator it = v1.begin();it != v1.end();it++){
			cout<<*it<<" ";
		} 
		cout<<endl;
	}
	else{
		random_shuffle(v2.begin(),v2.end());
		for(vector<int>::iterator it = v2.begin();it != v2.end();it++){
			cout<<*it<<" "; 
		}
		cout<<endl;
	}
	cout<<"------------------------------"<<endl;
	system("pause");
}
 
 
//开始比赛
void speechManager::speechContest(){
	cout<<"第"<<this->index<<"轮比赛正在进行中"<<endl;
	
	//map multimap容器，都是按照key值进行从大到小进行排列的，所以multimap 用其分数作为key 编号作为value 
	multimap<double,int,greater<double>	> groupScore;  //临时容器，用于存储 （选手分数，编号） （key,value） 
	
	//用于记录比赛分组   12->6：每六个人一组，取其中前三名进入比赛   
	int num = 0 ;   
	
	vector<int> v_Src;  //比赛人员的容器
	if(this->index == 1){ 
		v_Src = v1;   //如果是第一轮比赛，正在比赛的人员是v1中的人员 
	}
	else{
		v_Src = v2;  //第二轮比赛，正在比赛中的人员是v2中的人员 
	}
	//遍历所有参赛选手
	for(vector<int>::iterator it = v_Src.begin();it != v_Src.end();it++){
		num++;  //记录已经操作的参赛人员
		
		//评委打分
		deque<double> d; //deque 双端队列  能在队首和队尾对元素进行操作  
		for(int i = 0;i<10;i++){
			//用rand()函数对分数进行生成，rand()如果不指定范围，生成的是0-rand_max之间的数，rand_max至少为32767 
			double score = (rand() % 401 + 600) / 10.f;   //rand() % 401 生成一个0-400的数，然后+600 生成得到一个600-1000的数   /10.f 含有小数
			d.push_back(score);  //将打分得到的成绩放到队列中 
		} 
		
//		对队列中的成绩进行排序 需要去除每个人成绩的最高分和最低分
		sort(d.begin(),d.end(),greater<double>());  
		d.pop_front();  //去除最高分
		d.pop_back();  //去除最低分 
		
		double sum = accumulate(d.begin(),d.end(),0.0f); //获取该选手的总分
		double avg = sum /(double)d.size();  //获取选手平均分
		
		//每个选手的成绩需要放到创建的speaker类中
		this->m_speaker[*it].score[this->index-1] = avg;  
	
		//6个人存一组，用临时容器保存
		groupScore.insert(make_pair(avg,*it));   //将该选手的平均分和编号存放在临时容器
	
		if(num % 6 == 0){  //对6进行去模运算，为0 表示已经够6个人 
			cout<<"第"<<num / 6 <<"组的比赛名次是："<<endl;
			
			//从临时容器中将数据拿出来  map 会根据key 对元素进行排序 所以现在已经是从大到小的顺序
			for(multimap<double,int,greater<double> >::iterator it = groupScore.begin();it != groupScore.end();it++){
				cout<<"编号:"<<it->second<<" "
					<<"姓名:"<<this->m_speaker[it->second].name<<" "
					<<"成绩:"<<this->m_speaker[it->second].score[this->index-1]<<endl; 
			}
			 	 
			int count = 0;
			
			//取前三名
			for(multimap<double,int,greater<double> >::iterator it = groupScore.begin() ;it != groupScore.end() && count < 3;it++,count++){ //count也要++啊！！！ 
				
				if(this->index == 1 ){  //如果是第一轮比赛,将前三名的数据放到v2中 
					v2.push_back((*it).second);
				}
				else  {  //如果是第二轮比赛，那么将前三名放到vVictory中 
					vVictory.push_back((*it).second);
				}
			} 
			groupScore.clear();  //一个选手已经操作完，清空临时容器
			cout<<endl; 
		}  
	}
	cout<<"------------第"<<this->index<<"轮的比赛结束---------------"<<endl;
	system("pause"); 
} 


//显示比赛结果
void speechManager::showScore(){
	cout<<"------------第"<<this->index<<"轮比赛晋级选手：-------------"<<endl;
	
	//创建一个vector 用于显示晋级的人员
	vector<int> v;
	if(this->index == 1){
		v = v2;   //如果是第一轮比赛，晋级的人员已经放到了v2中 
	} 
	else{
		v = vVictory; //同理 
	}
	
	for(vector<int>::iterator it = v.begin();it!=v.end();it++){
		cout<<"编号:"<<*it<<" "
			<<"姓名:"<<this->m_speaker[*it].name<<" "
			<<"成绩:"<<this->m_speaker[*it].score[this->index - 1]<<endl;
		 
	}
	cout<<endl;
	system("pause");
	system("cls");
	this->showMenu(); 
} 

//保存比赛记录
void speechManager::saveRecord(){
	ofstream ofs;
	ofs.open("speech.csv",ios::out | ios::app);   //用输出的方式打开文件 并且是追加方式
	
	//将vVictory中每个人的数据写到文件中
	for(vector<int>::iterator it = this->vVictory.begin();it != this->vVictory.end();it++){
		ofs << *it << "," << this->m_speaker[*it].score[1]<<",";
	} 
	ofs<<endl;
	ofs.close();
	cout<<"记录已经保存"<<endl;
	this->fileIsEmpty = false;
}

//查看比赛记录
void speechManager::loadRecord(){
	ifstream ifs("speech.csv",ios::in); // 输入流对象，读文件
	if(!ifs.is_open()){   //打开失败 
		this->fileIsEmpty = true;
		cout<<"文件不存在！"<<endl;
		ifs.close(); 
	} 
	char ch;
	ifs >> ch;  //读走一个字符，如果读走一个字符以后，接下来的那个字符是文件尾，那么表示文件为空 
	if(ifs.eof()){  
		cout<<"文件为空！"<<endl;
		this->fileIsEmpty = true;
		ifs.clear();
	}
	
	//文件不为空的情况下：
	this->fileIsEmpty = false;
	
	ifs.putback(ch);  //将上面读走的字符放回去
	
	string data;
	int index = 0;
	while(ifs >> data){
		vector<string> v;
		
		int pos = -1;
		int start = 0;
		
		while(true){
			pos = data.find(",",start); //从0开始查找','  拿到','的下标 
			if(pos == -1 ){
				break;  //找不到break返回 
			} 
			
			string temp = data.substr(start,pos-start); //截取子串，开始位置为start 截止位置为找到的','坐标 - start
			v.push_back(temp);  //将子串以字符串的形式放到vector中
			start = pos + 1;  // 下一个子串的开始位置是pos的位置加一 
		}
		this->m_Record.insert(make_pair(index,v));   // index表示第几届  key    v 为保存的数据  value 
		index++;
	} 
	ifs.close();
//	for(map<int,vector<string> >::iterator it = this->m_Record.begin();it != this->m_Record.end();it++){
//		cout<<"第"<<it->first + 1<<"届的比赛情况如下:"<<endl;
//		cout<<"冠军编号："<<it->second[0]<<" "
//			<<"分数："<<it->second[1]<<endl; 
//	}
} 


//显示比赛记录 
void speechManager::showRecord(){
	if(this->fileIsEmpty){
		cout<<"文件不存在，或记录为空！"<<endl; 
	}
	else{
		for(int i = 0;i < this->m_Record.size();i++){
			cout<<"第"<< i + 1 <<"届的比赛信息："<<endl;
			cout<<"冠军编号："<<this->m_Record[i][0]<<" "<<"冠军分数"<<this->m_Record[i][1]<<"  " 
				<<"亚军编号："<<this->m_Record[i][2]<<" "<<"亚军分数"<<this->m_Record[i][3]<<"  "
				<<"季军编号："<<this->m_Record[i][4]<<" "<<"季军分数"<<this->m_Record[i][5]<<"  "<<endl; 
	}		
	}

	
	system("pause");
	system("cls");
}

//清空比赛记录
void speechManager::clearRecord(){
	cout<<"是否确认清空？"<<endl;
	cout<<"1、确认"<<endl;
	cout<<"2、返回"<<endl;
	
	int select;
	cin>>select;
	
	if(select == 1){
		//打开模式，ios::trunc 如果文件存在，那么删除文件并且重新创建
		ofstream ofs("speech.csv",ios::trunc);
		ofs.clear();
		
		//初始化属性
		this->initSpeech();
		
		//创建选手
		this->createSpeaker();
		
		this->loadRecord();
		
		this->fileIsEmpty = true;
		
		cout<<"清空成功"<<endl; 
	}
	
	system("pause");
	system("cls");
} 

//退出程序 
void speechManager::exitSystem(){
			cout<<"欢迎下次使用"<<endl;
			system("pause");
			exit(0);
}

speechManager::~speechManager(){
	
}
