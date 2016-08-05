/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/Define/WM_Window.h
	概要		：	ウィンドウ管理 - ウィンドウ関連定義ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ウィンドウ関連 定数定義
*******************************************************************************/
enum
{
	//ウィンドウリソース
	RSRC_WINDOW_WIDTH		= 32,
	RSRC_WINDOW_HEIGHT		= 32,
	RSRC_WINDOW_NUM			= 2,
	RSRC_WINDOW_SIZE		= RSRC_WINDOW_WIDTH * RSRC_WINDOW_HEIGHT * RSRC_WINDOW_NUM * sizeof (Color4),
	//ウィンドウボタン３種リソース
	RSRC_BUTTON_WIDTH		= 16,
	RSRC_BUTTON_HEIGHT		= 16,
	RSRC_BUTTON_NUM			= 4,
	RSRC_BUTTON_SIZE		= RSRC_BUTTON_WIDTH * RSRC_BUTTON_HEIGHT * RSRC_BUTTON_NUM * sizeof (Color4),

	//ウィンドウ情報
	WINFRAME_WIDTH			= 3,				//ウィンドウのフレーム幅(拡縮カーソルにする領域)
	CPTBAR_EDGE_WIDTH		= WINFRAME_WIDTH,	//キャプションバーの端の幅
	CPTBAR_HEIGHT			= 23,				//キャプションバーの高さ
	WINDOW_MAX_WIDTH		= 1024,
	WINDOW_MAX_HEIGHT		= 768,
	WINDOW_MIN_WIDTH		= WINFRAME_WIDTH * 2 + 2,
	WINDOW_MIN_HEIGHT		= CPTBAR_HEIGHT + WINFRAME_WIDTH + 2,

	//ボタンの位置情報
	BUTTON_TOP				= 3,								//ウィンドウのTopからの距離
	BUTTON_RIGHT			= 4,								//ウィンドウのRightからの距離
	BUTTON_SPACING			= 3,								//ボタン間隔

	//ウィンドウの移動限界値
	WIN_LIMIT_TOP			= 0,
	WIN_LIMIT_LEFT			= 60,
	WIN_LIMIT_RIGHT			= WIN_LIMIT_LEFT,
	WIN_LIMIT_BOTTOM		= 45,
};


//ボタンタイプ
enum
{
	BUTTON_CLOSE,
	BUTTON_MAX,
	BUTTON_MIN,

	NUM_BUTTON_TYPE,
};

//ボタンフェーズ
enum
{
	PH_GRAY,		//グレー
	PH_DEFAULT,		//通常
	PH_LIGHT,		//発光
	PH_PRESS,		//押下
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
