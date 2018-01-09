#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//�萔
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

//---�S�̂̃p�[�Z���g��ێ�����\����---
typedef struct{
	int rare;           //���A�x
	float percent;     //�p�[�Z���g
} percentData;

//---�L�����̃f�[�^---
typedef struct{
	char characterName[32];
	char words[512];
} charaData;

//---�v���g�^�C�v�錾---
int loadAllPercentCSV(percentData *data);
int lotteryRare(percentData *data, int *rare);
int getRareCSVName(char **name, int rare);
int getAllCharaData(charaData *data, int *length, char *csvName);
int chooseCharacter(charaData *charas, int length, charaData *choseChara);
void dispEffect();
int dispWords(charaData data, int rare);

int main(void){
	//--�ϐ��̐錾�E�����̏��� --
	//���I�Ɏg���ϐ�
	percentData allPercent[ALL_PERCENT_LENGTH];
	int choseRare = 0;
	//�L�����f�[�^�֌W�Ɏg���ϐ�
	charaData characters[CHARACTER_LENGTH];
	int charactersNum = 0;
	char *choseCSVName = "";
	charaData choseCharacter;
	//�ėp
	int i;
	int check;
	srand((unsigned)time(NULL));

    //�f�[�^�̎擾
	if(loadAllPercentCSV(allPercent) == -1){
		printf("CSV�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return 0;
	}

	//���������̕\��
	puts("�K�`���V�X�e���i�e�X�g�j�ւ悤�����B");
	Sleep(1000);
	puts("���̃V�X�e���͒��쌠�̖��ɂ��A�f�o�b�O�d�l�ɂȂ��Ă��܂��B");
	Sleep(1000);
	puts("���X�̓��A��S���A�̒�����m�����I����悤�ɂȂ��Ă��܂��B");
	Sleep(2000);

	printf("�ł́A�K�`�������Ă���낵���ł���? yes[1] OR no[0] : ");
	scanf_s("%d", &check);


	//���[�U��y�ȊO����͂���܂ő�����
	while(check == 1){
		//���I
		if(lotteryRare(allPercent, &choseRare) == -1){
			printf("�m���̒��I���ɖ�肪�������܂����B\n");
			return 0;
		}

		//�L������CSV���𒊏o
		if(getRareCSVName(&choseCSVName, choseRare) == -1){
			printf("�L������CSV�����o���ɖ�肪�������܂����B\n");
			return 0;
		}

		//���I���ꂽ���A�x�̑S�L�����𒊏o
		if(getAllCharaData(characters, &charactersNum, choseCSVName) == -1){
			printf("CSV���L�����̒��o���ɖ�肪�������܂����B\n");
			return 0;
		}

		//�Ō��1�L�����𒊑I����
		if(chooseCharacter(characters, charactersNum, &choseCharacter) == -1){
			printf("�L�����̒��I���ɖ�肪�������܂����B\n");
		}

		//���o�����傱����
		dispEffect();

		//�L�����̏��Ƒ䎌��\��
		if(dispWords(choseCharacter, choseRare) == -1){
			printf("�L�������\�����ɖ�肪�������܂����B\n");
		}

		//�����邩�m�F
		printf("\n�K�`���𑱂��܂����H yes[1] OR no[0] : ");
		scanf_s("%d", &check);
	}

	//�I���̂�������
	printf("\n");
	puts("�ł́A�I���������܂��B");
	puts("�V��ł����������肪�Ƃ��������܂����B");
    puts("���̃E�B���h�E��4�b��Ɏ����ŕ��܂�");
	Sleep(4000);

	return 0;
}

//---csv�̓ǂݍ���---
int loadAllPercentCSV(percentData *data){
	//�ϐ��̐錾
	int i;
	FILE *fp;
	int line = 0;
	char rare[3];
	char percent[3];

	//�t�@�C���̓ǂݍ���
	fp = fopen(ALL_PERCENT_CSV, "r");
	if(fp == NULL){
		printf("%s�t�@�C�����J���܂���B\n", ALL_PERCENT_CSV);
		return -1;
	}

	//�f�[�^�̑��
	while((line = fscanf(fp, "%[^,],%s,%s",rare ,percent)) != EOF){
		data->rare = strtol(rare, NULL, 10);
		data->percent = strtol(percent, NULL, 10);
		data++;
	}

	fclose(fp);

	//�ԋp
	return 1;
}

//---���I���\�b�h---
int lotteryRare(percentData *data, int *rare){
	//�ϐ��̐錾
	int max = 0;
	int randResult = 0;
	int i;
	percentData *head = data;

	//�m���̍��v���Z�o
	for(i = 0; i < ALL_PERCENT_LENGTH; i++){
		max += data->percent;
		data++;
	}

	//�m���̒��I
	randResult = (int)(rand() % (max - 1));
	data = head;

	//���I���ꂽ���A�x��ϐ��֑��
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

//���I���ꂽ���A�x����CSV���𔻕ʂ����
int getRareCSVName(char **name, int rare){
	//�{���͂����ł��ꂼ��̃��A�x�ɑΉ�����CSV�f�[�^���擾����͂�������
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
	//����̓e�X�g�p��CSV�������I�ɓǂݍ��܂���.
    *name = TEST_CSV;

    return 1;
}

//�w�肳�ꂽCSV�̑S�L�����f�[�^���i�[����
int getAllCharaData(charaData *data, int *length, char *csvName){
	//�ϐ��̐錾
	int i;
	FILE *fp;
	int line;
	int count = 0;
	char name[32];
    char words[512];

	//�t�@�C���̓ǂݍ���
	fp = fopen(csvName, "r");
	if(fp == NULL){
		printf("%s�t�@�C�����J���܂���B\n", csvName);
		return -1;
	}

	//�f�[�^�̑��
	while((line = fscanf(fp, "%[^,],%[^\n],%s,%s",name ,words)) != EOF){
		strcpy(data->characterName, name);
		strcpy(data->words, words);
		data++;
		count++;
	}

	*length = count;

	fclose(fp);

	//�ԋp
	return 1;

}

//�S�L�����̃f�[�^����P�L�����I�o
int chooseCharacter(charaData *charas, int length, charaData *choseChara){
	//�ϐ��̐錾
	int randNum = (int)(rand() % (length - 1));

	//���o
	*choseChara = charas[randNum];

	return 1;
}

//���o
void dispEffect(){
	//10��u����\������v
	int i;
	for(i = 0; i < 20; i++){
		if(i % 2 == 0) printf("��");
		else printf("��");
		Sleep(150);
	}
	printf("\n!�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`!\n");
}

//�L�����̑䎌���������\������
int dispWords(charaData chara, int rare){
	//�ϐ��̐錾
	int i;

	//���ڂ̑ҋ@
	Sleep(1000);

	//���A�x��\��
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

	//���O��\��
	printf("Name:%s\n", chara.characterName);

	//���ڂ̑ҋ@
	Sleep(1000);

	//�x�点�����̕\��
	for(i = 0; i < strlen(chara.words); i++){
		printf("%c", chara.words[i]);
		Sleep(80) ;
	}

	printf("\n");

	return 1;

}