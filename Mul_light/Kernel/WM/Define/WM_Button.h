/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/Define/WM_Button.h
	概要		：	ウィンドウ管理 - ボタン関連定義ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ボタン関連 定数定義
*******************************************************************************/
enum
{
	//ボタンリソース
	RSRC_BUTTON_WIDTH		= 32,				//ボタン一つの幅
	RSRC_BUTTON_HEIGHT		= 20,				//ボタン一つの高さ
	RSRC_BUTTON_TYPE		= 4,				//ボタンの種類数
	RSRC_BUTTON_NUM			= 2,				//ボタンの重ね合わせの数
	RSRC_BUTTON_SIZE		= RSRC_BUTTON_WIDTH * RSRC_BUTTON_HEIGHT * RSRC_BUTTON_TYPE * RSRC_BUTTON_NUM * sizeof (Color4),

	//ボタン情報
	BTNFRAME_WIDTH			= 1,				//ボタンのフレーム幅
	BTNEDGE_WIDTH			= 3,				//ボタンの端の幅

	RSRC_BTN_BG_WIDTH		= 30,
	RSRC_BTN_BG_HEIGHT		= 18,
};

//ボタンフェーズ
enum
{
	PH_DEFAULT,		//通常
	PH_LIGHT,		//発光
	PH_PRESS,		//押下
	PH_SELECT,		//選択状態
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
