#ifndef _MAIN_H_
#define	_MAIN_H_

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#pragma comment(lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FILE_NAME			("test.wav")	// ファイル名

#define CHANNEL_MAX			(2)				// チャンネル数
#define BIT_RATE_8			(8)				// ビットレート
#define BIT_RATE_16			(16)			// ビットレート
#define SAMPLE_FREQ_MAX		(200000)		// サンプリング周波数
#define FREQ_MAX			(40000.0f)		// 周波数

#define FREQ_MID_8BIT		(128)			// 8ビットの中間値	

//*****************************************************************************
// 定数定義
//*****************************************************************************
enum WAVE_TYPE
{
	WAVE_END,
	WAVE_SINE,
	WAVE_SQUARE,
	WAVE_SAWTOOTH
};

enum BIT_RATE
{
	BIT_ERROR,
	BIT_16,
	BIT_8
};

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct	// 波形用変数 
{
	float secPerSample;		// 1s / サンプリング周波数
	float samplePos;		// サンプリングの位置 
	float waveIndex;		// 波形番目
	float wavePos;			// 波形中の位置
	float secPerFreq;		// 1s / 周波数
	float volume;			// ボリューム

}WAVE_DATA;

#endif
