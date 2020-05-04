#include<bangtal.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>


#define PI 3.1415926535


SceneID scene1, shop,title, introduction,scoreboard;
ObjectID angleGauge, powerGauge, Canon, background, background1, angpointer, powpointer, angstopbutton, powstopbutton, startbutton, shopbutton, backbutton, titlestartbutton,restartbutton;
ObjectID angaccuracyicon, powaccuracyicon,maxpowericon, baseballicon, basketballicon;
ObjectID cannon,base;
ObjectID Ball[16];


SoundID click = createSound("sound/click.wav");
SoundID fire = createSound("sound/fire.wav");
SoundID thud = createSound("sound/thud.wav");

double score;


// 각도 애니메이션 변수
//시작
double angx = 0, angX = 0, anganispeed = 40;//각도 게이지 속도
int AngAnidiraction = 1;//AngAnidiraction가 1일때 오른쪽, 0일때 왼쪽 방향으로 이동중
int angAniworking = 0; //ang 애니메이션이 작동하는 지 0 false 1 true
//끝

// 파워 애니메이션 변수
//시작
double powx = 0, powX = 0, powanispeed =1000;//파워 게이지 속도
int powAniworking = 0; //pow 애니메이션이 작동하는 지 0 false 1 true
//끝


// 발사 애니메이션 변수
//시작
double firingangle, firingpower,defaultpower=100;// 기본 파워
double ballmovement=0;// 화면 움직이기 전 공만 움직일때 움직인 거리 측정
double Ballx = 0, Bally = 0;
double BallX = 0, BallY = 0,BallX1=0;
double BallXr = 0, BallX1r = 0;
int balllaunched = 0;// 공 혼자 날라가는 애니메이션을 실행 했는지 아직 실행안함=0 실행함=1 
int firingrotate = 0;
int rotationcache = 0;
double backgroundsliding = 0, backgroundsliding1=0;
//끝

// 상점 변수
//시작
int money = 40000; 
int angaccuracylev = 1, powaccuracylev = 1, maxpowerlev = 1;
//끝


TimerID timer1, timer2, angAni, powAni;




ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return(object);
}



double Radian(double _num) {
	return _num * (PI/ 180);
}

void angleanimaion() {	//angX 는 실제 x 좌표 angx 는 실제를 구하기위한 변수
	if (angX>0 && angX < 160 && AngAnidiraction==1) {
		angx = angx + anganispeed;		
		angX = 31 * log(angx + 1);
	}
	else if (angX > -160 && angX < 0 && AngAnidiraction == 1) {
		angx = angx + anganispeed;		
		angX = -31 * log(-angx + 1);
	}
	
	else if(angX > 0 && angX < 160 && AngAnidiraction == 0) {
		angx = angx - anganispeed;		
		angX = 31 * log(angx + 1);
	}
	else if (angX > -160 && angX < 0 && AngAnidiraction == 0) {
		angx = angx - anganispeed;
		angX = -31 * log(-angx + 1);
	}
	//----------
	else if (angX >= 160 && AngAnidiraction == 1) {
		angx = angx - anganispeed;
		angX = 31 * log(angx + 1);
		AngAnidiraction = 0;
	}
	else if (angX <= -160 && AngAnidiraction == 0) {
		angx = angx + anganispeed;		
		angX = -31*log(-angx+1);
		//angX = 0.0003 * angx * angx * angx;
		//angX = -31*log(-angx+1);
		AngAnidiraction = 1;
	}
	//-----------
	else if (angX == 0 && AngAnidiraction == 0) {
		angx = angx - anganispeed;
		angX = -31 * log(-angx + 1);
	}
	else if (angX == 0 && AngAnidiraction == 1) {
		angx = angx + anganispeed;
		angX = 31 * log(angx + 1);
	}
	locateObject(angpointer, scene1, angX + 190, 40);
	stopTimer(angAni);
	startTimer(angAni);
	setTimer(angAni, 0.01f);
}

void poweranimaion() {//powX 는 실제 x 좌표 powx 는 실제를 구하기위한 변수
	if (powX >= 0 && powX < 450) {
		powx = powx + powanispeed;
		powX = 0.000005 * (powx) *  (powx) ; // ---------------------------------------------------------------------나중에 함수 어떻게 세팅할 것인지 고민
	}
	else if (powX >= 449 ) {
		powx = 0;
		powX = 0;
	}

	locateObject(powpointer, scene1, powX + 770, 40);
	stopTimer(powAni);
	startTimer(powAni);
	setTimer(powAni, 0.01f);
}

void angAnimationstop() {
	
	if (angAniworking != 0) {
		hideObject(restartbutton);
		hideObject(angstopbutton);
		double num;
		stopTimer(angAni);
		angAniworking = 0;				
		num = angX + 160;
		num = num / 320;
		firingangle = num * 50 + 20;		
	}
	if (firingangle <= 35) {
		setObjectImage(cannon, "image/cannon20.png");
	}
	else if (firingangle > 35 && firingangle<=50) {
		setObjectImage(cannon, "image/cannon40.png");
	}
	else if (firingangle > 50 ) {
		setObjectImage(cannon, "image/cannon60.png");
	}
	firingangle = Radian(firingangle);
}
void powAnimationstop() {
	if (powAniworking != 0) {
		hideObject(restartbutton);
		hideObject(powstopbutton);
		double firingpowerscale;
		stopTimer(powAni);
		powAniworking = 0;
		firingpowerscale = powX / 400;
		firingpower = firingpowerscale * defaultpower+50;
		
	}
}

void firingrotaition() {
	if (rotationcache%12 == 0) {
		setObjectImage(Ball[1], "image/baseball1.png");
	}
	else if (rotationcache%12 == 1) {
		setObjectImage(Ball[1], "image/baseball2.png");
	}
	else if (rotationcache % 12 == 2) {
		setObjectImage(Ball[1], "image/baseball3.png");
	}
	else if (rotationcache % 12 == 3) {
		setObjectImage(Ball[1], "image/baseball4.png");
	}
	else if (rotationcache % 12 == 4) {
		setObjectImage(Ball[1], "image/baseball5.png");
	}
	else if (rotationcache % 12 == 5) {
		setObjectImage(Ball[1], "image/baseball6.png");
	}
	else if (rotationcache % 12 == 6) {
		setObjectImage(Ball[1], "image/baseball7.png");
	}
	else if (rotationcache % 12 == 7) {
		setObjectImage(Ball[1], "image/baseball8.png");
	}
	else if (rotationcache % 12 == 8) {
		setObjectImage(Ball[1], "image/baseball9.png");
	}
	else if (rotationcache % 12 == 9) {
		setObjectImage(Ball[1], "image/baseball10.png");
	}
	else if (rotationcache % 12 == 10) {
		setObjectImage(Ball[1], "image/baseball11.png");
	}
	else if (rotationcache % 12 == 11) {
		setObjectImage(Ball[1], "image/baseball12.png");
	}
	
}


void firinganimation() {//Ballx 는공만 날아가는거 BallX는 화면이 날아가는거
	//firingangle = 0.52;
	//firingangle = 0.78;
	//firingangle = 1.22;
	//firingpower = 200;
	
	
	if (0<= (Ballx+150)&& (Ballx+150) <= 640 && 0<=(Bally+250) &&(Bally+250) <= 360) {
		double num1, num2, num3;

		Ballx = Ballx + 50;//---------------------------------------------------------------------------------------------적당한 움직임 숫자 찾기 프레임당 움직임
		num1 = Ballx * tan(firingangle);
		num2 = 2 * firingpower * firingpower * cos(firingangle) * cos(firingangle);
		num3 = 10 * Ballx * Ballx / num2;
		Bally = num1 - num3;//포물선 공식
		locateObject(Ball[1], scene1, Ballx+150, Bally+250);// Ballx는 포물선 상 좌표 실제는 Ballx에다가 150 더해준거 y 도 마찬가지
		stopTimer(timer1);
		startTimer(timer1);
		setTimer(timer1, 0.01f);
		rotationcache++;
		firingrotaition();
		ballmovement = ballmovement + 50;//-----------------------------------------------프레임당 움직임
		rotationcache++;
		balllaunched = 1;
		if (50 >= (Bally + 150)) {// 화면 내에서 떨어졌을때 애니메이션 멈추는 장치----------------------------------보완 필요 !!!!!!!!!
			stopTimer(timer1);
			score = Ballx;

			playSound(thud);

			char buf[256];
			sprintf_s(buf, "제 점수는요 %.0lf점 입니다!!", score/100, scene1);// 점수 메세지 출력 부분 총2개 있음 주의!
			showMessage(buf);

			money = money + score / 100;
			showObject(shopbutton);
			showObject(restartbutton);
		}
	}
	
	
	else if ((Ballx+150) > 640 || (Bally+250) > 360) {
		if (balllaunched == 1) {
			
			if (BallY-1280>-1030) {// 땅에 박혔을때 애니메이션 멈추는 장치----------------------------------보완 필요 !!!!!!!!!
				stopTimer(timer1);
				score = Ballx - BallX;

				playSound(thud);

				char buf[256];
				sprintf_s(buf, "제 점수는요 %.0lf점 입니다!!", score/100, scene1);// 점수 메세지 출력 부분 총2개 있음 주의!
				showMessage(buf);

				money = money + score / 100;
				showObject(shopbutton);
				showObject(restartbutton);
			}
			else {
				
				double num1, num2, num3;
				BallX = BallX - ballmovement;//---------------------------------------------------프레임당 움직임
				num1 = BallX * tan(firingangle);
				num2 = 2 * firingpower * firingpower * cos(firingangle) * cos(firingangle);
				num3 = 10 * BallX * BallX / num2;
				BallY = num1 + num3;//포물선 공식

				locateObject(cannon, scene1, BallX, BallY); // ----------------------------------------------------------------------------------------수정해야되지만 귀찮음 굳이?
				locateObject(base, scene1, BallX, BallY);

				BallX1 = BallX + 4300;
				BallXr = BallX - 1280;
				BallX1r = BallX1 - 1280;
				
				if ((BallXr+4300+backgroundsliding) <= 0 && (BallY - 1030) < -1030) {//이미지 평행이동 함수 실제 로케이트 되는 곳으로 판단해야
					//4300은 이미지 가로 길이, backgroundsliding은 이미지가 총 평행이동한 거리
					
					backgroundsliding = backgroundsliding + 8600;					
				}
				if ((BallX1r + 4300 + backgroundsliding1) <= 0 && (BallY - 1030) < -1030) {
					
					backgroundsliding1 = backgroundsliding1 + 8600;
				}
				
				locateObject(background, scene1, BallXr + backgroundsliding, BallY - 1030); //BallX는 포물선 상 좌표 실제는 BallX에다가 -1280 더해준거 Y 도 마찬가지
				locateObject(background1, scene1, BallX1r + backgroundsliding1, BallY - 1030);
				
				stopTimer(timer1);
				startTimer(timer1);
				setTimer(timer1, 0.01f);//--------------------------------------------------------------------------------------실험용
				rotationcache++;
				firingrotaition();

			}
		
	}
	}		
	}

void restart() {
	showObject(powpointer);
	showObject(angpointer);
}

void Gamestarter() {
	startTimer(angAni);
	startTimer(powAni);
	balllaunched = 0;
	ballmovement = 0;
	angAniworking = 1;
	powAniworking = 1;
	angx = 0, angX = 0;
	powx = 0, powX = 0;
	Ballx = 0, Bally = 0;
	BallX = 0, BallY = 0,BallX1 = 0;
	BallXr = 0, BallX1r = 0;
	backgroundsliding = 0;
	backgroundsliding1 = 0;
	showObject(powpointer);
	showObject(angpointer);
	showObject(angstopbutton);
	showObject(powstopbutton);
	locateObject(background, scene1, -520, -1030);
	locateObject(background1, scene1, 3780, -1030);
	locateObject(Ball[1], scene1, 150, 250);
	locateObject(cannon, scene1, 15, 188); 
	locateObject(base, scene1, 0, 0);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == angstopbutton) {
		angAnimationstop();
		if (angAniworking == 0 && powAniworking == 0) {
			hideObject(powpointer);
			hideObject(angpointer);
			firinganimation();
			playSound(fire);
		}
	}
	else if (object == powstopbutton) {
		powAnimationstop();
		if (angAniworking == 0 && powAniworking == 0) {
			hideObject(powpointer);
			hideObject(angpointer);
			firinganimation();
			playSound(fire);
		}
	}
	else if (object == startbutton) {
		Gamestarter();
		hideObject(startbutton);
	}
	else if (object == shopbutton) {
		enterScene(shop);
		showObject(backbutton);

		char buf[256];
		sprintf_s(buf, "현재 돈 : %d", money, shop);
		showMessage(buf);

	}
	else if (object == backbutton) {
		enterScene(scene1);
		Gamestarter();
	}
	else if (object == titlestartbutton) {
		enterScene(scene1);

	}
	else if (object == restartbutton) {
		enterScene(scene1);
		Gamestarter();

	}
	else if (object == angaccuracyicon) {
		if (angaccuracylev == 1) {
			if (money >= 500) {
				setObjectImage(angaccuracyicon, "image/angaccuracyicon2.png");
				money = money - 500;
				angaccuracylev++;
				anganispeed = anganispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else if (angaccuracylev == 2) {
			if (money >= 1000) {
				setObjectImage(angaccuracyicon, "image/angaccuracyicon3.png");
				money = money - 1000;
				angaccuracylev++;
				anganispeed = anganispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else if (angaccuracylev == 3) {
			if (money >= 1500) {
				setObjectImage(angaccuracyicon, "image/angaccuracyicon4.png");
				money = money - 1500;
				angaccuracylev++;
				anganispeed = anganispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else {
			showMessage("풀업글 완료");
		}
	}
	else if (object == powaccuracyicon) {
		if (powaccuracylev == 1) {
			if (money >= 500) {
				setObjectImage(powaccuracyicon, "image/powaccuracyicon2.png");
				money = money - 500;
				powaccuracylev++;
				powanispeed = powanispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else if (powaccuracylev == 2) {
			if (money >= 1000) {
				setObjectImage(powaccuracyicon, "image/powaccuracyicon3.png");
				money = money - 1000;
				powaccuracylev++;
				powanispeed = powanispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else if (powaccuracylev == 3) {
			if (money >= 1500) {
				setObjectImage(powaccuracyicon, "image/powaccuracyicon4.png");
				money = money - 1500;
				powaccuracylev++;
				powanispeed = powanispeed - 5;
			}
			else {
				showMessage("돈이 없어요 ㅠ");
			}
		}
		else {
			showMessage("풀업글 완료");
		}

	}
	else if (object == maxpowericon) {
	if (maxpowerlev == 1) {
		if (money >= 500) {
			setObjectImage(maxpowericon, "image/maxpowericon2.png");
			money = money - 500;
			maxpowerlev++;
			defaultpower = defaultpower + 200;
			
		}
		else {
			showMessage("돈이 없어요 ㅠ");
		}
	}
	else if (maxpowerlev == 2) {
		if (money >= 1000) {
			setObjectImage(maxpowericon, "image/maxpowericon3.png");
			money = money - 1000;
			maxpowerlev++;
			defaultpower = defaultpower + 100;
		}
		else {
			showMessage("돈이 없어요 ㅠ");
		}
	}
	else if (maxpowerlev == 3) {
		if (money >= 1500) {
			setObjectImage(maxpowericon, "image/maxpowericon4.png");
			money = money - 1500;
			maxpowerlev++;
			defaultpower = defaultpower + 10;
		}
		else {
			showMessage("돈이 없어요 ㅠ");
		}
	}
	else {
		showMessage("풀업글 완료");
	}
	} 
	else if (object == baseballicon) {
	setObjectImage(Ball[1], "image/baseball1.png");//// 로테이션 프로세스까지 다다다다다다 해야 엉어유ㅠ

	}
	playSound(click);
}

   // if (object == ) {       }


void timerCallback(TimerID timer) {
	if (timer == timer1) {
		/*BallX = BallX - 50;
		BallY = 0.0001*(-BallX-640)*(-BallX - 640) - 409.6;
		locateObject(background, scene1, BallX, BallY);
		stopTimer(timer1);
		startTimer(timer1);
		setTimer(timer1, 0.001f);
		*/
		firinganimation();
	}
	else if (timer == angAni) {
		angleanimaion();
	}
	else if (timer == powAni) {
		poweranimaion();
	}
}



/*void startGame () {// ------------------------------------------------------------------------------------------------????
	if (angAniworking == 1) {	
		double num;
		startTimer(angAni);
		angAniworking = 0;
		num = angX + 160;
		num = num / 320;
		firingangle = num * 90;
		

	}
	else if (powAniworking == 1) {
		double firingpowerscale;
		startTimer(powAni);
		powAniworking = 0;
		firingpowerscale = powX / 450;
		firingpower = firingpowerscale * defaultpower;
	}
	

}*/

int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	scene1 = createScene("게임화면", "image/1.png");
	title = createScene("타이틀화면", "image/title.png");
	introduction = createScene("설명화면", "image/1.png");
	scoreboard = createScene("점수화면", "image/1.png");
	shop = createScene("상점", "image/shop back.png");


	background = createObject("image/배경 수정.png", scene1, -520, -1030, true);
	background1 = createObject("image/배경 수정1.png", scene1, 3780, -1030, true);
	
	
	/*char Ball[12];
	for (int i = 0; i < 12; i++) {
		sprintf (Ball, "baseball%d.png", i + 1);
		createObject(Ball);
		locateObject(Ball[i], scene1, 150, 250);
		//showObject(Ball[i]);
	}*/
	
	Ball[1] = createObject("image/baseball1.png", scene1, 150, 250, true);
	base = createObject("image/base.png", scene1, 0, 0, true);

	angpointer = createObject("image/anggauge.png", scene1, 190, 40,false);
	powpointer = createObject("image/anggauge.png", scene1, 770, 40, false);

	angstopbutton = createObject("image/button1.png", scene1, 555, 35, false);
	powstopbutton = createObject("image/button1.png", scene1, 655, 35, false);

	titlestartbutton = createObject("image/start.png", title, 400, 250, true);
	startbutton = createObject("image/start.png", scene1, 400, 250, true);
	shopbutton = createObject("image/shopbutton.png", scene1, 1000, 500, false);
	backbutton = createObject("image/back.png", shop, 10, 600, false);
	restartbutton = createObject("image/restart.png", scene1, 100, 500, false);

	angaccuracyicon = createObject("image/angaccuracyicon1.png", shop, 850, 450, true);	
	powaccuracyicon = createObject("image/powaccuracyicon1.png", shop, 850, 250, true);	
	maxpowericon = createObject("image/maxpowericon1c.png", shop, 850, 20, true);

	baseballicon = createObject("image/baseballicon.png", shop, 195, 395, true);
	basketballicon = createObject("image/basketballicon.png", shop, 195, 245, true);
	
	
	cannon = createObject("image/cannon00.png", scene1, 15, 188, true);
	
	



	timer1 = createTimer(0.01f);
	
	
	
	angAni = createTimer(0.01f);
	powAni = createTimer(0.01f);
	

	startGame(title);
	
}






	/*int a = 0;
	double BallX=150, BallY=250;
	if (0<= BallX <= 640 && 0<=BallY <= 360) {
		double num1, num2, num3;

		BallX = BallX + 1;//---------------------------------------------------------------------------------------------적당한 움직임 숫자 찾기
		num1 = BallX * tan(firingangle);
		num2 = 2 * firingpower * firingpower * cos(firingangle) * cos(firingangle);
		num3 = 10 * BallX * BallX / num2;
		BallY = num1 - num3;//포물선 공식
		locateObject(Ball[1], scene1, BallX, BallY);
		stopTimer(timer1);
		startTimer(timer1);
		setTimer(timer1, 1.0f);
		ballmovement = ballmovement + 50;
		a = 1;
	}
	else if (BallX > 640 || BallY > 360) {
	*/


	/*if (a == 1) {
					BallX = BallX - ballmovement;
					a = 0;
				}*/