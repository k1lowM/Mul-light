/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/GUI/Desktop.cpp
	概要		：	デスクトップタスク
	詳細		：	デスクトップやタスクバーの処理を担当します。
					■研究・開発のために今は外せんけど、いずれはアプリケーションにしたいと思う。■
					■タスクバーとデスクトップは分離すべきなんかなぁ。。。■
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Desktop.h"
#include	"GUI.h"
#include	"TM.h"

#include	"Image.h"
#include	"Bmp.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		//デスクトップ
		DESK_WIDTH			= 1024,
		DESK_HEIGHT			= 768,
		DESK_SIZE			= DESK_WIDTH * DESK_HEIGHT * sizeof (Color4),

		//タスクバー
		TASKBAR_WIDTH		= 1024,
		TASKBAR_HEIGHT		= 768,
		TASKBAR_SIZE		= TASKBAR_WIDTH * TASKBAR_HEIGHT * sizeof (Color4),

		//壁紙
		BG_WIDTH			= 1024,
		BG_HEIGHT			= 768,
		BG_IMAGE_SIZE		= BG_WIDTH * BG_HEIGHT * sizeof (Color4),

		//タスクバーのリソース
		//Mul lightボタン
		RSRC_MLBUTTON_WIDTH		= 32,
		RSRC_MLBUTTON_HEIGHT	= 32,
		RSRC_MLBUTTON_NUM		= 2,
		RSRC_MLBUTTON_SIZE		= RSRC_MLBUTTON_WIDTH * RSRC_MLBUTTON_HEIGHT * RSRC_MLBUTTON_NUM * sizeof (Color4),
		//タスクバー
		RSRC_TBAR_WIDTH			= 32,
		RSRC_TBAR_HEIGHT		= 32,
		RSRC_TBAR_NUM			= 1,
		RSRC_TBAR_SIZE			= RSRC_TBAR_WIDTH * RSRC_TBAR_HEIGHT * RSRC_TBAR_NUM * sizeof (Color4),
		//タスクボタン
		RSRC_TBUTTON_WIDTH		= 32,
		RSRC_TBUTTON_HEIGHT		= 32,
		RSRC_TBUTTON_NUM		= 4,
		RSRC_TBUTTON_SIZE		= RSRC_TBUTTON_WIDTH * RSRC_TBUTTON_HEIGHT * RSRC_TBUTTON_HEIGHT * sizeof (Color4),
	};
}

//タスクバーリソースのベースアドレス
#define	RSRC_MLBUTTON_BASE		( (Color4*)G_GMA.GetBase( Mui_TBRsrcGMID ) )
#define	RSRC_TBAR_BASE			( RSRC_MLBUTTON_BASE + RSRC_MLBUTTON_WIDTH * RSRC_MLBUTTON_HEIGHT * RSRC_MLBUTTON_NUM )
#define	RSRC_TBUTTON_BASE		( RSRC_TBAR_BASE + RSRC_TBAR_WIDTH * RSRC_TBAR_HEIGHT * RSRC_TBAR_NUM )

//画像のファイルパス
#define	IMG_DEFAULTWALLPAPER_FP		"/DATA/IMAGE_4.0/WALLPAPER/DEFAULTWP.BMP"				//壁紙
#define	IMG_MLBUTTON_FP				"/DATA/IMAGE_4.0/WSRSRC/TASKBAR/MLBUTTON.BMP"			//MLボタン
#define	IMG_TASKBAR_FP				"/DATA/IMAGE_4.0/WSRSRC/TASKBAR/TASKBAR.BMP"			//タスクバー
#define	IMG_TASKBUTTON_FP			"/DATA/IMAGE_4.0/WSRSRC/TASKBAR/TASKBUTTON.BMP"			//タスクボタン



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
Task*	GP_DesktopTask;		//デスクトップタスクのポインタ
Desktop	G_Desktop;

extern TM		G_TM;		//タスク管理


//デバッグ用
#include	"ATA.h"
#include	"Appli.h"
#include	"APM.h"
#include	<stdio.h>
#include	<string.h>
#include	"DM/PIT/PIT.h"
#include	"RDTM/RDTM.h"
#include	"VFS/VFS.h"
#include	"PS2Keyboard.h"
#include	"Driver.h"
#include	"DM.h"
extern PIT		G_PIT;
extern RDTM		G_RDTM;
extern ATA		G_SecATA;
extern VFS		G_VFS;
extern PS2Keyboard	G_PS2Keyboard;
extern APM		G_APM;
extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デスクトップクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デスクトップタスク
	説明	：
	Include	：	Desktop.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Desktop::Task( void )
{
	G_Desktop.Init();
	G_Desktop.Main();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デスクトップクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デスクトップクラスの初期化
	説明	：
	Include	：	Desktop.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Desktop::Init( void )
{
	u1		Au1_BmpHeadBuf[256];
//	Bmp::FileHead*			P_FileHead = (Bmp::FileHead*)Au1_BmpHeadBuf;		//BMPファイルヘッダ
//	Bmp::WinBmpInfoHead*	P_InfoHead = (Bmp::WinBmpInfoHead*)( Au1_BmpHeadBuf + sizeof (Bmp::FileHead) );	//BMP情報ヘッダ

	u1		u1_DeviceID;
	char	Ac_FilePath[256];
	DevInfo	DevInfo;

	M_MsgQ.Init();						//メッセージキュー初期化
	M_BGColor.u4_Color = 0xff123456;	//背景色設定

	//デスクトップバッファ確保
	if( !( Mui_DesktopBufGMID = G_GMA.Create( BG_IMAGE_SIZE ) ) )
		goto	_Finalize;
	//デスクトップのバッファをセット
	else
		G_GUI.SetDesktopBuffer( Mui_DesktopBufGMID );

	//タスクバーバッファ確保
	if( !( Mui_TaskBarBufGMID = G_GMA.Create( TASKBAR_SIZE ) ) )
		goto	_Finalize;
	//タスクバーのバッファをセット
	else
		G_GUI.SetTaskBarBuffer( Mui_TaskBarBufGMID );

	//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
	G_TM.DisableTS();
	//■本当は、ちゃんと排他制御を実装しなければいけない。■

	//CDデバイスを検索
	for( u1_DeviceID = 0; ; u1_DeviceID++ )
	{
		DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得

		//CDデバイスなら画像を読み取る
		if( DevInfo.P_Driver->GetDeviceType() == DT::CDD )
		{
			//ファイルヘッダ、情報ヘッダ読み込み
			sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_DEFAULTWALLPAPER_FP );
			if( G_VFS.Read( Ac_FilePath, 0, Au1_BmpHeadBuf,sizeof Au1_BmpHeadBuf ) > 0 )
				break;		//見つかれば検索終了。画像の読み取りに入る。
		}

		if( u1_DeviceID == 0xff )
		{
			DP( "Desktop::Init()\t\t\t\t\t\t\t\t[Failed]" );
			goto	_Finalize;
		}
	}

	//壁紙
	{
		//壁紙のバッファ確保
		if( !( Mui_BGImageGMID = G_GMA.Create( BG_IMAGE_SIZE ) ) )
			goto	_Finalize;

		//Bmp画像読み取り
		if( ReadBmp( Ac_FilePath, (Color4*)G_GMA.GetBase( Mui_BGImageGMID ), BG_IMAGE_SIZE ) < 0 )
		{
			DP( "Wall paper loading Failed..." );
			G_GMA.Delete( Mui_BGImageGMID );
			goto	_Finalize;
		}

		//壁紙をデスクトップバッファへ描画
		memcpy( G_GMA.GetBase( Mui_DesktopBufGMID ), G_GMA.GetBase( Mui_BGImageGMID ), DESK_SIZE );
	}

	//タスクバー
	{
		//タスクバーリソースバッファ確保
		if( !( Mui_TBRsrcGMID = G_GMA.Create( RSRC_MLBUTTON_SIZE + RSRC_TBAR_SIZE + RSRC_TBUTTON_SIZE ) ) )
			goto	_Finalize;

		//Mul lightボタン
		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_MLBUTTON_FP );
		if( ReadBmp( Ac_FilePath, RSRC_MLBUTTON_BASE, RSRC_MLBUTTON_SIZE ) < 0 )
		{
			DP( "ML button resource loading Failed..." );
			G_GMA.Delete( Mui_TBRsrcGMID );
			goto	_Finalize;
		}

		//タスクバー
		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_TASKBAR_FP );
		if( ReadBmp( Ac_FilePath, RSRC_TBAR_BASE, RSRC_TBAR_SIZE ) < 0 )
		{
			DP( "Task bar resource loading Failed..." );
			G_GMA.Delete( Mui_TBRsrcGMID );
			goto	_Finalize;
		}

		//タスクボタン
		sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_TASKBUTTON_FP );
		if( ReadBmp( Ac_FilePath, RSRC_TBUTTON_BASE, RSRC_TBUTTON_SIZE ) < 0 )
		{
			DP( "Task button resource loading Failed..." );
			G_GMA.Delete( Mui_TBRsrcGMID );
			goto	_Finalize;
		}

		//タスクバーをバッファへ描画
		{
			memset( G_GMA.GetBase( Mui_TaskBarBufGMID ), 0, TASKBAR_SIZE );
			DrawTBar();
			DrawMLButton();
		}
	}


//最終処理
_Finalize:

	//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
	G_TM.EnableTS();
	//■本当は、ちゃんと排他制御を実装しなければいけない。■

//return;
	//描画更新
	G_GUI.FullDrawUpdate();
}


/*******************************************************************************
	概要	：	デスクトップクラスのメイン
	説明	：
	Include	：	Desktop.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Desktop::Main( void )
{
	Msg		Message;
	DateTime	DateTime;
	char	c_DebugStr[128];


	while( true )
	{
		G_RDTM.GetRealDateTime( &DateTime );
		sprintf( c_DebugStr, "%04d/%02d/%02d %02d:%02d:%02d:%04d", DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay(), DateTime.GetHour(), DateTime.GetMin(), DateTime.GetSec(), DateTime.GetMSec() );
		G_GUI.DebugPrintString( c_DebugStr, 30, 1, 0xffffff00 );



		//メッセージ取得
		if( M_MsgQ.ERROR_QUEUE_NULL == M_MsgQ.Dequeue( &Message ) )
		{
			G_TM.SleepTask();
			continue;
		}


		G_GMA.ShowMemoryUsage();
//		G_TM.ShowTaskList();



		//メッセージ処理
		switch( Message.M_Message )
		{
		case Msg::KEY_PRESS:
			//デバッグ表示
			sprintf( c_DebugStr, "Msg::KEY_PRESS  :%#08x, %c    ", Message.Mu4_Data[0], (char)Message.Mu4_Data[0] );
			G_GUI.DebugPrintString( c_DebugStr, 30, 0, 0xffffffff );
			break;
		case Msg::KEY_RELEASE:
			switch( (u1)Message.Mu4_Data[0] )
			{
			case '1':
			case '2':
			case '3':
			//実験アプリを起動してみる
			{
				u1			u1_DeviceID;
				DevInfo		DevInfo;
				AppliHeader	AplHead;
				s4			s4_Return;
				char		Ac_FilePath[256];

				//CDデバイスを検索
				for( u1_DeviceID = 0; ; u1_DeviceID++ )
				{
					DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得

					//CDデバイスならアプリケーションを検索。
					if( DevInfo.P_Driver->GetDeviceType() == DT::CDD )
					{
						//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
						G_TM.DisableTS();
						//■本当は、ちゃんと排他制御を実装しなければいけない。■

						//アプリヘッドを読み取り。
						sprintf( Ac_FilePath, "/%d%s%c%s%c%s", u1_DeviceID, "/APPLICATION/SAMPLE", (u1)Message.Mu4_Data[0], "/SAMPLE", (u1)Message.Mu4_Data[0],".APL" );
						DP( "%s", Ac_FilePath )
						if( G_VFS.Read( Ac_FilePath, 0, &AplHead,sizeof AplHead ) > 0 )
						{
							//シグネチャが一致すればアプリケーション起動。
							if( !strcmp( "MLAppli\0", AplHead.Ac_Signature ) )
							{
								s4_Return = G_APM.StartAppli( Ac_FilePath );
								DP( "G_APM.StartAppli():%#x", s4_Return );
								//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
								G_TM.EnableTS();
								//■本当は、ちゃんと排他制御を実装しなければいけない。■
								break;
							}
						}
						//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
						G_TM.EnableTS();
						//■本当は、ちゃんと排他制御を実装しなければいけない。■
					}
					//発見できず。。。
					if( u1_DeviceID == 0xff )
					{
						DP( "appli not found..." );
						break;
					}
				}
				break;
			}

			case 'D':
				G_GUI.FullDrawUpdate();
				break;

			case 'M':
				G_GMA.ShowMemoryMap();
				break;

			case 'T':
				G_TM.ShowTaskList();
				break;
			}

			//デバッグ表示
			sprintf( c_DebugStr, "Msg::KEY_RELEASE:%#08x, %c    ", Message.Mu4_Data[0], (char)Message.Mu4_Data[0] );
			G_GUI.DebugPrintString( c_DebugStr, 30, 0, 0xffffffff );
			break;

		case Msg::MOUSE_MOVE:
			sprintf( c_DebugStr, "Msg::MOUSE_MOVE:X:%4d, Y:%3d  ", Message.Mu4_Data[0] >> 16, Message.Mu4_Data[0] & 0xffff );
			G_GUI.DebugPrintString( c_DebugStr, 30, 0, 0xffffffff );

//			//MLボタンにヒットしたら、光らせる
//			if(  )
//
//			//
//			else
			break;
		case Msg::OBJECT_MOUSE_OVER:
			G_GUI.DebugPrintString( "Msg::OBJECT_MOUSE_OVER        ", 30, 0, 0xffffffff );
			break;
		case Msg::OBJECT_MOUSE_OUT:
			G_GUI.DebugPrintString( "Msg::OBJECT_MOUSE_OUT         ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_L_PRESS:
			G_GUI.DebugPrintString( "Msg::MOUSE_L_PRESS            ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_L_RELEASE:
			G_GUI.DebugPrintString( "Msg::MOUSE_L_RELEASE          ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_R_PRESS:
			G_GUI.DebugPrintString( "Msg::MOUSE_R_PRESS            ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_R_RELEASE:
			G_GUI.DebugPrintString( "Msg::MOUSE_R_RELEASE          ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_C_PRESS:
			G_GUI.DebugPrintString( "Msg::MOUSE_C_PRESS            ", 30, 0, 0xffffffff );
			break;
		case Msg::MOUSE_C_RELEASE:
			G_GUI.DebugPrintString( "Msg::MOUSE_C_RELEASE          ", 30, 0, 0xffffffff );
			break;
		default:
			G_GUI.DebugPrintString( "                              ", 30, 0, 0xffffffff );
			break;
		}
	}
}


/*******************************************************************************
	概要	：	メッセージエンキュー
	説明	：
	Include	：	Desktop.h
	引数	：	const Msg& CR_Message
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Desktop::EnqueueMsg( const Msg& CR_Message )
{
	s4		s4_Return = M_MsgQ.Enqueue( CR_Message );
	G_TM.WakeupTask( GP_DesktopTask );		//タスクを起動
	return s4_Return;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デスクトップクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Bmp画像読み取り
	説明	：
	Include	：	Desktop.h
	引数	：	const char* CPc_ImageFP		ファイルパス
				Color4* P_Dest				格納先バッファ
				const ui Cui_BufSize		バッファサイズ(byte)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Desktop::ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize )
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


/*******************************************************************************
	概要	：	タスクバー描画
	説明	：
	Include	：	Desktop.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Desktop::DrawTBar( void )
{
	Color4	( *P_TaskBarBuf )[TASKBAR_WIDTH]	= (Color4(*)[TASKBAR_WIDTH])G_GMA.GetBase( Mui_TaskBarBufGMID );
	Color4	( *P_TaskBarRsrc )[RSRC_TBAR_WIDTH]	= (Color4(*)[RSRC_TBAR_WIDTH])RSRC_TBAR_BASE;
	for( ui ui_CountY = 0; ui_CountY < RSRC_TBAR_HEIGHT; ui_CountY++ )
		for( ui ui_CountX = 0; ui_CountX < TASKBAR_WIDTH; ui_CountX++ )
			if( P_TaskBarRsrc[ui_CountY][ui_CountX % RSRC_TBAR_WIDTH].u1_Alpha != 0 )
				P_TaskBarBuf[ui_CountY + TASKBAR_HEIGHT - 32][ui_CountX] = P_TaskBarRsrc[ui_CountY][ui_CountX % RSRC_TBAR_WIDTH];
}


/*******************************************************************************
	概要	：	Mul lightボタン描画
	説明	：
	Include	：	Desktop.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Desktop::DrawMLButton( void )
{
	Color4	( *P_TaskBarBuf )[TASKBAR_WIDTH]		= (Color4(*)[TASKBAR_WIDTH])G_GMA.GetBase( Mui_TaskBarBufGMID );
	Color4	( *P_MLButtonRsrc )[RSRC_TBAR_WIDTH]	= (Color4(*)[RSRC_TBAR_WIDTH])RSRC_MLBUTTON_BASE;
	for( ui ui_CountY = 0; ui_CountY < RSRC_MLBUTTON_HEIGHT; ui_CountY++ )
		for( ui ui_CountX = 0; ui_CountX < RSRC_MLBUTTON_WIDTH; ui_CountX++ )
			if( P_MLButtonRsrc[ui_CountY][ui_CountX].u1_Alpha != 0 )
				P_TaskBarBuf[ui_CountY + TASKBAR_HEIGHT - 32][ui_CountX] = P_MLButtonRsrc[ui_CountY][ui_CountX];
}


/*******************************************************************************
	概要	：	Mul lightボタンマウス処理
	説明	：
	Include	：	Desktop.h
	引数	：	Msg Message
	戻り値	：	void
*******************************************************************************/
void	Desktop::MLButtonProc( Msg Message )
{
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

