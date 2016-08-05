/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/GUI/GUI.cpp
	概要		：	描画関連全般
	詳細		：	描画関連のすべてを司ります。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GUI.h"
#include	"TM.h"
#include	"DM.h"
#include	"VFS.h"
#include	"WM.h"
#include	<stdio.h>


//画像ライブラリ
#include	"Image.h"
#include	"Bmp.h"


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		BGI_BASE		= WORK_BASE,

		MGB_BASE		= 0x100000,		//メイン・グラフィックス・バッファのベースアドレス
		MGB_SIZE		= 0x300000,

		SPM_BASE		= 0x400000,		//スクリーン・ピクセル・マップのベースアドレス
		SPM_SIZE		= 0x300000,

		TASKBAR_BUF_SIZE	= 1024 * 4 * 32,
	};
}

//画像のファイルパス
#define	IMG_MOUSE_POINTER_FP	"/DATA/IMAGE_4.0/WSRSRC/MOUSE/POINTER.BMP"				//マウスポインタ



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	/*******************************************************************************
		起動時のグラフィックス情報構造体
		Boot Graphics Information
	*******************************************************************************/
	struct	BGI
	{
	public:
		u2		u2_ModeAttributes;			//モード属性
		u1		u1_WindowAAttributes;		//ウィンドウA属性
		u1		u1_WindowBAttributes;		//ウィンドウB属性
		u2		u2_WindowGranularity;		//ウィンドウグラニュラリティ
		u2		u2_WindowSize;				//ウィンドウサイズ
		u2		u2_WindowASegment;			//ウィンドウAセグメント
		u2		u2_WindowBSegment;			//ウィンドウBセグメント
		u4		u4_WindowFunctionPointer;	//ウィンドウ関数ポインタ
		u2		u2_BytesPerScanline;		//一ラインのバイト数
		u2		u2_XResolution;				//解像度X
		u2		u2_YResolution;				//解像度Y
		u1		u1_XCharSize;				//文字の横幅
		u1		u1_YCharSize;				//文字の高さ
		u1		u1_NumPlanes;				//メモリプレーン数
		u1		u1_BitPerDot;				//一ドットのビット数
		u1		u1_NumBanks;				//バンク数
		u1		u1_MemoryModel;				//メモリモデル
		u1		u1_BankSize;				//バンクサイズ(KiB)
		u1		u1_NumImagePages;			//イメージページ数
		u1		u1_Reserved;				//予約(VBEでは1)
		u1		u1_RedMaskSize;				//赤のビット数
		u1		u1_RedFieldPosition;		//赤のLSBの位置
		u1		u1_GreenMaskSize;			//緑のビット数
		u1		u1_GreenFieldPosition;		//緑のLSBの位置
		u1		u1_BlueMaskSize;			//青のビット数
		u1		u1_BlueFieldPosition;		//青のLSBの位置
		u1		u1_UnusedBitSize;			//不使用ビット数
		u1		u1_UnusedFieldPosition;		//不使用ビットのLSBの位置
		u1		u1_DirectColorAttributes;	//ダイレクトカラーの属性
		u4		u4_VRAMBaseAddress;			//リニアフレームバッファのベースアドレス
		u4		u4_OutVRAM;					//画面外の先頭VRAMオフセット
		u2		u2_OutVRAMSize;				//画面外のVRAMサイズ(KiB)
	};


	//マウスポインタの情報
	enum
	{
		//マウスポインタ
		RSRC_MPOINTER_WIDTH		= 16,
		RSRC_MPOINTER_HEIGHT	= 16,
		RSRC_MPOINTER_NUM		= 8,		//ポインタ、待ち、指、カーソル、縦矢印、横矢印、斜め矢印1、斜め矢印2、移動矢印、
		RSRC_MPOINTER_SIZE		= RSRC_MPOINTER_WIDTH * RSRC_MPOINTER_HEIGHT * RSRC_MPOINTER_NUM * sizeof (Color4),
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern const u1		GCu1_DebugFont[128][16];

extern TM		G_TM;
extern DM		G_DM;
extern VFS		G_VFS;
extern WM		G_WM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GUIクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	GUIクラスの初期化
	説明	：	ブート時に取得したグラフィックス情報を受けとります。
	Include	：	GUI.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GUI::Init( void )
{
	char	c_String[128];
	BGI		*P_BGI = (BGI*)BGI_BASE;		//起動時のグラフィックス情報

	//メンバ初期化、設定
	{
		Mui_ResX		= P_BGI->u2_XResolution;
		Mui_ResY		= P_BGI->u2_YResolution;
		Mui_BytePerDot	= P_BGI->u1_BitPerDot >> 3;
		MPv_VRAM		= (u1*)P_BGI->u4_VRAMBaseAddress;
		Mui_FDUFlag		= 0;		//全再描画フラグ全ビット許可

		MPA_MGB		= (Color4(*)[WINBUF_MAX_WIDTH])MGB_BASE;
		MPPA_SPM	= (WinList*(*)[WINBUF_MAX_WIDTH])SPM_BASE;

		MPA_DesktopBuf	= NULL;
		MPA_TaskBarBuf	= NULL;

		//マウスポインタ関連
		Mui_MouseRsrcGMID = NULL;
		Mui_MouseXOld	= Mui_ResX >> 1;
		Mui_MouseYOld	= Mui_ResY >> 1;
		Mui_MPT			= MPT_ARROW;			//マウスポインタタイプ = 通常矢印
		//マウスポインタのオフセット情報
		Mu1_MPOffset[MPT_ARROW]			= 0x00;
		Mu1_MPOffset[MPT_MOVE]			= 0x77;
		Mu1_MPOffset[MPT_V_ARROW]		= 0x77;
		Mu1_MPOffset[MPT_H_ARROW]		= 0x77;
		Mu1_MPOffset[MPT_R_UP_ARROW]	= 0x77;
		Mu1_MPOffset[MPT_R_DOWN_ARROW]	= 0x77;
		Mu1_MPOffset[MPT_CURSOR]		= 0x77;
		Mu1_MPOffset[MPT_HAND]			= 0x31;

		M_WinFrameOld.Init();
	}

	//グラフィックス情報表示
	sprintf( c_String, "MPv_VRAM:%#x", MPv_VRAM );
	DebugPrintString( c_String, 0, 0, 0xffffff );
	sprintf( c_String, "Resolution:%d x %d", Mui_ResX, Mui_ResY );
	DebugPrintString( c_String, 0, 1, 0xffffff );
	sprintf( c_String, "Mui_BytePerDot:%d", Mui_BytePerDot );
	DebugPrintString( c_String, 0, 2, 0xffffff );

	DP( "GUI::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	GUIクラスの初期化2
	説明	：	ブート時に取得したグラフィックス情報を受けとります。
	Include	：	GUI.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GUI::Init2( void )
{
	u1		Au1_BmpHeadBuf[256];

	u1		u1_DeviceID;
	DevInfo	DevInfo;
	char	Ac_FilePath[256];


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
			sprintf( Ac_FilePath, "/%d%s", u1_DeviceID, IMG_MOUSE_POINTER_FP );
			if( G_VFS.Read( Ac_FilePath, 0, Au1_BmpHeadBuf,sizeof Au1_BmpHeadBuf ) > 0 )
				break;		//見つかれば検索終了。画像の読み取りに入る。
		}

		if( u1_DeviceID == 0xff )
		{
			DP( "ERROR:GUI::Init2():System disc not found.." );
			goto _Error;
		}
	}

	//マウスリソースのメモリ確保
	if( !( Mui_MouseRsrcGMID = G_GMA.Create( RSRC_MPOINTER_SIZE ) ) )
		goto _Error;

	//マウスリソース読み取り
	if( ReadBmp( Ac_FilePath, (Color4*)G_GMA.GetBase( Mui_MouseRsrcGMID ), RSRC_MPOINTER_SIZE ) < 0 )
	{
		DP( "Mouse pointer resource loading Failed..." );
		goto _Error;
	}


	goto _Finalize;

//エラーがあった場合
_Error:
	G_GMA.Delete( Mui_MouseRsrcGMID );
	//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
	G_TM.EnableTS();
	//■本当は、ちゃんと排他制御を実装しなければいけない。■
	DP( "GUI::Init2()\t\t\t\t\t\t\t\t\t[Failed]" );
	return;


_Finalize:
	//■とりあえず、タスクスイッチを禁止して、正常に読み取れるようにしておく。■
	G_TM.EnableTS();
	//■本当は、ちゃんと排他制御を実装しなければいけない。■

	DP( "GUI::Init2()\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	描画更新
	説明	：	壁紙、ウィンドウ、タスクバーなどをメインバッファに描画します。
				ついでに、スクリーン・ピクセル・マップも更新。(デスクトップとタスクバーはとりあえずNULLで。)
	Include	：	GUI.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GUI::FullDrawUpdate( void )
{
	if( Mui_FDUFlag )		//全再描画禁止中。
		return;

	G_TM.DisableTS();		//タスクスイッチ禁止

	//デスクトップ描画
	{
		//描画
		__asm__
		(
				"rep	movsl;"
			:
			: "c"( Mui_ResY * Mui_ResX ), "S"( MPA_DesktopBuf ), "D"( MPA_MGB )
		);
		//スクリーン・ピクセル・マップ
		__asm__
		(
				"rep	stosl;"
			:
			: "c"( Mui_ResY * Mui_ResX ), "a"( NULL ), "D"( MPPA_SPM )
		);
	}

	//ウィンドウ描画
	{
		WinList*	P_WinList = (WinList*)G_WM.M_WinLM.GetTailList();

		//ウィンドウが一つもなければ描画しない。
		if( P_WinList != NULL )
		{
			ui		ui_CacheX, ui_CacheY;

			//後ろのウィンドウから順番に描画する。
			for( ; ; P_WinList = (WinList*)P_WinList->P_Prev )
			{
				Window*	P_Window		= P_WinList->P_Window;
				Rect	Area			= P_Window->GetArea();
				ui		ui_WinWidth		= Area.GetWidth();
				ui		ui_WinHeight	= Area.GetHeight();
				Color4	(*PA_WinBuf)[WINBUF_MAX_WIDTH]		= (Color4(*)[WINBUF_MAX_WIDTH])G_GMA.GetBase( P_WinList->ui_WinBufGMID );

				//ウィンドウ描画
				for( ui ui_CountY = 0; ui_CountY <= ui_WinHeight; ui_CountY++ )
				{
					if( ( ui_CacheY = ui_CountY + Area.Msi_Top ) >= Mui_ResY )		//画面外なら次へ。
						continue;
					for( ui ui_CountX = 0; ui_CountX <= ui_WinWidth; ui_CountX++ )
					{
						if( ( ui_CacheX = ui_CountX + Area.Msi_Left ) >= Mui_ResX || PA_WinBuf[ui_CountY][ui_CountX].u1_Alpha == 0 )		//透明なら次へ。
							continue;

						MPA_MGB[ui_CacheY][ui_CacheX] = PA_WinBuf[ui_CountY][ui_CountX];
						MPPA_SPM[ui_CacheY][ui_CacheX] = P_WinList;
					}
				}

				//最前面のウィンドウまで描画したら終了。
				if( P_WinList == (WinList*)G_WM.M_WinLM.GetHeadList() )
					break;
			}
		}
	}

	//タスクバー描画
	{
		for( ui ui_CountY = 0; ui_CountY < WINBUF_MAX_HEIGHT; ui_CountY++ )
		{
			for( ui ui_CountX = 0; ui_CountX < WINBUF_MAX_WIDTH; ui_CountX++ )
			{
				if( MPA_TaskBarBuf[ui_CountY][ui_CountX].u1_Alpha != 0 )
				{
					MPA_MGB[ui_CountY][ui_CountX] = MPA_TaskBarBuf[ui_CountY][ui_CountX];
					MPPA_SPM[ui_CountY][ui_CountX] = NULL;
				}
			}
		}
	}

	//マウスポインタ描画
	DrawMouse( G_WM.GetMouseX(), G_WM.GetMouseY(), MPT_NOCHANGE, false );

//	//デバッグ用。SPMの状況を見てみたいときに。
//	//SPMを直接描画してみるという荒技。
//	__asm__
//	(
//			"rep	movsl;"
//		:
//		: "c"( Mui_ResX * Mui_ResY ), "S"( SPM_BASE ), "D"( MGB_BASE )
//	);

	//VRAM転送
	VRAMTransfer();

	G_TM.EnableTS();		//タスクスイッチ許可
}


/*******************************************************************************
	概要	：	描画更新(部分描画)
	説明	：	壁紙、ウィンドウ、タスクバーなどをメインバッファに描画します。
				ついでに、スクリーン・ピクセル・マップも更新。
	Include	：	GUI.h
	引数	：	Rect DrawArea		描画する領域
	戻り値	：	void
*******************************************************************************/
void	GUI::DrawUpdate( Rect DrawArea )
{
	DrawArea.And( 0, 0, 1023, 767 );				//描画領域をスクリーン領域内に設定

	//エラー処理
	if( DrawArea.GetAreaSize() <= 0 )	//描画領域無し
		return;

	G_TM.DisableTS();		//タスクスイッチ禁止


	//デスクトップ描画
	{
		for( ui ui_CountY = DrawArea.Msi_Top; ui_CountY <= (ui)DrawArea.Msi_Bottom; ui_CountY++ )
		{
			for( ui ui_CountX = DrawArea.Msi_Left; ui_CountX <= (ui)DrawArea.Msi_Right; ui_CountX++ )
			{
				if( MPA_DesktopBuf[ui_CountY][ui_CountX].u1_Alpha != 0 )
				{
					MPA_MGB[ui_CountY][ui_CountX] = MPA_DesktopBuf[ui_CountY][ui_CountX];
					MPPA_SPM[ui_CountY][ui_CountX] = NULL;
				}
			}
		}
	}

	//ウィンドウ描画
	{
		WinList*	P_WinList = (WinList*)G_WM.M_WinLM.GetTailList();

		//ウィンドウが一つもなければ描画しない。
		if( P_WinList != NULL )
		{
			//後ろのウィンドウから順番に描画する。
			for( ; ; P_WinList = (WinList*)P_WinList->P_Prev )
			{
				Window*	P_Window		= P_WinList->P_Window;
				Rect	WinArea			= P_Window->GetArea();
				Color4	(*PA_WinBuf)[WINBUF_MAX_WIDTH]		= (Color4(*)[WINBUF_MAX_WIDTH])G_GMA.GetBase( P_WinList->ui_WinBufGMID );

				//ウィンドウ描画
				for( ui ui_CountY = DrawArea.Msi_Top; ui_CountY <= (ui)DrawArea.Msi_Bottom; ui_CountY++ )
				{
					for( ui ui_CountX = DrawArea.Msi_Left; ui_CountX <= (ui)DrawArea.Msi_Right; ui_CountX++ )
					{
						if( PA_WinBuf[ui_CountY - WinArea.Msi_Top][ui_CountX - WinArea.Msi_Left].u1_Alpha == 0 )		//透明なら次へ。
							continue;
						MPA_MGB[ui_CountY][ui_CountX] = PA_WinBuf[ui_CountY - WinArea.Msi_Top][ui_CountX - WinArea.Msi_Left];
						MPPA_SPM[ui_CountY][ui_CountX] = P_WinList;
					}
				}

				//最前面のウィンドウまで描画したら終了。
				if( P_WinList == (WinList*)G_WM.M_WinLM.GetHeadList() )
					break;
			}
		}
	}

	//タスクバー描画
	{
		for( ui ui_CountY = DrawArea.Msi_Top; ui_CountY <= (ui)DrawArea.Msi_Bottom; ui_CountY++ )
		{
			for( ui ui_CountX = DrawArea.Msi_Left; ui_CountX <= (ui)DrawArea.Msi_Right; ui_CountX++ )
			{
				if( MPA_TaskBarBuf[ui_CountY][ui_CountX].u1_Alpha != 0 )
				{
					MPA_MGB[ui_CountY][ui_CountX] = MPA_TaskBarBuf[ui_CountY][ui_CountX];
					MPPA_SPM[ui_CountY][ui_CountX] = NULL;
				}
			}
		}
	}

	//マウスポインタ描画
	{
		//裏バッファを更新
		ui		ui_OffsetX, ui_OffsetY;			//今回のマウスポインタ内のオフセット

		ui		ui_MouseX = G_WM.GetMouseX();
		ui		ui_MouseY = G_WM.GetMouseY();
		ui		ui_CacheX, ui_CacheY;


		//オフセットを設定
		ui_OffsetX = Mu1_MPOffset[Mui_MPT] >> 4;
		ui_OffsetY = Mu1_MPOffset[Mui_MPT] & 0xf;

		for( ui ui_CountY = 0; ui_CountY < MOUSE_POINTER_HEIGHT; ui_CountY++ )
			if( ( ui_CacheY = ui_MouseY + ui_CountY - ui_OffsetY ) < Mui_ResY )		//画面外なら次へ。
				for( ui ui_CountX = 0; ui_CountX < MOUSE_POINTER_WIDTH; ui_CountX++ )
					if( DrawArea.CheckInArea( ui_CacheX = ui_MouseX + ui_CountX - ui_OffsetX, ui_CacheY ) )
//						&& ( ui_CacheX = ui_MouseX + ui_CountX - ui_OffsetX ) < Mui_ResX )		//画面外なら次へ。
						MA_MBB[ui_CountY][ui_CountX] = MPA_MGB[ui_CacheY][ui_CacheX];
		DrawMouse( G_WM.GetMouseX(), G_WM.GetMouseY(), MPT_NOCHANGE, true );
	}

	//VRAM転送
	VRAMTransfer( DrawArea );

	G_TM.EnableTS();		//タスクスイッチ許可
}


/*******************************************************************************
	概要	：	マウス描画
	説明	：
	Include	：	GUI.h
	引数	：	ui ui_X		マウスを描画するX座標
				ui ui_Y		マウスを描画するY座標
				[MPT MPT = MPT_NOCHANGE]		マウスポインタタイプ
				[bool b_Erase = true]			前マウス消去フラグ(true:消去)
	戻り値	：	void
*******************************************************************************/
void	GUI::DrawMouse( ui ui_X, ui ui_Y, MPT MPT, bool b_Erase )
{
	G_TM.DisableTS();		//タスクスイッチ禁止

	Color4	( *PA_MouseRsrc )[MOUSE_POINTER_WIDTH] = (Color4(*)[MOUSE_POINTER_WIDTH])G_GMA.GetBase( Mui_MouseRsrcGMID );
	ui		ui_OffsetX, ui_OffsetY;			//今回のマウスポインタ内のオフセット
	ui		ui_OffsetXOld, ui_OffsetYOld;	//前回のマウスポインタ内のオフセット

	//マウスポインタ内のオフセットとマウスポインタタイプを設定
	{
		//前回のオフセットを設定
		ui_OffsetXOld = Mu1_MPOffset[Mui_MPT] >> 4;
		ui_OffsetYOld = Mu1_MPOffset[Mui_MPT] & 0xf;

		//マウスポインタ"変更なし"でなければマウスポインタタイプを変更
		if( MPT != MPT_NOCHANGE )
			Mui_MPT = MPT;		//マウスポインタタイプを変更
		PA_MouseRsrc += MOUSE_POINTER_HEIGHT * Mui_MPT;		//マウスポインタタイプによって、描画するポインタを変更

		//オフセットを設定
		ui_OffsetX = Mu1_MPOffset[Mui_MPT] >> 4;
		ui_OffsetY = Mu1_MPOffset[Mui_MPT] & 0xf;
	}

	//裏バッファでポインタを消す
	if( b_Erase )		//消去フラグがtrueのとき
	{
		ui		ui_CacheX, ui_CacheY;
		for( ui ui_CountY = 0; ui_CountY < MOUSE_POINTER_HEIGHT; ui_CountY++ )
			if( ( ui_CacheY = Mui_MouseYOld + ui_CountY - ui_OffsetYOld ) < Mui_ResY )		//画面外なら次へ。
				for( ui ui_CountX = 0; ui_CountX < MOUSE_POINTER_WIDTH; ui_CountX++ )
					if( ( ui_CacheX = Mui_MouseXOld + ui_CountX - ui_OffsetXOld ) < Mui_ResX )		//画面外なら次へ。
						MPA_MGB[ui_CacheY][ui_CacheX] = MA_MBB[ui_CountY][ui_CountX];
	}

	//裏バッファを更新 & ポインタ描画
	{
		ui		ui_CacheX, ui_CacheY;
		for( ui ui_CountY = 0; ui_CountY < MOUSE_POINTER_HEIGHT; ui_CountY++ )
		{
			if( ( ui_CacheY = ui_Y + ui_CountY - ui_OffsetY ) >= Mui_ResY )		//画面外なら次へ。
				continue;
			for( ui ui_CountX = 0; ui_CountX < MOUSE_POINTER_WIDTH; ui_CountX++ )
			{
				if( ( ui_CacheX = ui_X + ui_CountX - ui_OffsetX ) >= Mui_ResX )		//画面外なら次へ。
					continue;
				MA_MBB[ui_CountY][ui_CountX] = MPA_MGB[ui_CacheY][ui_CacheX];
				if( PA_MouseRsrc[ui_CountY][ui_CountX].u1_Alpha != 0 )		//透明でなければ描画
					MPA_MGB[ui_CacheY][ui_CacheX] = PA_MouseRsrc[ui_CountY][ui_CountX];
			}
		}
	}

	//VRAM転送
	{
		Rect	DrawArea;
		if( b_Erase )
		{
			DrawArea.SetRect( Mui_MouseYOld - ui_OffsetYOld, Mui_MouseXOld - ui_OffsetXOld, Mui_MouseXOld + MOUSE_POINTER_WIDTH - ui_OffsetXOld, Mui_MouseYOld + MOUSE_POINTER_HEIGHT - ui_OffsetYOld );
			VRAMTransfer( DrawArea );
		}
		DrawArea.SetRect( ui_Y - ui_OffsetY, ui_X - ui_OffsetX, ui_X + MOUSE_POINTER_WIDTH - ui_OffsetX, ui_Y + MOUSE_POINTER_HEIGHT - ui_OffsetY );
		VRAMTransfer( DrawArea );
	}

	Mui_MouseXOld = ui_X;
	Mui_MouseYOld = ui_Y;

	G_TM.EnableTS();		//タスクスイッチ許可
}


/*******************************************************************************
	概要	：	ウィンドウフレーム描画
	説明	：
	Include	：	GUI.h
	引数	：	Rect WinRect			フレームを描画する矩形
				[bool b_Erase = true]	前フレーム消去フラグ(true:消去)
	戻り値	：	void
*******************************************************************************/
void	GUI::DrawWindowFrame( Rect WinRect, bool b_Erase )
{
	G_TM.DisableTS();		//タスクスイッチ禁止

	//前回のフレームを削除(VRAM上のフレームを上書き)
	if( b_Erase )		//消去フラグがtrueのとき
	{
		Rect	DrawArea;
		DrawArea = M_WinFrameOld;

		//上
		DrawArea.Msi_Bottom	= M_WinFrameOld.Msi_Top;
		if( (ui)DrawArea.Msi_Top < Mui_ResY )
			VRAMTransfer( DrawArea );
		//左
		DrawArea.Msi_Right	= M_WinFrameOld.Msi_Left;
		DrawArea.Msi_Bottom	= M_WinFrameOld.Msi_Bottom;
		if( (ui)DrawArea.Msi_Left < Mui_ResX )
			VRAMTransfer( DrawArea );
		//右
		DrawArea.Msi_Left	= M_WinFrameOld.Msi_Right;
		DrawArea.Msi_Right	= M_WinFrameOld.Msi_Right;
		if( (ui)DrawArea.Msi_Right < Mui_ResX )
			VRAMTransfer( DrawArea );
		//下
		DrawArea.Msi_Top	= M_WinFrameOld.Msi_Bottom;
		DrawArea.Msi_Left	= M_WinFrameOld.Msi_Left;
		if( (ui)DrawArea.Msi_Bottom < Mui_ResY )
			VRAMTransfer( DrawArea );
	}

	//フレーム描画
	if( Mui_BytePerDot == 3 )
	{
		ui		ui_YCC, ui_CC;

		//上
		if( (ui)WinRect.Msi_Top < Mui_ResY )
		{
			ui_YCC = WinRect.Msi_Top * Mui_ResX;
			for( ui ui_CountX = WinRect.Msi_Left + 1; (si)ui_CountX < WinRect.Msi_Right; ui_CountX++ )
			{
				if( ui_CountX >= Mui_ResX )		//画面外なら次へ。
					continue;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Blue	= ~MPA_MGB[WinRect.Msi_Top][ui_CountX].u1_Blue;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Green	= ~MPA_MGB[WinRect.Msi_Top][ui_CountX].u1_Green;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Red	= ~MPA_MGB[WinRect.Msi_Top][ui_CountX].u1_Red;
			}
		}
		//下
		if( (ui)WinRect.Msi_Bottom < Mui_ResY )
		{
			ui_YCC = WinRect.Msi_Bottom * Mui_ResX;
			for( ui ui_CountX = WinRect.Msi_Left + 1; (si)ui_CountX < WinRect.Msi_Right; ui_CountX++ )
			{
				if( ui_CountX >= Mui_ResX )		//画面外なら次へ。
					continue;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Blue	= ~MPA_MGB[WinRect.Msi_Bottom][ui_CountX].u1_Blue;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Green	= ~MPA_MGB[WinRect.Msi_Bottom][ui_CountX].u1_Green;
				( (Color3*)MPv_VRAM )[ui_YCC + ui_CountX].u1_Red	= ~MPA_MGB[WinRect.Msi_Bottom][ui_CountX].u1_Red;
			}
		}
		//左
		if( (ui)WinRect.Msi_Left < Mui_ResX )
		{
			for( ui ui_CountY = WinRect.Msi_Top; (si)ui_CountY <= WinRect.Msi_Bottom; ui_CountY++ )
			{
				if( ui_CountY >= Mui_ResY )		//画面外なら次へ。
					continue;
				ui_CC = ui_CountY * Mui_ResX + WinRect.Msi_Left;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Blue	= ~MPA_MGB[ui_CountY][WinRect.Msi_Left].u1_Blue;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Green	= ~MPA_MGB[ui_CountY][WinRect.Msi_Left].u1_Green;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Red		= ~MPA_MGB[ui_CountY][WinRect.Msi_Left].u1_Red;
			}
		}
		//右
		if( (ui)WinRect.Msi_Right < Mui_ResX )
		{
			for( ui ui_CountY = WinRect.Msi_Top; (si)ui_CountY <= WinRect.Msi_Bottom; ui_CountY++ )
			{
				if( ui_CountY >= Mui_ResY )		//画面外なら次へ。
					continue;
				ui_CC = ui_CountY * Mui_ResX + WinRect.Msi_Right;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Blue	= ~MPA_MGB[ui_CountY][WinRect.Msi_Right].u1_Blue;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Green	= ~MPA_MGB[ui_CountY][WinRect.Msi_Right].u1_Green;
				( (Color3*)MPv_VRAM )[ui_CC].u1_Red		= ~MPA_MGB[ui_CountY][WinRect.Msi_Right].u1_Red;
			}
		}
	}
	else if( Mui_BytePerDot == 4 )
	{
		ui		ui_YCC;

		//上
		if( (ui)WinRect.Msi_Top < Mui_ResY )
		{
			ui_YCC = WinRect.Msi_Top * Mui_ResX;
			for( ui ui_CountX = WinRect.Msi_Left + 1; (si)ui_CountX < WinRect.Msi_Right; ui_CountX++ )
				if( ui_CountX < Mui_ResX )
					( (Color4*)MPv_VRAM )[ui_YCC + ui_CountX].u4_Color = ~MPA_MGB[WinRect.Msi_Top][ui_CountX].u4_Color;
		}
		//下
		if( (ui)WinRect.Msi_Bottom < Mui_ResY )
		{
			ui_YCC = WinRect.Msi_Bottom * Mui_ResX;
			for( ui ui_CountX = WinRect.Msi_Left + 1; (si)ui_CountX < WinRect.Msi_Right; ui_CountX++ )
				if( ui_CountX < Mui_ResX )
					( (Color4*)MPv_VRAM )[ui_YCC + ui_CountX].u4_Color = ~MPA_MGB[WinRect.Msi_Top][ui_CountX].u4_Color;
		}
		//左
		if( (ui)WinRect.Msi_Left < Mui_ResX )
			for( ui ui_CountY = WinRect.Msi_Top; (si)ui_CountY <= WinRect.Msi_Bottom; ui_CountY++ )
				if( ui_CountY < Mui_ResY )
					( (Color4*)MPv_VRAM )[ui_CountY * Mui_ResX + WinRect.Msi_Left].u4_Color = ~MPA_MGB[ui_CountY][WinRect.Msi_Left].u4_Color;
		//右
		if( (ui)WinRect.Msi_Right < Mui_ResX )
			for( ui ui_CountY = WinRect.Msi_Top; (si)ui_CountY <= WinRect.Msi_Bottom; ui_CountY++ )
				if( ui_CountY < Mui_ResY )
					( (Color4*)MPv_VRAM )[ui_CountY * Mui_ResX + WinRect.Msi_Right].u4_Color = ~MPA_MGB[ui_CountY][WinRect.Msi_Right].u4_Color;
	}
	else
		return;

	M_WinFrameOld = WinRect;

	G_TM.EnableTS();		//タスクスイッチ許可
}


/*******************************************************************************
	概要	：	デスクトップのバッファ設定
	説明	：	デスクトップのバッファを登録します。
	Include	：	GUI.h
	引数	：	ui ui_DesktopGMID		デスクトップのバッファのメモリID(グローバル)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GUI::SetDesktopBuffer( ui ui_DesktopGMID )
{
	MPA_DesktopBuf = (Color4(*)[WINBUF_MAX_WIDTH])G_GMA.GetBase( ui_DesktopGMID );
	return SUCCESS;
}


/*******************************************************************************
	概要	：	タスクバーのバッファ設定
	説明	：	タスクバーのバッファを登録します。
	Include	：	GUI.h
	引数	：	ui ui_TaskBarGMID		デスクトップのバッファのメモリID(グローバル)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GUI::SetTaskBarBuffer( ui ui_TaskBarGMID )
{
	MPA_TaskBarBuf = (Color4(*)[WINBUF_MAX_WIDTH])G_GMA.GetBase( ui_TaskBarGMID );
	return SUCCESS;
}


/*******************************************************************************
	概要	：	横方向解像度取得
	説明	：
	Include	：	GUI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		GUI::GetResX( void )
{
	return Mui_ResX;
}


/*******************************************************************************
	概要	：	縦方向解像度取得
	説明	：
	Include	：	GUI.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		GUI::GetResY( void )
{
	return Mui_ResY;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GUIクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	VRAM転送(すべて)
	説明	：	メイン・グラフィックス・バッファからVRAMへ転送
	Include	：	GUI.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GUI::VRAMTransfer( void )
{
	switch( Mui_BytePerDot )
	{
	case 3:
		__asm__
		(
				"movb	(%%ebx),	%%al;"
			"_TransferLoop:"
				"movsl;"					//4Byte転送
				"decl	%%edi;"				//バッファとVRAMのByte/Dotのギャップを埋める。
				"loop	_TransferLoop;"
				"movb	%%al,		(%%ebx);"
			:
			: "c"( Mui_ResX * Mui_ResY ), "S"( MGB_BASE ), "D"( MPv_VRAM ), "b"( (u1*)MPv_VRAM + Mui_ResX * Mui_ResY * Mui_BytePerDot )		//バッファとVRAMのギャップ, 総ドット数, ソース(バッファ), ディスティネーション(VRAM)
			: "%al"
		);
		return;
	case 4:
		__asm__
		(
				"rep	movsl;"				//4Byte転送
			:
			: "c"( Mui_ResX * Mui_ResY ), "S"( MGB_BASE ), "D"( MPv_VRAM )		//バッファとVRAMのギャップ, 総ドット数, ソース(バッファ), ディスティネーション(VRAM)
		);
		return;
	default:
		return;
	}
}


/*******************************************************************************
	概要	：	VRAM転送(部分領域)
	説明	：	メイン・グラフィックス・バッファからVRAMへ転送
	Include	：	GUI.h
	引数	：	Rect CopyArea		バッファからVRAMへ転送する領域
	戻り値	：	void
*******************************************************************************/
void	GUI::VRAMTransfer( Rect CopyArea )
{
	CopyArea.And( 0, 0, 1023, 767 );				//描画領域をスクリーン領域内に設定

//	//エラー処理
//	if( CopyArea.GetAreaSize() <= 0 )	//転送領域無し
//		return;

	//転送
	{
		Color4*	P_MGB;
		s4		s4_CountX;
		s4		s4_CountY;
		s4		s4_CC;		//カウント・キャッシュ

		switch( Mui_BytePerDot )
		{
		case 3:
		{
			Color3*		P_VRAM;
			for( s4_CountY = CopyArea.Msi_Top; s4_CountY <= CopyArea.Msi_Bottom; s4_CountY++ )
			{
				s4_CC	= s4_CountY * Mui_ResX;
				P_VRAM	= (Color3*)MPv_VRAM + s4_CC;
				P_MGB	= (Color4*)MGB_BASE + s4_CC;
				for( s4_CountX = CopyArea.Msi_Left; s4_CountX <= CopyArea.Msi_Right; s4_CountX++ )
				{
					P_VRAM[s4_CountX].u1_Red	= P_MGB[s4_CountX].u1_Red;
					P_VRAM[s4_CountX].u1_Green	= P_MGB[s4_CountX].u1_Green;
					P_VRAM[s4_CountX].u1_Blue	= P_MGB[s4_CountX].u1_Blue;
				}
			}
			return;
		}
		case 4:
		{
			Color4*		P_VRAM;
			for( s4_CountY = CopyArea.Msi_Top; s4_CountY <= CopyArea.Msi_Bottom; s4_CountY++ )
			{
				s4_CC	= s4_CountY * Mui_ResX;
				P_VRAM	= (Color4*)MPv_VRAM + s4_CC;
				P_MGB	= (Color4*)MGB_BASE + s4_CC;
				for( s4_CountX = CopyArea.Msi_Left; s4_CountX <= CopyArea.Msi_Right; s4_CountX++ )
					P_VRAM[s4_CountX]	= P_MGB[s4_CountX];
			}
			return;
		}
		default:
			return;
		}
	}
}


/*******************************************************************************
	概要	：	Bmp画像読み取り
	説明	：
	Include	：	GUI.h
	引数	：	const char* CPc_ImageFP		ファイルパス
				Color4* P_Dest				格納先バッファ
				const ui Cui_BufSize		バッファサイズ(byte)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GUI::ReadBmp( const char* CPc_ImageFP, Color4* P_Dest, const ui Cui_BufSize )
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

