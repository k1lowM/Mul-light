/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/GUI/GUI.h
	概要		：	描画関連ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"WM.h"

#include	<Rect.h>
#include	<Objects/Window.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	GUIクラス
*******************************************************************************/
class	GUI
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_INVALID_GMID,		//無効なGMID
		ERROR_READFAILED,		//ファイル読み取り失敗
		ERROR_INVALIDBITCOUNT,	//無効色数

		SUCCESS = _SUCCESS_,
	};

	//マウスポインタタイプ
	enum	MPT
	{
		MPT_ARROW,				//通常矢印
		MPT_MOVE,				//移動矢印
		MPT_V_ARROW,			//垂直矢印
		MPT_H_ARROW,			//水平矢印
		MPT_R_UP_ARROW,			//右上がり斜め矢印
		MPT_R_DOWN_ARROW,		//右下がり斜め矢印
		MPT_CURSOR,				//カーソル
		MPT_HAND,				//指差し

		MPT_NOCHANGE,			//変更なし
		NUM_MPT = MPT_NOCHANGE,
	};

private:
	enum
	{
		WINBUF_MAX_WIDTH		= 1024,
		WINBUF_MAX_HEIGHT		= 768,
		WINBUF_SIZE				= WINBUF_MAX_WIDTH * WINBUF_MAX_HEIGHT * sizeof (Color4),

		MOUSE_POINTER_WIDTH		= 16,		//マウスポインタ幅
		MOUSE_POINTER_HEIGHT	= 16,		//マウスポインタ高さ
	};
	enum
	{
		WINFRAME_TOP,
		WINFRAME_LEFT,
		WINFRAME_RIGHT,
		WINFRAME_BOTTOM,
		NUM_WINFRAME
	};

//変数
private:
	//基本情報
	ui		Mui_ResX;			//解像度X
	ui		Mui_ResY;			//解像度Y
	ui		Mui_BytePerDot;		//バイト/ドット
	void*	MPv_VRAM;			//VRAMアドレス
	ui		Mui_FDUFlag;		//全再描画フラグのビットスタック(0:許可、1:不許可)

	//スクリーン情報
	Color4		( *MPA_MGB )[WINBUF_MAX_WIDTH];		//メイン・グラフィックス・バッファ
	WinList*	( *MPPA_SPM )[WINBUF_MAX_WIDTH];	//スクリーン・ピクセル・マップ

	//デスクトップバッファ
	Color4	( *MPA_DesktopBuf )[WINBUF_MAX_WIDTH];		//デスクトップバッファ
	Color4	( *MPA_TaskBarBuf )[WINBUF_MAX_WIDTH];		//タスクバーバッファ

	//マウス
	ui		Mui_MouseRsrcGMID;									//マウスリソースGMID
	Color4	MA_MBB[MOUSE_POINTER_HEIGHT][MOUSE_POINTER_WIDTH];	//マウス描画用裏バッファ(Mouse Back Buffer)
	ui		Mui_MouseXOld, Mui_MouseYOld;						//マウス座標
	ui		Mui_MPT;											//マウスポインタタイプ
	u1		Mu1_MPOffset[NUM_MPT];								//マウスポインタのオフセット(7-4:x, 3-0:y)

	//ウィンドウフレーム
	Rect	M_WinFrameOld;		//ウィンドウ矩形


//メソッド
public:
	void	Init( void );		//初期化
	void	Init2( void );		//初期化2

	void	FullDrawUpdate( void );			//描画更新(すべて)
	void	DrawUpdate( Rect CopyArea );	//描画更新(部分領域)

	void	DrawMouse( ui ui_X, ui ui_Y, MPT MPT = MPT_NOCHANGE, bool b_Erase = true );	//マウス描画
	void	DrawWindowFrame( Rect WinRect, bool b_Erase = true );					//ウィンドウフレーム描画

	s4		SetDesktopBuffer( ui ui_DesktopGMID );	//デスクトップのバッファ設定
	s4		SetTaskBarBuffer( ui ui_TaskBarGMID );	//タスクバーのバッファ設定

	//ゲッター
	ui		GetResX( void );
	ui		GetResY( void );

private:
	void	VRAMTransfer( void );				//VRAM転送(すべて)
	void	VRAMTransfer( Rect CopyArea );		//VRAM転送(部分領域)

	s4		ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize );



//デバッグ用
public:
	void	DebugPrintRect( s4 s4_X, s4 s4_Y, u4 u4_Width, u4 u4_Height, u4 u4_Color );
	void	DebugPrintChar( char c_Char, u4 u4_X, u4 u4_Y, u4 u4_Color );
	void	DebugPrintString( const char* CPc_String, u4 u4_X, u4 u4_Y, u4 u4_Color );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
