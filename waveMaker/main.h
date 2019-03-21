#ifndef _MAIN_H_
#define	_MAIN_H_

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#pragma comment(lib, "winmm.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FILE_NAME			("test.wav")	// �t�@�C����

#define CHANNEL_MAX			(2)				// �`�����l����
#define BIT_RATE_8			(8)				// �r�b�g���[�g
#define BIT_RATE_16			(16)			// �r�b�g���[�g
#define SAMPLE_FREQ_MAX		(200000)		// �T���v�����O���g��
#define FREQ_MAX			(40000.0f)		// ���g��

#define FREQ_MID_8BIT		(128)			// 8�r�b�g�̒��Ԓl	

//*****************************************************************************
// �萔��`
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
// �\����
//*****************************************************************************
typedef struct	// �g�`�p�ϐ� 
{
	float secPerSample;		// 1s / �T���v�����O���g��
	float samplePos;		// �T���v�����O�̈ʒu 
	float waveIndex;		// �g�`�Ԗ�
	float wavePos;			// �g�`���̈ʒu
	float secPerFreq;		// 1s / ���g��
	float volume;			// �{�����[��

}WAVE_DATA;

#endif
