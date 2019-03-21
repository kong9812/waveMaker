//=============================================================================
// < WAVE MAKER OBJECT v1.0 >
// Main処理 [main.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/* 初期化 */
void Init(void);
/* 更新処理 */
void Update(void);
/* ファイルの書き出し */
void SetWavFile(int type);
/* 正弦波 */
void SetSineWave(WAVE_DATA waveData, int BitRate);
/* 矩形波 */
void SetSquareWave(WAVE_DATA waveData, int BitRate);
/* のこぎり波 */
void SetSawtooth(WAVE_DATA waveData, int BitRate);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
WAVEFORMATEX	wave;						// 私の好み(wave構造体)
WAVE_DATA		waveData;					// 波形データ
FILE			*fp;						// ファイル構造体							

bool			loop;						// ループ用							
							
char			*work;						// ファイルワーク

short			*buf16Bit;					// 16bitの波形バッファ
unsigned char	*buf8Bit;					// 8bitの波形バッファ

int				fileSize;					// ファイルサイズ															
int				waveSize;					// 波形サイズ

//=============================================================================
// メイン関数
//=============================================================================
int main(void)
{
	/* 初期化 */
	Init();
	
	/* ウィンドウズのロックオン */
	PlaySound("WindowsLogon", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	while (loop)
	{
		/* 更新処理 */
		Update();

		/* 再生 */
		PlaySound("test.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	/* ウィンドウズのロックオフ */
	PlaySound("WindowsLogoff", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	/* メッセージ */
	system("cls");
	printf("See You Next Time");

	/* キー押したら終了 */
	getchar();
	getchar();

	return 0;
}

//=============================================================================
// 初期化
//=============================================================================
void Init(void)
{
	/* ループ開始 */
	loop = true;

	{/* 波形データ */
		waveData.samplePos		= 0;
		waveData.secPerFreq		= 0;
		waveData.secPerSample	= 0;
		waveData.volume			= 0;
		waveData.waveIndex		= 0;
		waveData.wavePos		= 0;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	int waveType;	// 波形種類

	/* 波形種類 */
	printf("0:end 1:sine 2:square 3:sawtooth \n");
	scanf_s("%d", &waveType);
	
	/* ウィンドウズのエラー音 */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	// オーバしないように
	if (waveType < WAVE_END)
	{
		waveType = WAVE_END;
	}

	// オーバしないように
	else if (waveType > WAVE_SAWTOOTH)
	{
		waveType = WAVE_SAWTOOTH;
	}


	// 終了
	if (waveType == WAVE_END)
	{
		loop = false;
	}

	// 波形生成
	else
	{
		/* ファイルの書き出し */
		SetWavFile(waveType);
	}
}

//=============================================================================
// ファイルの書き出し
//=============================================================================
void SetWavFile(int type)
{
	int channel;	// チャンネル数
	int bitRate;	// ビットレート
	int sf;			// サンプリング周波数
	int freq;		// 周波数

	/* チャンネル数 */
	printf("channel? \n");
	scanf_s("%d", &channel);
	/* ウィンドウズのエラー音 */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (channel < 0 || channel>CHANNEL_MAX)
	{
		printf("error \n");
		return;
	}

	/* ビットレート */
	printf("bit rate?(8/16) \n");
	printf("1:16bit 2:8bit \n");
	scanf_s("%d", &bitRate);
	/* ウィンドウズのエラー音 */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	switch (bitRate)
	{
	case BIT_16:
		bitRate = BIT_RATE_16;
		break;
	case BIT_8:
		bitRate = BIT_RATE_8;
		break;
	default:
		printf("error \n");
		return;
		break;
	}

	/* サンプリング周波数 */
	printf("sampling frequency? \n");
	scanf_s("%d", &sf);
	/* ウィンドウズのエラー音 */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (sf <0 || sf >SAMPLE_FREQ_MAX)
	{
		printf("error \n");
		return;
	}

	/* 周波数 */
	printf("frequency? \n");
	scanf_s("%d", &freq);
	/* ウィンドウズのエラー音 */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (freq <0 || freq >FREQ_MAX)
	{
		printf("error \n");
		return;
	}

	/* ファイルの準備 */
	fopen_s(&fp, FILE_NAME, "wb");

	/* wav構造体の初期化 */
	wave.cbSize = 0;
	wave.nChannels = channel;
	wave.wBitsPerSample = bitRate;
	wave.nSamplesPerSec = sf;
	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nBlockAlign = (wave.nChannels*wave.wBitsPerSample) / 8;
	wave.nAvgBytesPerSec = wave.nSamplesPerSec*wave.nBlockAlign;

	/* ファイルサイズの初期化 */
	waveSize = (wave.nSamplesPerSec*(wave.wBitsPerSample / 8)) * 5;

	/* ファイルサイズの初期化 */
	fileSize = 44 + (waveSize);

	/* ワークのメモリー確保 */
	work = (char *)malloc(fileSize);

	/* [4 byte]riff */
	memcpy(work, "RIFF", 4);

	/* [4 byte]これ以降のファイルサイズ */
	work[4] = (fileSize - 8) >> 0 & 0xff;
	work[5] = (fileSize - 8) >> 8 & 0xff;
	work[6] = (fileSize - 8) >> 16 & 0xff;
	work[7] = (fileSize - 8) >> 24 & 0xff;

	/* [4 byte]wave */
	memcpy(work + 8, "WAVE", 4);

	/* [4 byte]fmt  */
	memcpy(work + 12, "fmt ", 4);

	/* [4 byte]バイト数 */
	work[16] = 16;
	work[17] = 0;
	work[18] = 0;
	work[19] = 0;

	/* [2 byte]フォーマットID */
	work[20] = 1;
	work[21] = 0;

	/* [2 byte]チャンネル数 */
	work[22] = (char)wave.nChannels;
	work[23] = 0;

	/* [4 byte]サンプリングレート */
	work[24] = wave.nSamplesPerSec >> 0 & 0xff;
	work[25] = wave.nSamplesPerSec >> 8 & 0Xff;
	work[26] = wave.nSamplesPerSec >> 16 & 0xff;
	work[27] = wave.nSamplesPerSec >> 24 & 0xff;

	/* [4 byte]データ速度(Byte/sec) サンプリング周波数×チャンネル数×量子化ビット/2*/
	work[28] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 0 & 0xff;
	work[29] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 8 & 0xff;
	work[30] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 16 & 0xff;
	work[31] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 14 & 0xff;

	/* [2 byte]プロックサイズ(Byte/sample*チャンネル数) */
	work[32] = (wave.wBitsPerSample / 8)*wave.nChannels >> 0 & 0xff;
	work[33] = (wave.wBitsPerSample / 8)*wave.nChannels >> 8 & 0xff;

	/* [2 byte]サンプリングあたりのビット数 */
	work[34] = wave.wBitsPerSample >> 0 & 0xff;
	work[35] = wave.wBitsPerSample >> 8 & 0xff;

	/* [4 byte]data */
	memcpy(work + 36, "data", 4);

	/* [4 byte]サウンド全体のバイト数n */
	work[40] = waveSize >> 0 & 0xff;
	work[41] = waveSize >> 8 & 0xff;
	work[42] = waveSize >> 16 & 0xff;
	work[43] = waveSize >> 24 & 0xff;

	{/* 初期化 */
		waveData.secPerSample = 1.0f / wave.nSamplesPerSec;
		waveData.secPerFreq = 1.0f / freq;
	}

	/* 波形生成 */
	if (wave.wBitsPerSample == BIT_RATE_16)
	{
		buf16Bit = (short *)malloc(waveSize * sizeof(short));
	}
	else if (wave.wBitsPerSample == BIT_RATE_8)
	{
		buf8Bit = (unsigned char *)malloc(waveSize * sizeof(unsigned char));
	}

	/* 波形の分岐 */
	switch (type)
	{
	case WAVE_SINE:
		SetSineWave(waveData,wave.wBitsPerSample);		// 正弦波
		break;
	case WAVE_SQUARE:
		SetSquareWave(waveData, wave.wBitsPerSample);	// 矩形波
		break;
	case WAVE_SAWTOOTH:
		SetSawtooth(waveData, wave.wBitsPerSample);		// のこぎり波
		break;
	default:
		break;
	}

	/* [n byte]波形情報 */
	if (wave.wBitsPerSample == BIT_RATE_16)
	{
		memcpy(work + 44, buf16Bit, waveSize);
	}
	else if (wave.wBitsPerSample == BIT_RATE_8)
	{
		memcpy(work + 44, buf8Bit, waveSize);
	}

	/* 書き出し */
	fwrite(work, fileSize, 1, fp);


	{/* またね～処理 */
		if (wave.wBitsPerSample == BIT_RATE_16)
		{
			free(buf16Bit);
		}
		else if (wave.wBitsPerSample == BIT_RATE_8)
		{
			free(buf8Bit);
		}
		fclose(fp);
		free(work);
	}

}

//=============================================================================
// 正弦波
//=============================================================================
void SetSineWave(WAVE_DATA waveData,int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* 正弦波 */
		waveData.samplePos = waveData.secPerSample * i;
		waveData.waveIndex = waveData.samplePos / waveData.secPerFreq;
		waveData.wavePos = fmodf(waveData.waveIndex, 1.0);
		waveData.volume = (float)i / waveSize;
		if (wave.wBitsPerSample == BIT_RATE_16)
		{
			buf16Bit[i] = (short)((float)(SHRT_MAX*waveData.volume) * sinf((float)(2.0f*M_PI*waveData.wavePos)));
		}
		else if (wave.wBitsPerSample == BIT_RATE_8)
		{
			buf8Bit[i] = (unsigned char)((float)(CHAR_MAX *waveData.volume) * sinf((float)(2.0f*M_PI*waveData.wavePos)) + FREQ_MID_8BIT);
		}
	}
}

//=============================================================================
// 矩形波
//=============================================================================
void SetSquareWave(WAVE_DATA waveData, int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* 矩形波 */
		waveData.samplePos = waveData.secPerSample * i;
		waveData.waveIndex = waveData.samplePos / waveData.secPerFreq;
		waveData.wavePos = fmodf(waveData.waveIndex, 1.0);
		waveData.volume = (float)i / waveSize;
		if (waveData.wavePos < 0.5f)
		{
			if (wave.wBitsPerSample == BIT_RATE_16)
			{
				buf16Bit[i] = (short)(SHRT_MAX*waveData.volume);
			}
			else if (wave.wBitsPerSample == BIT_RATE_8)
			{
				buf8Bit[i] = (unsigned char)((CHAR_MAX*waveData.volume)+ FREQ_MID_8BIT);
			}
		}
		else
		{
			if (wave.wBitsPerSample == BIT_RATE_16)
			{
				buf16Bit[i] = (short)(SHRT_MIN*waveData.volume);
			}
			else if (wave.wBitsPerSample == BIT_RATE_8)
			{
				buf8Bit[i] = (unsigned char)((CHAR_MIN*waveData.volume) + FREQ_MID_8BIT);
			}

		}
	}
}

//=============================================================================
// のこぎり波
//=============================================================================
void SetSawtooth(WAVE_DATA waveData, int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* のこぎり波 */
		waveData.samplePos = waveData.secPerSample * i;
		waveData.waveIndex = waveData.samplePos / (waveData.secPerFreq / 2);
		waveData.volume = (float)i / waveSize;
		if (wave.wBitsPerSample == BIT_RATE_16)
		{
			buf16Bit[i] = (short)((short)(((float)SHRT_MAX + 1)
				* waveData.waveIndex)* waveData.volume);
		}
		else if (wave.wBitsPerSample == BIT_RATE_8)
		{
			buf8Bit[i] = (unsigned char)((unsigned char)(((float)CHAR_MAX + 1)
				* waveData.waveIndex));
		}
	}
}
