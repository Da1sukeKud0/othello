#include <iostream>
#include <string.h>
#include <iomanip>
#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <ctype.h>
using namespace std;
const int size = 8;//何マス平方かの設定
int turn;
int masu[size][size];
//2次元配列覚書 [9以上][]は0になるが[負][]は謎数字
//C++ANDOR覚書 ABもしくはACを要求するときは(A||B&&C)ではなく((A||B)&&C)が正しい

void init(){ //empty:0 black:1 white:2
	turn=size*size-4;
	masu[size][size]={};
	int half = size/2;
	masu[half-1][half-1] = 2;
	masu[half-1][half] = 1;
	masu[half][half-1] = 1;
	masu[half][half] = 2;
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
	string input;
	int v;
	int h;
	int que;
	int status; //石を置くプレイヤー
	int enst; //の敵プレイヤー
	bool puten; //石を置けるかどうか(石を返せないと置けない)
public:
	othello(int p);
	bool toryo;
	void console();
	bool validationCheck(int v,int h);
	void ver(int x,int y);
	void hor(int x,int y);
	void sla(int x,int y);
	void display();
	void calc();
};
othello::othello(int p){
	puten = false;
	toryo = false;
	status = p;
	que = 1;
	//プレイヤーIDを設定し盤面を表示
	if(status == 1) enst = 2;
	else enst = 1;
	display();
	cout <<"あなたは";
	if(p == 1){cout << "○ ";}
	else{cout << "● ";}
	cout << "です どこに置く？" << endl;
	//cliからの入力を受け付け、ルール的に石を置けるか判定
	do{
		do console(); while(!validationCheck(v,h));
		cout << "OK?(0) 選び直す?(1) 投了?(88):";
		string sque;
		cin >> sque;
		while(!stoiCheck(sque)){
				cout << "もう一度入力:";
				cin >> sque;
			}
			que = stoi(sque);
		if(que == 88){
			calc();
			toryo = true;
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
bool othello::validationCheck(int v,int h){
	//石が置けるかの判定
	if(v>size+1){
		cout << "枠外だよ。";
		return false;
	}
	if(h>size+1){
		cout << "枠外だよ。";
		return false;
	}
	if(masu[v-1][h-1] != 0){
		cout << "アホか。すでに置かれてる。もっかい選べ" << endl;
		return false; //que=1
	}
	//石が返せるかの判定
	ver(v-1,h-1);
	hor(v-1,h-1);
	sla(v-1,h-1);
	if(puten == false){
		cout << "アホか。石を返せぬわ。もっかい選べ" <<endl;
		que = 1;
		return false;
	}
	return true;
}
void othello::ver(int x,int y){ //縦が揃っているか
	int fp=size;
	int bp=size;
	if((x == 0 || x == 1) && masu[x+1][y] == enst){
		for(int i=size-1;i>=x+2;i--){
			if(masu[i][y] == status){bp = i;}
		}
	}
	else if((x == size-2 || x == size-1) && masu[x-1][y] == enst){
		for(int i=0;i<=x-2;i++){
			if(masu[i][y] == status){fp = i;}
		}
	}
	else{
		if(masu[x-1][y] == enst){
			for(int i=0;i<=x-2;i++){
				if(masu[i][y] == status){fp = i;}
			}
		}
		if(masu[x+1][y] == enst){
			for(int i=size-1;i>=x+2;i--){
				if(masu[i][y] == status){bp = i;}
			}
		}
	}
	//fp~x,x~bp間が埋まるかのチェック
	if(fp != size){
		int fpcheck=0;
		for(int i=fp+1;i<=x-2;i++){
			if(masu[i][y] != enst){fpcheck = 1;} //bug03
		}
		if(fpcheck == 0){
			for(int i=fp+1;i<=x-1;i++){masu[i][y] = status;puten = true;}
		}
	}
	if(bp != size){
		int bpcheck=0;
		for(int i=x+2;i<=bp-1;i++){
			if(masu[i][y] != enst){bpcheck = 1;}  //bug03
		}
		if(bpcheck == 0){
			for(int i=x+1;i<=bp-1;i++){masu[i][y] = status;puten = true;}
		}
	}
	if(puten){masu[x][y] = status;}
}
void othello::hor(int x,int y){ //横が揃っているか
	int fp=size;
	int bp=size;
	if((y == 0 || y == 1) && masu[x][y+1] == enst){
		for(int i=size-1;i>=y+2;i--){
			if(masu[x][i] == status){bp = i;}
		}
	}
	else if((y == size-2 || y == size-1) && masu[x][y-1] == enst){
		for(int i=0;i<=y-2;i++){
			if(masu[x][i] == status){fp = i;}
		}
	}
	else{
		if(masu[x][y-1] == enst){
			for(int i=0;i<=y-2;i++){
				if(masu[x][i] == status){fp = i;}
			}
		}
		if(masu[x][y+1] == enst){
			for(int i=size-1;i>=y+2;i--){
				if(masu[x][i] == status){bp = i;}
			}
		}
	}
	//fp~x,x~bp間が埋まるかのチェック
	if(fp != size){
		int fpcheck=0;
		for(int i=fp+1;i<=y-2;i++){
			if(masu[x][i] != enst){fpcheck = 1;} //bug03
		}
		if(fpcheck == 0){
			for(int i=fp+1;i<=y-1;i++){masu[x][i] = status;puten = true;}
		}
	}
	if(bp != size){
		int bpcheck=0;
		for(int i=y+2;i<=bp-1;i++){
			if(masu[x][i] != enst){bpcheck = 1;} //bug03
		}
		if(bpcheck == 0){
			for(int i=y+1;i<=bp-1;i++){masu[x][i] = status;puten = true;}
		}
	}
	if(puten){masu[x][y] = status;}
}
void othello::sla(int x,int y){ //斜めが揃っているか
	int fpr=size;
	int fpl=size;
	int bpr=size;
	int bpl=size;
	//int max = (14-x-y)/2;
	if(x <= 1 && y <= 1 && masu[x+1][y+1] == enst){
		for(int i=size-1;i>=2;i--){
			if(masu[x+i][y+i] == status){bpl = i;}
		}
	}
	else if(x <= 1 && y >= size-2 && masu[x+1][y-1] == enst){
		for(int i=size-1;i>=2;i--){
			if(masu[x+i][y-i] == status){bpr = i;}
		}
	}
	else if(x >= size-2 && y <= 1 && masu[x-1][y+1] == enst){
		for(int i=size-1;i>=2;i--){
			if(masu[x-i][y+i] == status){fpr = i;}
		}
	}
	else if(x >= size-2 && y >= size-2 && masu[x-1][y-1] == enst){
		for(int i=size-1;i>=2;i--){
			if(masu[x-i][y-i] == status){fpl = i;}
		}
	}
	else{
		if(masu[x+1][y+1] == enst){
			for(int i=size-1;i>=2;i--){
				if(masu[x+i][y+i] == status){bpl = i;}
			}
		}
		if(masu[x+1][y-1] == enst){
			for(int i=size-1;i>=2;i--){
				if(masu[x+i][y-i] == status){bpr = i;}
			}
		}
		if(masu[x-1][y+1] == enst){
			for(int i=size-1;i>=2;i--){
				if(masu[x-i][y+i] == status){fpr = i;}
			}
		}
		if(masu[x-1][y-1] == enst){
			for(int i=size-1;i>=2;i--){
				if(masu[x-i][y-i] == status){fpl = i;}
			}
		}

	}
	//pointer~x間が埋まるかのチェック
	if(fpr != size){
		int fprcheck=0;
		for(int i=2;i<=fpr-1;i++){
			if(masu[x-i][y+i] != enst){fprcheck = 1;}
		}
		if(fprcheck == 0){
			for(int i=1;i<=fpr-1;i++){masu[x-i][y+i] = status;puten = true;}
		}
	}
	if(fpl != size){
		int fplcheck=0;
		for(int i=2;i<=fpl-1;i++){
			if(masu[x-i][y-i] != enst){fplcheck = 1;}
		}
		if(fplcheck == 0){
			for(int i=1;i<=fpl-1;i++){masu[x-i][y-i] = status;puten = true;}
		}
	}
	if(bpr != size){
		int bprcheck=0;
		for(int i=2;i<=bpr-1;i++){
			if(masu[x+i][y-i] != enst){bprcheck = 1;}
		}
		if(bprcheck == 0){
			for(int i=1;i<=bpr-1;i++){masu[x+i][y-i] = status;puten = true;}
		}
	}
	if(bpl != size){
		int bplcheck=0;
		for(int i=2;i<=bpl-1;i++){
			if(masu[x+i][y+i] != enst){bplcheck = 1;}
		}
		if(bplcheck == 0){
			for(int i=1;i<=bpl-1;i++){masu[x+i][y+i] = status;puten = true;}
		}
	}
	if(puten){masu[x][y] = status;}
}
void othello::display(){
	//盤面テンプレ
	cout << "　｜";
	for(int i=0;i<=size-1;i++){cout << setw(2) << i+1;}
	//３行目以降
	cout << "｜" << endl;
	//cout << "　｜";
	//for(int i=0;i<=7;i++){cout << "ー";}
	//cout << "｜" << endl;

	for(int i=0;i<=size-1;i++){
		cout << setw(2) << i+1;
		cout << "｜";
		for(int j=0;j<=size-1;j++){
			if(masu[i][j] == 0){cout << "　";}
			if(masu[i][j] == 1){cout << "○";}
			if(masu[i][j] == 2){cout << "●";}
		}
		cout << "｜" << endl;

	}
	//盤面テンプレ
	cout << "　｜";
	for(int i=0;i<=size-1;i++){cout << setw(2) << i+1;}
	cout <<  "｜" << endl;

}
void othello::calc(){ //石数と勝敗を表示
	int blk = 0;
	int whi = 0;
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if(masu[i][j] == 1){blk++;}
			else if(masu[i][j] == 2){whi++;}
		}
	}
	display();
	cout << endl;
	cout << "● が" << whi << "個 ○ が" << blk << "個 よって";
	if(blk > whi){ cout << "勝者:○ "<< endl;}
	else if(blk < whi){ cout << "勝者:● "<< endl;}
	else{ cout << "引き分け" << endl;}
}
int main(void){
	init();
	for(int i=1;i<=turn;i++){
		cout << i << "手目:" << endl;
		othello play((i%2)+1);
		if(i == turn){play.calc();}
		if(play.toryo){return 0;}
	}
	return 0;
}
