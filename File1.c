#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//定数
#define ALL_PERCENT_CSV "CSV/AllPercents.csv"
#define RARE_CSV "CSV/Rare.csv"
#define SRARE_CSV "CSV/Srare.csv"
#define SSRARE_CSV "CSV/SSrare.csv"
#define TEST_CSV "CSV/Test.csv"
#define ALL_PERCENT_LENGTH 3
#define CHARACTER_LENGTH 64
#define RARE 1
#define SRARE 2
#define SSRARE 3

//---全体のパーセントを保持する構造体---
typedef struct{
	int rare;           //レア度
	float percent;     //パーセント
} percentData;

//---キャラのデータ---
typedef struct{
	char characterName[32];
	char words[512];
} charaData;

//---プロトタイプ宣言---
int loadAllPercentCSV(percentData *data);
int lotteryRare(percentData *data, int *rare);
int getRareCSVName(char **name, int rare);
int getAllCharaData(charaData *data, int *length, char *csvName);
int chooseCharacter(charaData *charas, int length, charaData *choseChara);
void dispEffect();
int dispWords(charaData data, int rare);

int main(void){
	//--変数の宣言・乱数の準備 --
	//抽選に使う変数
	percentData allPercent[ALL_PERCENT_LENGTH];
	int choseRare = 0;
	//キャラデータ関係に使う変数
	charaData characters[CHARACTER_LENGTH];
	int charactersNum = 0;
	char *choseCSVName = "";
	charaData choseCharacter;
	//汎用
	int i;
	int check;
	srand((unsigned)time(NULL));

    //データの取得
	if(loadAllPercentCSV(allPercent) == -1){
		printf("CSVデータの読み込みに失敗しました。\n");
		return 0;
	}

	//導入文字の表示
	puts("ガチャシステム（テスト）へようこそ。");
	Sleep(1000);
	puts("このシステムは著作権の問題により、デバッグ仕様になっています。");
	Sleep(1000);
	puts("元々はレアやSレアの中から確率抽選するようになっています。");
	Sleep(2000);

	printf("では、ガチャをしてもよろしいですか? yes[1] OR no[0] : ");
	scanf_s("%d", &check);


	//ユーザがy以外を入力するまで続ける
	while(check == 1){
		//抽選
		if(lotteryRare(allPercent, &choseRare) == -1){
			printf("確率の抽選中に問題が発生しました。\n");
			return 0;
		}

		//キャラのCSV名を抽出
		if(getRareCSVName(&choseCSVName, choseRare) == -1){
			printf("キャラのCSV名抽出時に問題が発生しました。\n");
			return 0;
		}

		//抽選されたレア度の全キャラを抽出
		if(getAllCharaData(characters, &charactersNum, choseCSVName) == -1){
			printf("CSVよりキャラの抽出時に問題が発生しました。\n");
			return 0;
		}

		//最後の1キャラを抽選する
		if(chooseCharacter(characters, charactersNum, &choseCharacter) == -1){
			printf("キャラの抽選時に問題が発生しました。\n");
		}

		//演出をちょこっと
		dispEffect();

		//キャラの情報と台詞を表示
		if(dispWords(choseCharacter, choseRare) == -1){
			printf("キャラ情報表示時に問題が発生しました。\n");
		}

		//続けるか確認
		printf("\nガチャを続けますか？ yes[1] OR no[0] : ");
		scanf_s("%d", &check);
	}

	//終了のあいさつ
	printf("\n");
	puts("では、終了いたします。");
	puts("遊んでいただきありがとうございました。");
    puts("このウィンドウは4秒後に自動で閉じます");
	Sleep(4000);

	return 0;
}

//---csvの読み込み---
int loadAllPercentCSV(percentData *data){
	//変数の宣言
	int i;
	FILE *fp;
	int line = 0;
	char rare[3];
	char percent[3];

	//ファイルの読み込み
	fp = fopen(ALL_PERCENT_CSV, "r");
	if(fp == NULL){
		printf("%sファイルが開けません。\n", ALL_PERCENT_CSV);
		return -1;
	}

	//データの代入
	while((line = fscanf(fp, "%[^,],%s,%s",rare ,percent)) != EOF){
		data->rare = strtol(rare, NULL, 10);
		data->percent = strtol(percent, NULL, 10);
		data++;
	}

	fclose(fp);

	//返却
	return 1;
}

//---抽選メソッド---
int lotteryRare(percentData *data, int *rare){
	//変数の宣言
	int max = 0;
	int randResult = 0;
	int i;
	percentData *head = data;

	//確率の合計を算出
	for(i = 0; i < ALL_PERCENT_LENGTH; i++){
		max += data->percent;
		data++;
	}

	//確率の抽選
	randResult = (int)(rand() % (max - 1));
	data = head;

	//抽選されたレア度を変数へ代入
	for(i = 0; i < ALL_PERCENT_LENGTH; i++){
		max -= (int)data->percent;
		if(max <= randResult){
			*rare = data->rare;
			return 1;
		}
		data++;
	}


	return -1;
}

//抽選されたレア度からCSV名を判別し代入
int getRareCSVName(char **name, int rare){
	//本当はここでそれぞれのレア度に対応したCSVデータを取得するはずだった
	/*switch(rare){
		case RARE:
			*name = RARE_CSV;
			break;
		case SRARE:
			*name = SRARE_CSV;
			break;
		case SSRARE:
			*name = SSRARE_CSV;
			break;
		default:
			return -1;
	}*/
	//今回はテスト用のCSVを強制的に読み込ませる.
    *name = TEST_CSV;

    return 1;
}

//指定されたCSVの全キャラデータを格納する
int getAllCharaData(charaData *data, int *length, char *csvName){
	//変数の宣言
	int i;
	FILE *fp;
	int line;
	int count = 0;
	char name[32];
    char words[512];

	//ファイルの読み込み
	fp = fopen(csvName, "r");
	if(fp == NULL){
		printf("%sファイルが開けません。\n", csvName);
		return -1;
	}

	//データの代入
	while((line = fscanf(fp, "%[^,],%[^\n],%s,%s",name ,words)) != EOF){
		strcpy(data->characterName, name);
		strcpy(data->words, words);
		data++;
		count++;
	}

	*length = count;

	fclose(fp);

	//返却
	return 1;

}

//全キャラのデータから１キャラ選出
int chooseCharacter(charaData *charas, int length, charaData *choseChara){
	//変数の宣言
	int randNum = (int)(rand() % (length - 1));

	//抽出
	*choseChara = charas[randNum];

	return 1;
}

//演出
void dispEffect(){
	//10回「□を表示する」
	int i;
	for(i = 0; i < 20; i++){
		if(i % 2 == 0) printf("□");
		else printf("■");
		Sleep(150);
	}
	printf("\n!～～～～～～～～～～～～～～～～～～～!\n");
}

//キャラの台詞をゆっくり表示する
int dispWords(charaData chara, int rare){
	//変数の宣言
	int i;

	//一回目の待機
	Sleep(1000);

	//レア度を表示
	printf("Rare:");
	if(rare == RARE){ printf("RARE"); }
	else if(rare == SRARE){ printf("SRARE"); }
	else if(rare == SSRARE){
		char *ssrare = "SSRARE!!";
		for(i = 0; i < strlen(ssrare); i++){
            printf("%c", ssrare[i]);
			Sleep(150);
		}
	}
	printf("\n");

	//名前を表示
	printf("Name:%s\n", chara.characterName);

	//二回目の待機
	Sleep(1000);

	//遅らせつつ文字の表示
	for(i = 0; i < strlen(chara.words); i++){
		printf("%c", chara.words[i]);
		Sleep(80) ;
	}

	printf("\n");

	return 1;

}