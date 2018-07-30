#include <iostream>
#include <string.h>
#include <iomanip>
#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <ctype.h>
using namespace std;
const int BoardSize = 8; //何マス平方かの設定
int turn;
int masu[BoardSize][BoardSize];
//2次元配列覚書 [9以上][]は0になるが[負][]は謎数字になる
//C++ANDOR覚書 ABもしくはACを要求するときは(A||B&&C)ではなく((A||B)&&C)が正しい

void outputStoneImage(int pID){
    if(pID == 1) cout << "⚪️";
    else cout << "⚫️";
}
void init(){ //empty:0 black:1 white:2
    turn=BoardSize*BoardSize-4;
    masu[BoardSize][BoardSize]={};
    int half = BoardSize/2;
    masu[half-1][half-1] = 2;
    masu[half-1][half] = 1;
    masu[half][half-1] = 1;
    masu[half][half] = 2;
}
string toOmojiNum(int num){
    if(num==1) return "１";
    else if(num==2) return "２";
    else if(num==3) return "３";
    else if(num==4) return "４";
    else if(num==5) return "５";
    else if(num==6) return "６";
    else if(num==7) return "７";
    else if(num==8) return "８";
    else if(num==9) return "９";
    else return "０";
}
bool stoiCheck(string s){
    try {
        stoi(s);
        return true;
    }
    catch (const std::logic_error& e) {
        return false;
    }
}

class othello{
    protected:
        int v; //縦の入力値
        int h; //横の入力値
        int myPID; //石を置くプレイヤー
        int enemyPID; //の敵プレイヤー
        bool placeable; //石を置けるかどうか(石を返せないと置けない)
    public:
        othello(int pID);
        void console();
        bool validationCheck(int v,int h);
        void ver(int x,int y);
        void hor(int x,int y);
        void sla(int x,int y);
        void display();
        void calc();
};
othello::othello(int pID){
    placeable = false;
    myPID = pID;
    int que = 1;
    //プレイヤーIDを設定し盤面を表示
    if(myPID == 1) enemyPID = 2;
    else enemyPID = 1;
    display();
    cout <<"あなたは";
    outputStoneImage(pID);
    cout << "です どこに置く？ (0,0)を指定すると投了: " << endl;
    //cliからの入力を受け付け、ルール的に石を置けるか判定
    do{
        do console(); while(!validationCheck(v,h));
        cout << "OK?(0) 選び直す?(any)";
        string sque;
        cin >> sque;
        if(!stoiCheck(sque)){
            que = 1;
        }
        else{
            que = stoi(sque);
            if(que!=0){
                que = 1;
            }
        }
        if(que == 0){
            ver(v-1,h-1);
            hor(v-1,h-1);
            sla(v-1,h-1);
            if(placeable == false){
                cout << "アホか。石を返せぬわ。もっかい選べ" <<endl;
                que = 1;
            }
        }
        else if(placeable == false){
            cout << "もっかい選べ" <<endl;
            que = 1;
        }
    } while(que == 1);
}
void othello::console(){ //cliの入力を受け付ける。数字以外の場合は再入力
    string sv,sh;
    cout << "縦の座標を入力:";
    cin >> sv;
    while(!stoiCheck(sv)){
        cout << "もう一度入力:";
        cin >> sv;
    }
    v = stoi(sv);
    cout << "横の座標を入力:";
    cin >> sh;
    while(!stoiCheck(sh)){
        cout << "もう一度入力:";
        cin >> sh;
    }
    h = stoi(sh);
}
bool othello::validationCheck(int v,int h){ //石が置けるかの判定
    if(v == 0 && h == 0){
        string confirm;
        cout << "投了しますか？(press y)" << endl;
        cin >> confirm;
        if(confirm=="y"){
            calc();
            exit(1);
        }
        else return false;
    }
    if(v>BoardSize+1){
        cout << "枠外だよ。";
        return false;
    }
    if(h>BoardSize+1){
        cout << "枠外だよ。";
        return false;
    }
    if(masu[v-1][h-1] != 0){
        cout << "アホか。すでに置かれてる。もっかい選べ" << endl;
        return false;
    }
    return true;
}
void othello::ver(int x,int y){ //縦が揃っているか
    int fp=BoardSize;
    int bp=BoardSize;
    if((x == 0 || x == 1) && masu[x+1][y] == enemyPID){
        for(int i=BoardSize-1;i>=x+2;i--){
            if(masu[i][y] == myPID){bp = i;}
        }
    }
    else if((x == BoardSize-2 || x == BoardSize-1) && masu[x-1][y] == enemyPID){
        for(int i=0;i<=x-2;i++){
            if(masu[i][y] == myPID){fp = i;}
        }
    }
    else{
        if(masu[x-1][y] == enemyPID){
            for(int i=0;i<=x-2;i++){
                if(masu[i][y] == myPID){fp = i;}
            }
        }
        if(masu[x+1][y] == enemyPID){
            for(int i=BoardSize-1;i>=x+2;i--){
                if(masu[i][y] == myPID){bp = i;}
            }
        }
    }
    //fp~x,x~bp間が埋まるかのチェック
    if(fp != BoardSize){
        int fpcheck=0;
        for(int i=fp+1;i<=x-2;i++){
            if(masu[i][y] != enemyPID){fpcheck = 1;} //bug03
        }
        if(fpcheck == 0){
            for(int i=fp+1;i<=x-1;i++){masu[i][y] = myPID;placeable = true;}
        }
    }
    if(bp != BoardSize){
        int bpcheck=0;
        for(int i=x+2;i<=bp-1;i++){
            if(masu[i][y] != enemyPID){bpcheck = 1;}  //bug03
        }
        if(bpcheck == 0){
            for(int i=x+1;i<=bp-1;i++){masu[i][y] = myPID;placeable = true;}
        }
    }
    if(placeable){masu[x][y] = myPID;}
}
void othello::hor(int x,int y){ //横が揃っているか
    int fp=BoardSize;
    int bp=BoardSize;
    if((y == 0 || y == 1) && masu[x][y+1] == enemyPID){
        for(int i=BoardSize-1;i>=y+2;i--){
            if(masu[x][i] == myPID){bp = i;}
        }
    }
    else if((y == BoardSize-2 || y == BoardSize-1) && masu[x][y-1] == enemyPID){
        for(int i=0;i<=y-2;i++){
            if(masu[x][i] == myPID){fp = i;}
        }
    }
    else{
        if(masu[x][y-1] == enemyPID){
            for(int i=0;i<=y-2;i++){
                if(masu[x][i] == myPID){fp = i;}
            }
        }
        if(masu[x][y+1] == enemyPID){
            for(int i=BoardSize-1;i>=y+2;i--){
                if(masu[x][i] == myPID){bp = i;}
            }
        }
    }
    //fp~x,x~bp間が埋まるかのチェック
    if(fp != BoardSize){
        int fpcheck=0;
        for(int i=fp+1;i<=y-2;i++){
            if(masu[x][i] != enemyPID){fpcheck = 1;} //bug03
        }
        if(fpcheck == 0){
            for(int i=fp+1;i<=y-1;i++){masu[x][i] = myPID;placeable = true;}
        }
    }
    if(bp != BoardSize){
        int bpcheck=0;
        for(int i=y+2;i<=bp-1;i++){
            if(masu[x][i] != enemyPID){bpcheck = 1;} //bug03
        }
        if(bpcheck == 0){
            for(int i=y+1;i<=bp-1;i++){masu[x][i] = myPID;placeable = true;}
        }
    }
    if(placeable){masu[x][y] = myPID;}
}
void othello::sla(int x,int y){ //斜めが揃っているか
    int fpr=BoardSize;
    int fpl=BoardSize;
    int bpr=BoardSize;
    int bpl=BoardSize;
    //int max = (14-x-y)/2;
    if(x <= 1 && y <= 1 && masu[x+1][y+1] == enemyPID){
        for(int i=BoardSize-1;i>=2;i--){
            if(masu[x+i][y+i] == myPID){bpl = i;}
        }
    }
    else if(x <= 1 && y >= BoardSize-2 && masu[x+1][y-1] == enemyPID){
        for(int i=BoardSize-1;i>=2;i--){
            if(masu[x+i][y-i] == myPID){bpr = i;}
        }
    }
    else if(x >= BoardSize-2 && y <= 1 && masu[x-1][y+1] == enemyPID){
        for(int i=BoardSize-1;i>=2;i--){
            if(masu[x-i][y+i] == myPID){fpr = i;}
        }
    }
    else if(x >= BoardSize-2 && y >= BoardSize-2 && masu[x-1][y-1] == enemyPID){
        for(int i=BoardSize-1;i>=2;i--){
            if(masu[x-i][y-i] == myPID){fpl = i;}
        }
    }
    else{
        if(masu[x+1][y+1] == enemyPID){
            for(int i=BoardSize-1;i>=2;i--){
                if(masu[x+i][y+i] == myPID){bpl = i;}
            }
        }
        if(masu[x+1][y-1] == enemyPID){
            for(int i=BoardSize-1;i>=2;i--){
                if(masu[x+i][y-i] == myPID){bpr = i;}
            }
        }
        if(masu[x-1][y+1] == enemyPID){
            for(int i=BoardSize-1;i>=2;i--){
                if(masu[x-i][y+i] == myPID){fpr = i;}
            }
        }
        if(masu[x-1][y-1] == enemyPID){
            for(int i=BoardSize-1;i>=2;i--){
                if(masu[x-i][y-i] == myPID){fpl = i;}
            }
        }

    }
    //pointer~x間が埋まるかのチェック
    if(fpr != BoardSize){
        int fprcheck=0;
        for(int i=2;i<=fpr-1;i++){
            if(masu[x-i][y+i] != enemyPID){fprcheck = 1;}
        }
        if(fprcheck == 0){
            for(int i=1;i<=fpr-1;i++){masu[x-i][y+i] = myPID;placeable = true;}
        }
    }
    if(fpl != BoardSize){
        int fplcheck=0;
        for(int i=2;i<=fpl-1;i++){
            if(masu[x-i][y-i] != enemyPID){fplcheck = 1;}
        }
        if(fplcheck == 0){
            for(int i=1;i<=fpl-1;i++){masu[x-i][y-i] = myPID;placeable = true;}
        }
    }
    if(bpr != BoardSize){
        int bprcheck=0;
        for(int i=2;i<=bpr-1;i++){
            if(masu[x+i][y-i] != enemyPID){bprcheck = 1;}
        }
        if(bprcheck == 0){
            for(int i=1;i<=bpr-1;i++){masu[x+i][y-i] = myPID;placeable = true;}
        }
    }
    if(bpl != BoardSize){
        int bplcheck=0;
        for(int i=2;i<=bpl-1;i++){
            if(masu[x+i][y+i] != enemyPID){bplcheck = 1;}
        }
        if(bplcheck == 0){
            for(int i=1;i<=bpl-1;i++){masu[x+i][y+i] = myPID;placeable = true;}
        }
    }
    if(placeable){masu[x][y] = myPID;}
}
void othello::display(){
    //盤面テンプレ
    //cout << "　｜";
    cout << "　";
    for(int i=0;i<=BoardSize-1;i++){cout << toOmojiNum(i+1);}
    //３行目以降
    //cout << "｜" << endl;
    //cout << "　｜";
    //for(int i=0;i<=7;i++){cout << "ー";}
    //cout << "｜" << endl;
    cout << endl;

    for(int i=0;i<=BoardSize-1;i++){
        cout << toOmojiNum(i+1);
        //cout << "｜";
        for(int j=0;j<=BoardSize-1;j++){
            if(masu[i][j] == 0){cout << "・";}
            if(masu[i][j] == 1){cout << "⚪️";}
            if(masu[i][j] == 2){cout << "⚫️";}
        }
        //cout << "｜" << endl;
        cout << toOmojiNum(i+1) << endl;

    }
    //盤面テンプレ
    //cout << "　｜";
    cout << "　";
    for(int i=0;i<=BoardSize-1;i++){cout << toOmojiNum(i+1);}
    //cout <<  "｜" << endl;
    cout << endl;
}
void othello::calc(){ //石数と勝敗を表示
    int blk = 0;
    int whi = 0;
    for(int i=0;i<BoardSize;i++){
        for(int j=0;j<BoardSize;j++){
            if(masu[i][j] == 1){blk++;}
            else if(masu[i][j] == 2){whi++;}
        }
    }
    display();
    cout << endl;
    cout << "⚫️が" << whi << "個 ⚪️が" << blk << "個 よって";
    if(blk > whi){ cout << "勝者:⚪️"<< endl;}
    else if(blk < whi){ cout << "勝者:⚫️"<< endl;}
    else{ cout << "引き分け" << endl;}
}
int main(void){
    init();
    for(int i=1;i<=turn;i++){
        cout << i << "手目:" << endl;
        othello play((i%2)+1);
        if(i == turn){play.calc();return 0;}
    }
    return 0;
}
