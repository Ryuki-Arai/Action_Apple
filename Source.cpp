//ヘッダーファイル読み込み
#include "DxLib.h"
#include <math.h>

//コンパイル時の警告表示抑制
#pragma warning( disable:4305 4244 4305 )

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	reset:

	//変数宣言
	int jiki;
	int top;
	int back;
	int tekimiss;

	int setup;
	int level = 50;
	int result;
	float ag;

	int set = 1;

	int jx = 100;
	int jy = 550;

	float d;

	int tmf[1000];
	int tmw[1000];
	int tmx[1000];
	int tmy[1000];
	int tmyd[1000];

	int tmwait = 0;

	int point = 0;

	long long int ji = 0;
	long long int hun = 0;
	long long int byou = 0;
	long long int mill = 0;
	long long int starttime = 0;
	long long int now;
	long long int temp = 0;

	int sh0, sh1, sh2, sh3, sh4;

	int upf = 0, downf = 0;
	int mousf = 0;
	int menuf = 0;


	for (int i = 0; i <= 999; i++)
	{
		tmf[i] = 0;
		tmw[i]=0;
	}

	SetGraphMode( 1366, 768, 32 );		//画面サイズ設定(どれかひとつを選択)
	ChangeWindowMode(TRUE);				//ウィンドウモードで起動、コメントアウトでフルスクリーン
	if (DxLib_Init() == -1) return -1;	//DXライブラリ初期化

	SetDrawScreen(DX_SCREEN_BACK);		//裏画面に描画

	jiki = LoadGraph("jiki.png");				//自機画像
	tekimiss = LoadGraph("tekimiss.png");

	top = LoadGraph("top.jpg");
	back = LoadGraph("back.jpg");				//背景画像
	setup = LoadGraph("setup.jpg");
	result = LoadGraph("result.jpg");

	sh0 = LoadSoundMem("op.mp3");
	sh1 = LoadSoundMem("bgm.mp3");
	sh2 = LoadSoundMem("jump01.mp3");
	sh3 = LoadSoundMem("end.mp3");
	sh4 = LoadSoundMem("button04b.mp3");

	PlaySoundMem(sh0, DX_PLAYTYPE_LOOP);	//BGMの再生

	back:

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア
		DrawGraph(0, 0, top, FALSE);

		SetFontSize(36);
		DrawFormatString(450, 500, GetColor(255, 255, 255), "マウス左クリックで開始");

		SetFontSize(36);
		DrawFormatString(550, 600, GetColor(255, 255, 0), "メニュー(M)");

		if (GetMouseInput() && MOUSE_INPUT_LEFT == 1)	{
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			mousf = 1;
		}

		if (mousf == 1 && CheckHitKeyAll()==0)	{
			mousf = 0;
			goto start;
		}

		if (CheckHitKey(KEY_INPUT_M) == 1)	{
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			menuf = 1;
		}

		if (menuf == 1 && CheckHitKeyAll() == 0)	{
			menuf = 0;
			break;
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え
	}
	
	//メニューループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア
		DrawGraph(0, 0, setup, FALSE);

		SetFontSize(36);
		DrawFormatString(200, 150, GetColor(255, 255, 255), "上下キーを押して難易度を選択して下さい。");

		if (CheckHitKey(KEY_INPUT_UP) == 1 && set >=2)	{
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			upf = 1;
		}

		if (CheckHitKey(KEY_INPUT_DOWN) == 1 && set <= 2)	{
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			downf = 1;
		}

		if (upf == 1 && CheckHitKeyAll()==0)	{
			upf = 0;
			set--;
		}
		if (downf == 1 && CheckHitKeyAll() == 0)	{
			downf = 0;
			set++;
		}
		if (set == 1) {
			SetFontSize(72);
			DrawFormatString(500, 250, GetColor(255, 0, 255), "林檎狩り<選択中>");
			level = 50;
		}
		else	{
			SetFontSize(72);
			DrawFormatString(500, 250, GetColor(255, 0, 255), "林檎狩り");
		}

		if (set == 2) {
			SetFontSize(72);
			DrawFormatString(530, 400, GetColor(255, 255, 0), "収穫期<選択中>");
			level = 10;
		}
		else	{
			SetFontSize(72);
			DrawFormatString(530, 400, GetColor(255, 255, 0), "収穫期");
		}

		if (set == 3) {
			SetFontSize(72);
			DrawFormatString(530, 550, GetColor(0, 255, 255), "林檎雨<選択中>");
			level = 1;
		}
		else	{
			SetFontSize(72);
			DrawFormatString(530, 550, GetColor(0, 255, 255), "林檎雨");
		}

		SetFontSize(36);
		DrawFormatString(1000, 650, GetColor(255, 255, 255), "戻る(B)");

		if (CheckHitKey(KEY_INPUT_BACK) == 1){
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			goto back;
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え
	}
	start:

	StopSoundMem(sh0);
	
	PlaySoundMem(sh1, DX_PLAYTYPE_LOOP);	//BGMの再生

	starttime = GetNowCount();

	//メインループ（このループが１秒間に数十回繰り返されます）
	while(ProcessMessage()==0 && CheckHitKey(KEY_INPUT_ESCAPE)==0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア
		DrawGraph(0, 0, back, FALSE);
		tmwait++;

		GetMousePoint(&jx, &jy);
		
		DrawGraph(jx - 32, jy - 32, jiki, TRUE);


		if (tmwait >= level) //50...初級　10...中級　1...上級   Min...999    Max...1
		{
			for (int i = 0; i <= 999; i++)
			{
				if (tmf[i] == 0)
				{
					tmf[i] = 1;
					tmx[i] = GetRand(1400) - 30;
					tmy[i] = -20;
					tmwait = 0;
					tmyd[i] = GetRand(4) + 1;
					break;
				}
			}
		}

		for (int i = 0; i <= 999; i++)
		{
			if (tmf[i] == 1)
			{
				tmy[i]+=tmyd[i];
				DrawGraph(tmx[i], tmy[i], tekimiss, TRUE);
				if (tmy[i] >= 800)tmf[i] = 0;
				d = sqrt((float)(((jx-32) - tmx[i])*((jx-32) - tmx[i]) + ((jy-32) - tmy[i])*((jy-32) - tmy[i])));

				if (d <= 64)
				{

					tmf[i] = 0;
					point = point + 1;
					ChangeVolumeSoundMem(190*80/100,sh2);
					PlaySoundMem(sh2, DX_PLAYTYPE_BACK);
				}
			}
		}
		
		SetFontSize(36);
		DrawFormatString(1222, 702, GetColor(255, 255, 255), "%d", point);

		now = GetNowCount() - starttime;
		temp = now / 1000;
		ji = temp / 3600;
		temp = temp - ji * 3600;
		hun = temp / 60;
		temp = temp - hun * 60;
		byou = temp;
		mill = now - temp;

		if (hun == 3){
			if (byou == 6){
				SetFontSize(126);
				DrawFormatString(653, 354, GetColor(255, 255, 255), "3");
			}
			if (byou == 7){
				SetFontSize(126);
				DrawFormatString(653, 354, GetColor(255, 255, 255), "2");
			}
			if (byou == 8){
				SetFontSize(126);
				DrawFormatString(653, 354, GetColor(255, 255, 255), "1");
				}
			if (byou == 9){
				SetFontSize(126);
				DrawFormatString(573, 354, GetColor(255, 255, 255), "終了!");
			}
			if (byou >= 10)	{
				break;
			}
		}
		
		if (CheckHitKey(KEY_INPUT_E) == 1){
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			break;
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え

	}	//メインループ終了
	StopSoundMem(sh1);

	PlaySoundMem(sh3, DX_PLAYTYPE_LOOP);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア
		
		DrawGraph(0, 0, result, FALSE);
		ag = point * 295.6 / (1000.0 * 1000.0);
		SetFontSize(144);
		DrawFormatString(500, 200, GetColor(255, 255, 255), "%3d",point);
		SetFontSize(108);
		DrawFormatString(450, 450, GetColor(255, 255, 255), "%f",ag);

		SetFontSize(36);
		DrawFormatString(500, 600, GetColor(255, 255, 255), "マウス左クリックで終了");

		if (GetMouseInput() && MOUSE_INPUT_LEFT == 1)	{
			PlaySoundMem(sh4, DX_PLAYTYPE_BACK);
			mousf = 1;
		}

		if (mousf == 1 && CheckHitKeyAll() == 0)	{
			mousf = 0;
			goto reset;
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え
	}

	//DXライブラリ終了処理
	DxLib_End();

	return 0;
}
