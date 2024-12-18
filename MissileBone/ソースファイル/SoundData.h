// BGM/SE定義 [SoundData.h]

enum eBGM {
	BGM_TUTORIAL = 0, // チュートリアルBGM 
	BGM_GAME_1,	    // ゲームBGM 1
	BGM_GAME_2,  	// ゲームBGM 2
	BGM_GAME_3,  	// ゲームBGM 3
	BGM_TITLE,		// タイトルBGM
	BGM_STAGESELECT,		// ステージセレクトBGM
	BGM_GAMEOVER,		// ゲームオーバーBGM
	BGM_GAMECLEAR,		// ゲームクリアーBGM

	MAX_BGM
};

enum eSE {
	SE_SHOT = 0,	// 弾発射音
	SE_EXPLOSION,	// 爆発音
	SE_DEFEND,		// 衝突音
	SE_GAMEOVER,	// ゲームオーバーBGM
	SE_DECID,	    // 決定音
	SE_MENUE,	    // メニュー音
	SE_SELECT_DECID,// ステージセレクト決定音
	SE_ACCEL,// アクセル音

	MAX_SE
};
