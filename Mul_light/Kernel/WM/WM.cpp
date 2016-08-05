/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/WM/WM.cpp
	概要		：	ウィンドウ管理
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"WM.h"
#include	"GMQ.h"
#include	"DM.h"
#include	"VFS.h"
#include	"APM.h"

#include	"Define/WM_Window.h"

//画像ライブラリ
#include	"Image.h"
#include	"Bmp.h"

#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	//マウスのボタン情報
	enum
	{
		MOUSE_LEFT_BUTTON		= 0x01,			//左ボタン
		MOUSE_RIGHT_BUTTON		= 0x02,			//右ボタン
		MOUSE_CENTER_BUTTON		= 0x04,			//中央ボタン
		MOUSE_BUTTON_MASK		= 0x07,			//ボタンマスク
	};

	//スクリーン・ピクセル・マップ
	//スクリーンのどのピクセルにどのウィンドウ(デスクトップやタスクバー含む)が
	//	配置されているかを格納するビットマップのようなもの。)
	enum
	{
		SPM_BASE		= 0x400000,
		SPM_SIZE		= 0x300000,
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
Task*	GP_WMTask;			//ウィンドウ管理タスク
GMQ		G_GMQ;				//グローバル・メッセージ・キュー

extern WM		G_WM;
extern TM		G_TM;
extern DM		G_DM;
extern VFS		G_VFS;
extern APM		G_APM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウ管理タスク
	説明	：
	Include	：	WM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	WM::Task( void )
{
	G_WM.Init();
	G_WM.Main();

	while( true )
		G_TM.SleepTask();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウ管理クラスの初期化
	説明	：
	Include	：	WM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	WM::Init( void )
{
	u1		Au1_BmpHeadBuf[256];		//bmpヘッダバッファ

	u1		u1_DeviceID;				//デバイスID
	DevInfo	DevInfo;					//デバイス情報
	char	Ac_FilePath[256];			//ファイルパス

	//メンバ初期化
	{
		//リスト管理初期化
		M_WinLM.Init();

		MPPA_SPM		= (WinList*(*)[WINBUF_MAX_WIDTH])SPM_BASE;

		Mui_WinRsrcGMID	= 0;		//ウィンドウリソースGMID
		Mui_BtnRsrcGMID	= 0;		//ボタンリソースGMID
		MP_Focus		= NULL;		//フォーカスウィンドウリスト
		MP_FocusObj		= NULL;		//フォーカスオブジェクト
		M_Flags.Init( WINDOW_RESIZE_DISABLE | WINDOW_MOVE_DISABLE );

		//マウス情報
		Mui_MouseX		= G_GUI.GetResX() >> 1;	//マウス座標X
		Mui_MouseY		= G_GUI.GetResY() >> 1;	//マウス座標Y
		Mu4_MouseButton	= 0;					//マウスボタンフラグ
		MP_MOW			= NULL;					//マウスが重なったウィンドウリスト
		MP_MOO			= NULL;					//マウスが重なったオブジェクトのポインタ
		M_MOOArea.Init();						//MOOの有効領域
		Mu1_WinBtnNo	= NUM_BUTTON_TYPE;		//ウィンドウボタン番号
	}

	G_GMQ.Init();		//グローバル・メッセージ・キュー初期化


	G_TM.DisableTS();		//タスクスイッチ禁止

	//CDデバイスを検索
	for( u1_DeviceID = 0; ; u1_DeviceID++ )
	{
		DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得

		//CDデバイスなら画像を読み取る
		if( DevInfo.P_Driver->GetDeviceType() == DT::CDD )
		{
			//ファイルヘッダ、情報ヘッダ読み込み
			sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_WINDOW_FP );
			if( G_VFS.Read( Ac_FilePath, 0, Au1_BmpHeadBuf,sizeof Au1_BmpHeadBuf ) > 0 )
				break;		//見つかれば検索終了。画像の読み取りに入る。
		}

		if( u1_DeviceID == 0xff )
		{
			G_TM.EnableTS();		//タスクスイッチ許可
			DP( "WM::Init()\t\t\t\t\t\t\t\t\t[Failed]" );
			return;		//システムのディスクが見つからない。
		}
	}

	//以下、システムディスクが見つかった場合の処理
	G_TM.EnableTS();		//タスクスイッチ許可

	//各種初期化
	InitWindow( u1_DeviceID );			//ウィンドウ関連初期化
	InitButton( u1_DeviceID );			//ボタン関連初期化

	DP( "WM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
	return;
}


/*******************************************************************************
	概要	：	ウィンドウ管理のメイン
	説明	：	GMQ(グローバル・メッセージ・キュー)からデータを取り出し、
				各ウィンドウなどにメッセージを送ります。
	Include	：	WM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	WM::Main( void )
{
	Msg			Message;		//メッセージ格納バッファ

	//休止
_Sleep:
	G_TM.SleepTask();

	while( true )
	{
		//メッセージを取得。エラーがあれば休止。
		if( G_GMQ.Dequeue( &Message ) < 0 )
			goto _Sleep;

		//メッセージ送信処理
		SendMessage( Message );
		continue;		//もう一度実行。
	}
}


/*******************************************************************************
	概要	：	ウィンドウ登録
	説明	：	LDTを使ったアプリケーションがシステムコールを通して使う関数です。
				カーネルは使用禁止！
	Include	：	WM.h
	引数	：	ui ui_AppliID			アプリケーションID
				Window* P_Window		ウィンドウポインタ(ローカルポインタ)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		WM::RegiWindow( ui ui_AppliID, Window* P_Window )
{
	WinList*	P_WinList;			//ウィンドウリストポインタ
	ui		ui_WinListTempKMID;		//ウィンドウリストクラスの一時的なメモリID
	Appli*	P_Appli = G_APM.GetAppli( ui_AppliID );	//アプリケーションクラスポインタ
	void*	Pv_LocalBase = P_Appli->GetLocalBase();	//ローカルのベースアドレス

	//エラー処理
	if( P_Appli == NULL )			//NULLポインタエラー
		return ERROR_NULLPOINTER;

	P_Window = (Window*)KStdLib::ConvL2K( P_Window, Pv_LocalBase );

	//ウィンドウリストのメモリ領域確保
	if( !( ui_WinListTempKMID = G_KMA.Create( sizeof (WinList) ) ) )
		return ERROR_MEMORYALLOC;

	//ウィンドウリスト構造体の設定
	P_WinList = (WinList*)G_KMA.GetBase( ui_WinListTempKMID );	//ウィンドウリスト本体のアドレス設定
	P_WinList->ui_WinListKMID	= ui_WinListTempKMID;			//ウィンドウリストのメモリ領域IDを格納
	P_WinList->P_Window			= P_Window;						//ウィンドウポインタを設定
	P_WinList->ui_WinBufGMID	= 0;							//ウィンドウバッファメモリID
	P_WinList->ui_AppliID		= ui_AppliID;					//アプリケーションID
	P_WinList->Pv_LocalBase		= Pv_LocalBase;					//ローカルのベースアドレス

	//ウィンドウバッファ確保
	if( !( P_WinList->ui_WinBufGMID = G_GMA.Create( WINBUF_SIZE ) ) )
	{
		G_KMA.Delete( ui_WinListTempKMID );		//ウィンドウリストのメモリ領域削除
		return ERROR_MEMORYALLOC;
	}

	//ウィンドウリストのアドレスを設定
	P_Window->SetKWinList( P_WinList );

	//ウィンドウリストに結合(先頭:一番手前に)
	M_WinLM.JoinHead( P_WinList );

	//フォーカス変更
	MP_Focus	= P_WinList;
	MP_FocusObj	= (Object*)KStdLib::ConvL2K( P_WinList->P_Window, Pv_LocalBase );

	//Msg::INITIALIZE送信
	{
		Msg		Message;
		Message.Init( Msg::INITIALIZE );
		P_Window->SendMessage( Message );
	}

DP( "WM::RegiWindow():SUCCESS" );
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ウィンドウ削除
	説明	：
	Include	：	WM.h
	引数	：	ui ui_AppliID			削除するウィンドウのアプリケーションID
	戻り値	：	void
*******************************************************************************/
void	WM::DeleteWindow( ui ui_AppliID )
{
	//ウィンドウバッファの開放
	for( WinList* P_WinList = (WinList*)M_WinLM.GetHeadList(); M_WinLM.GetHeadList() != NULL; P_WinList = (WinList*)P_WinList->P_Next )
	{
		//同じアプリケーションIDのウィンドウリストがあれば、ウィンドウリストごと削除
		if( P_WinList->ui_AppliID == ui_AppliID )
		{
			M_WinLM.Split( P_WinList );						//ウィンドウリストを切り離す。
			G_GMA.Delete( P_WinList->ui_WinBufGMID );		//ウィンドウバッファ削除
			G_KMA.Delete( P_WinList->ui_WinListKMID );		//ウィンドウリスト自身を削除
		}

		//削除するウィンドウがマウス座標のウィンドウなら、NULLに設定。
		if( MP_MOW == P_WinList )
			MP_MOW = NULL;

		//最後まで検索したら終了
		if( P_WinList == M_WinLM.GetTailList() )
			break;
	}

	G_GUI.FullDrawUpdate();
}


/*******************************************************************************
	概要	：	各種ゲッター
	説明	：	メンバを返すだけなので、説明省略。
	Include	：	WM.h
*******************************************************************************/
ui		WM::GetMouseX( void )	{ return Mui_MouseX; }		//マウスX座標
ui		WM::GetMouseY( void )	{ return Mui_MouseY; }		//マウスY座標



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デスクトップクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ローカルのベースアドレス取得
	説明	：	ウィンドウクラスのポインタから、ローカルのベースアドレスを取得します。
	Include	：	WM.h
	引数	：	Window* P_Window		ウィンドウクラスのポインタ(カーネルアドレス)
	戻り値	：	void*		ローカルのベースアドレス
*******************************************************************************/
void*	WM::GetLocalBase( Window* P_Window )
{
	return G_APM.GetAppli( P_Window->GetAppliID() )->GetLocalBase();
}


/*******************************************************************************
	概要	：	Bmp画像読み取り
	説明	：
	Include	：	WM.h
	引数	：	const char* CPc_ImageFP		ファイルパス
				Color4* P_Dest				格納先バッファ
				const ui Cui_BufSize		バッファサイズ(byte)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		WM::ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize )
{
	Bmp::FileHead		FileHead;		//BMPファイルヘッダ
	Bmp::WinBmpInfoHead	InfoHead;		//BMP情報ヘッダ
	const ui		Cui_BufNum = Cui_BufSize >> 2;

	//画像読み取り
	if( Bmp::Read( CPc_ImageFP, P_Dest, Cui_BufSize, &FileHead, &InfoHead ) < 0 )
		return ERROR_READFAILED;

	//色数変換
	switch( InfoHead.u2_BitCount )
	{
	case 24:
		Image::Color3to4( P_Dest, (Color3*)P_Dest, Cui_BufNum );
		break;
	case 32:
		break;
	default:
		return ERROR_INVALIDBITCOUNT;
	}

	//上下反転
	if( InfoHead.s4_Height > 0 )
		Image::VFlip( P_Dest, InfoHead.s4_Width, InfoHead.s4_Height );

	//透明色処理
	Image::TransparentColorProcessing( P_Dest, Cui_BufNum );

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

