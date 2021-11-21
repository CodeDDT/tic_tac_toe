#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<random>
#include<chrono>
#include<stdlib.h>

using namespace std;

const int tNo=9;//tableNumber
const char X='X';
const char O='O';
const int winRows[8][3]={//胜利条件
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8},
    {0,4,8},
    {2,4,6}
};

void run(vector<char>& table,char player,char com);

void showTable(const vector<char>& sTable);
int YorN();
void firstMove(char& p,char& c);
int pTurn(vector<char>& T,const char& p);
int cTurn(vector<char>& T,const char& c);
void told(const vector<char>& T);
int pCheck(vector<char>& t,int place);
int wCheck(const vector<char>& t,const char& m);
void replace(vector<char>& table);

int main(){
    vector<char> table({'0','1','2','3','4','5','6','7','8'});
    char player;
    char com;

    int option;

    do{
        cout<<"*******************"<<endl;
        cout<<"*   tic-tac-toe   *"<<endl;
        cout<<"*******************"<<endl;
        cout<<"1.Start the game"<<endl;
        cout<<"0.quit"<<endl;
        cout<<"What do you want to do?"<<endl;
        cin>>option;
        switch (option)
        {
        case 1:
            run(table,player,com);
            system("pause");
            system("cls");
            break;
        case 0:
            return 0;
        default:
            cout<<"Wrong data."<<endl;
            system("pause");
            system("cls");
        }
    }while(option!=0);



}

/*........................................................................
.                        以下为程序运行逻辑函数                            .
........................................................................*/

void run(vector<char>& table,char player,char com){
    firstMove(player,com);
    if(player=='O'){
        showTable(table);
    }
    for(int turn=0;turn<4;turn++){
        if(player=='O'){
            if(pTurn(table,player)==1){
                cout<<"You win."<<endl;
                break;
            }
            if(cTurn(table,com)==1){
                cout<<"Com wins."<<endl;
                break;
            }
        }else{
            if(cTurn(table,com)==1){
                cout<<"Com wins."<<endl;
                break;
            }
            if(pTurn(table,player)==1){
                cout<<"You win."<<endl;
                break;
            }
        }
    }
    if(wCheck(table,player)==0&&wCheck(table,com)==0){
        if(player=='O'){
            if(pTurn(table,player)==1){
                cout<<"Final:"<<endl;
                showTable(table);
                cout<<"You win."<<endl;
            }
        }else{
            if(cTurn(table,com)==1){
                cout<<"Com win."<<endl;
            }
        }
    }
    if(wCheck(table,player)==0&&wCheck(table,com)==0){
        cout<<"Final:"<<endl;
        showTable(table);
        cout<<"Draw."<<endl;
    }
    replace(table);
}


/*........................................................................
.                        以下为游戏逻辑函数                                .
........................................................................*/


//显示棋盘
void showTable(const vector<char>& sTable){
    cout<<sTable[0]<<" | "<<sTable[1]<<" | "<<sTable[2]<<endl;
    cout<<"---------"<<endl;
    cout<<sTable[3]<<" | "<<sTable[4]<<" | "<<sTable[5]<<endl;
    cout<<"---------"<<endl;
    cout<<sTable[6]<<" | "<<sTable[7]<<" | "<<sTable[8]<<endl;
    cout<<"---------"<<endl;
}

//是否选择
int YorN(){
    char option;
    do{
        cin>>option;
        if(option=='Y'||option=='y'){
            return 1;
        }else if(option=='N'||option=='n'){
            return 0;
        }else{
            cout<<"Wrong answer.Please input y/n."<<endl;
        }
    }while(option!='Y'&&option!='y'&&option!='n'&&option!='N');
}

//先手权
void firstMove(char& p,char& c){
    cout<<"Wanna move first?"<<endl;
    if(YorN()==1){
        cout<<"You got the first move."<<endl;
        p=O;
        c=X;
    }else{
        cout<<"Com got the first move."<<endl;
        c=O;
        p=X;
    }
}

//玩家操作回合
int pTurn(vector<char>& T,const char& p){
    int place;
    int cPoint;
    cout<<"Please input the location num you want to place the chess."<<endl;
    do{
        cin>>place;
        if(place>8||place<0){
            cout<<"Wrong data"<<endl;
        }else{
            cPoint=pCheck(T,place);
            if(cPoint==0){
                cout<<"A chess already existed."<<endl;
            }
        }
    }while((place>8&&place<0)||cPoint==0);
    T[place]=p;
    if(wCheck(T,p)==1){
        return 1;
    }else{
        return 0;
    }
}

//电脑操作回合
int cTurn(vector<char>& T,const char& c){
    //第一步，判断是否有能力通过下一步棋获胜
    for(int move=0;move<tNo;move++){
        if(pCheck(T,move)==1){
            T[move]=c;
            if(wCheck(T,c)==1){
                told(T);
                return 1;
            }else{
                T[move]=move+48;//撤销操作
            }

        }
    }
    //第二步，阻止对手获胜
    char opponente;
    //生成随机数列表
    vector<int> list={0,1,2,3,4,5,6,7,8};
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    shuffle(list.begin(),list.end(),default_random_engine(seed));
    //判断对手先后手
    if(c=='O'){
        opponente='X';
    }else{
        opponente='O';
    }
    for(int move=0;move<tNo;move++){
        if(pCheck(T,list[move])==1){
            T[list[move]]=opponente;
            if(wCheck(T,opponente)==1){
                T[list[move]]=c;
                told(T);
                return 0;
            }else{
                T[list[move]]=list[move]+48;
            }
        }
    }
    //最后，占据优势领地
    //首先抢占中间
    if(pCheck(T,4)==1){
        T[4]=c;
        told(T);
        return 0;
    }
    //其次四角
    vector<int> list_c={0,2,6,8};
    seed=chrono::system_clock::now().time_since_epoch().count();
    shuffle(list_c.begin(),list_c.end(),default_random_engine(seed));
    for(int i=0;i<4;i++){
        if(pCheck(T,list_c[i])==1){
            T[list_c[i]]=c;
            told(T);
            return 0;
        }
    }
    //最后四边
    vector<int> list_s={1,3,5,7};
    seed=chrono::system_clock::now().time_since_epoch().count();
    shuffle(list_s.begin(),list_s.end(),default_random_engine(seed));
    for(int i=0;i<4;i++){
        if(pCheck(T,list_s[i])==1){
            T[list_s[i]]=c;
            told(T);
            return 0;
        }
    }
}

//电脑声明
void told(const vector<char>& T){
    cout<<"Computer has placed the chess."<<endl;
    showTable(T);
}

//检查棋子(place check)
int pCheck(vector<char>& t,int place){
    if(t[place]==O||t[place]==X){//已有棋子
        return 0;
    }else{//没有
        return 1;
    }
}

//胜利鉴定(winning check)
int wCheck(const vector<char>& t,const char& m){
    for(int i=0;i<8;i++){
        if(t[winRows[i][0]]==m&&t[winRows[i][1]]==m&&t[winRows[i][2]]==m){
            return 1;
        }
    }
    return 0;
}

//重置棋盘
void replace(vector<char>& table){
    for(int i=0;i<tNo;i++){
        table[i]=i+48;
    }
}