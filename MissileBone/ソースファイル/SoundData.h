// BGM/SE��` [SoundData.h]

enum eBGM {
	BGM_TUTORIAL = 0, // �`���[�g���A��BGM 
	BGM_GAME_1,	    // �Q�[��BGM 1
	BGM_GAME_2,  	// �Q�[��BGM 2
	BGM_GAME_3,  	// �Q�[��BGM 3
	BGM_TITLE,		// �^�C�g��BGM
	BGM_STAGESELECT,		// �X�e�[�W�Z���N�gBGM
	BGM_GAMEOVER,		// �Q�[���I�[�o�[BGM
	BGM_GAMECLEAR,		// �Q�[���N���A�[BGM

	MAX_BGM
};

enum eSE {
	SE_SHOT = 0,	// �e���ˉ�
	SE_EXPLOSION,	// ������
	SE_DEFEND,		// �Փˉ�
	SE_GAMEOVER,	// �Q�[���I�[�o�[BGM
	SE_DECID,	    // ���艹
	SE_MENUE,	    // ���j���[��
	SE_SELECT_DECID,// �X�e�[�W�Z���N�g���艹
	SE_ACCEL,// �A�N�Z����

	MAX_SE
};
