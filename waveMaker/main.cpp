//=============================================================================
// < WAVE MAKER OBJECT v1.0 >
// Main���� [main.cpp]
// Author : �� �F��
//
//=============================================================================
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
/* ������ */
void Init(void);
/* �X�V���� */
void Update(void);
/* �t�@�C���̏����o�� */
void SetWavFile(int type);
/* �����g */
void SetSineWave(WAVE_DATA waveData, int BitRate);
/* ��`�g */
void SetSquareWave(WAVE_DATA waveData, int BitRate);
/* �̂�����g */
void SetSawtooth(WAVE_DATA waveData, int BitRate);

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
WAVEFORMATEX	wave;						// ���̍D��(wave�\����)
WAVE_DATA		waveData;					// �g�`�f�[�^
FILE			*fp;						// �t�@�C���\����							

bool			loop;						// ���[�v�p							
							
char			*work;						// �t�@�C�����[�N

short			*buf16Bit;					// 16bit�̔g�`�o�b�t�@
unsigned char	*buf8Bit;					// 8bit�̔g�`�o�b�t�@

int				fileSize;					// �t�@�C���T�C�Y															
int				waveSize;					// �g�`�T�C�Y

//=============================================================================
// ���C���֐�
//=============================================================================
int main(void)
{
	/* ������ */
	Init();
	
	/* �E�B���h�E�Y�̃��b�N�I�� */
	PlaySound("WindowsLogon", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	while (loop)
	{
		/* �X�V���� */
		Update();

		/* �Đ� */
		PlaySound("test.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	/* �E�B���h�E�Y�̃��b�N�I�t */
	PlaySound("WindowsLogoff", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	/* ���b�Z�[�W */
	system("cls");
	printf("See You Next Time");

	/* �L�[��������I�� */
	getchar();
	getchar();

	return 0;
}

//=============================================================================
// ������
//=============================================================================
void Init(void)
{
	/* ���[�v�J�n */
	loop = true;

	{/* �g�`�f�[�^ */
		waveData.samplePos		= 0;
		waveData.secPerFreq		= 0;
		waveData.secPerSample	= 0;
		waveData.volume			= 0;
		waveData.waveIndex		= 0;
		waveData.wavePos		= 0;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	int waveType;	// �g�`���

	/* �g�`��� */
	printf("0:end 1:sine 2:square 3:sawtooth \n");
	scanf_s("%d", &waveType);
	
	/* �E�B���h�E�Y�̃G���[�� */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	// �I�[�o���Ȃ��悤��
	if (waveType < WAVE_END)
	{
		waveType = WAVE_END;
	}

	// �I�[�o���Ȃ��悤��
	else if (waveType > WAVE_SAWTOOTH)
	{
		waveType = WAVE_SAWTOOTH;
	}


	// �I��
	if (waveType == WAVE_END)
	{
		loop = false;
	}

	// �g�`����
	else
	{
		/* �t�@�C���̏����o�� */
		SetWavFile(waveType);
	}
}

//=============================================================================
// �t�@�C���̏����o��
//=============================================================================
void SetWavFile(int type)
{
	int channel;	// �`�����l����
	int bitRate;	// �r�b�g���[�g
	int sf;			// �T���v�����O���g��
	int freq;		// ���g��

	/* �`�����l���� */
	printf("channel? \n");
	scanf_s("%d", &channel);
	/* �E�B���h�E�Y�̃G���[�� */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (channel < 0 || channel>CHANNEL_MAX)
	{
		printf("error \n");
		return;
	}

	/* �r�b�g���[�g */
	printf("bit rate?(8/16) \n");
	printf("1:16bit 2:8bit \n");
	scanf_s("%d", &bitRate);
	/* �E�B���h�E�Y�̃G���[�� */
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

	/* �T���v�����O���g�� */
	printf("sampling frequency? \n");
	scanf_s("%d", &sf);
	/* �E�B���h�E�Y�̃G���[�� */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (sf <0 || sf >SAMPLE_FREQ_MAX)
	{
		printf("error \n");
		return;
	}

	/* ���g�� */
	printf("frequency? \n");
	scanf_s("%d", &freq);
	/* �E�B���h�E�Y�̃G���[�� */
	PlaySound("SystemExclamation", NULL, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

	if (freq <0 || freq >FREQ_MAX)
	{
		printf("error \n");
		return;
	}

	/* �t�@�C���̏��� */
	fopen_s(&fp, FILE_NAME, "wb");

	/* wav�\���̂̏����� */
	wave.cbSize = 0;
	wave.nChannels = channel;
	wave.wBitsPerSample = bitRate;
	wave.nSamplesPerSec = sf;
	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nBlockAlign = (wave.nChannels*wave.wBitsPerSample) / 8;
	wave.nAvgBytesPerSec = wave.nSamplesPerSec*wave.nBlockAlign;

	/* �t�@�C���T�C�Y�̏����� */
	waveSize = (wave.nSamplesPerSec*(wave.wBitsPerSample / 8)) * 5;

	/* �t�@�C���T�C�Y�̏����� */
	fileSize = 44 + (waveSize);

	/* ���[�N�̃������[�m�� */
	work = (char *)malloc(fileSize);

	/* [4 byte]riff */
	memcpy(work, "RIFF", 4);

	/* [4 byte]����ȍ~�̃t�@�C���T�C�Y */
	work[4] = (fileSize - 8) >> 0 & 0xff;
	work[5] = (fileSize - 8) >> 8 & 0xff;
	work[6] = (fileSize - 8) >> 16 & 0xff;
	work[7] = (fileSize - 8) >> 24 & 0xff;

	/* [4 byte]wave */
	memcpy(work + 8, "WAVE", 4);

	/* [4 byte]fmt  */
	memcpy(work + 12, "fmt ", 4);

	/* [4 byte]�o�C�g�� */
	work[16] = 16;
	work[17] = 0;
	work[18] = 0;
	work[19] = 0;

	/* [2 byte]�t�H�[�}�b�gID */
	work[20] = 1;
	work[21] = 0;

	/* [2 byte]�`�����l���� */
	work[22] = (char)wave.nChannels;
	work[23] = 0;

	/* [4 byte]�T���v�����O���[�g */
	work[24] = wave.nSamplesPerSec >> 0 & 0xff;
	work[25] = wave.nSamplesPerSec >> 8 & 0Xff;
	work[26] = wave.nSamplesPerSec >> 16 & 0xff;
	work[27] = wave.nSamplesPerSec >> 24 & 0xff;

	/* [4 byte]�f�[�^���x(Byte/sec) �T���v�����O���g���~�`�����l�����~�ʎq���r�b�g/2*/
	work[28] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 0 & 0xff;
	work[29] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 8 & 0xff;
	work[30] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 16 & 0xff;
	work[31] = (wave.nSamplesPerSec*wave.nChannels*(wave.wBitsPerSample / 2)) >> 14 & 0xff;

	/* [2 byte]�v���b�N�T�C�Y(Byte/sample*�`�����l����) */
	work[32] = (wave.wBitsPerSample / 8)*wave.nChannels >> 0 & 0xff;
	work[33] = (wave.wBitsPerSample / 8)*wave.nChannels >> 8 & 0xff;

	/* [2 byte]�T���v�����O������̃r�b�g�� */
	work[34] = wave.wBitsPerSample >> 0 & 0xff;
	work[35] = wave.wBitsPerSample >> 8 & 0xff;

	/* [4 byte]data */
	memcpy(work + 36, "data", 4);

	/* [4 byte]�T�E���h�S�̂̃o�C�g��n */
	work[40] = waveSize >> 0 & 0xff;
	work[41] = waveSize >> 8 & 0xff;
	work[42] = waveSize >> 16 & 0xff;
	work[43] = waveSize >> 24 & 0xff;

	{/* ������ */
		waveData.secPerSample = 1.0f / wave.nSamplesPerSec;
		waveData.secPerFreq = 1.0f / freq;
	}

	/* �g�`���� */
	if (wave.wBitsPerSample == BIT_RATE_16)
	{
		buf16Bit = (short *)malloc(waveSize * sizeof(short));
	}
	else if (wave.wBitsPerSample == BIT_RATE_8)
	{
		buf8Bit = (unsigned char *)malloc(waveSize * sizeof(unsigned char));
	}

	/* �g�`�̕��� */
	switch (type)
	{
	case WAVE_SINE:
		SetSineWave(waveData,wave.wBitsPerSample);		// �����g
		break;
	case WAVE_SQUARE:
		SetSquareWave(waveData, wave.wBitsPerSample);	// ��`�g
		break;
	case WAVE_SAWTOOTH:
		SetSawtooth(waveData, wave.wBitsPerSample);		// �̂�����g
		break;
	default:
		break;
	}

	/* [n byte]�g�`��� */
	if (wave.wBitsPerSample == BIT_RATE_16)
	{
		memcpy(work + 44, buf16Bit, waveSize);
	}
	else if (wave.wBitsPerSample == BIT_RATE_8)
	{
		memcpy(work + 44, buf8Bit, waveSize);
	}

	/* �����o�� */
	fwrite(work, fileSize, 1, fp);


	{/* �܂��ˁ`���� */
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
// �����g
//=============================================================================
void SetSineWave(WAVE_DATA waveData,int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* �����g */
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
// ��`�g
//=============================================================================
void SetSquareWave(WAVE_DATA waveData, int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* ��`�g */
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
// �̂�����g
//=============================================================================
void SetSawtooth(WAVE_DATA waveData, int BitRate)
{
	for (int i = 0; i < waveSize; i++)
	{
		/* �̂�����g */
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
