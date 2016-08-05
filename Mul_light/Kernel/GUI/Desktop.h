/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/GUI/Desktop.h
	概要		：	デスクトップタスク
	詳細		：	デスクトップやタスクバーの処理を担当します。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	<Message.h>
#include	<MsgQ.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	Desktopクラス
*******************************************************************************/
class	Desktop
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_READFAILED,			//ファイル読み取り失敗
		ERROR_INVALIDBITCOUNT,		//無効色数

		SUCCESS = _SUCCESS_,
	};

//変数
private:
	MsgQ	M_MsgQ;				//メッセージキュー

	ui		Mui_DesktopBufGMID;	//デスクトップバッファのメモリID
	ui		Mui_TaskBarBufGMID;	//タスクバーバッファのメモリID

	ui		Mui_BGImageGMID;	//壁紙のメモリID(0なら背景色で埋める)
	ui		Mui_TBRsrcGMID;		//MLボタン(32*64)、タスクバー(32*32)、タスクボタン(32*128)

	Color4	M_BGColor;			//背景色

//メソッド
public:
	static void	Task( void );

	void	Init( void );		//初期化
	void	Main( void );		//メイン

	s4		EnqueueMsg( const Msg& CR_Message );

private:
	s4		ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize );

	void	DrawTBar( void );
	void	DrawMLButton( void );

	void	MLButtonProc( Msg Message );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
