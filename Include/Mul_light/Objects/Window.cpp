/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Window.cpp
	概要		：	Mul light API - ウィンドウクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

#include	"Window.h"
#include	"Text.h"

#include	"MLStdLib.h"
#include	"SC.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		//ウィンドウのパディング情報
		WINPAD_TOP		= 23,
		WINPAD_LEFT		= 3,
		WINPAD_RIGHT	= 3,
		WINPAD_BOTTOM	= 3,
	};

	enum
	{
		TASK_BAR_HEIGHT	= 0x22,		//タスクバーの高さ
	};
}


//パーセント単位のサイズをピクセル単位のサイズに変換(100％が65536として計算。)
#define	SIZE_PER2PX( _PERCENT_, _MAX_PX_ )		( ( ( _PERCENT_ ) * ( _MAX_PX_ ) ) >> 16 )



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウクラスの初期化
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Window::Init( void )
{
	Caption::Init();
	Object::SetObjectType( WINDOW );
	Object::MP_Window = this;
	Object::SetOffset( 0, 0, H_CENTER, V_CENTER );
	Object::SetSize( 0x100, 0x100, WIDTH_PIXEL, HEIGHT_PIXEL );

	Box::SetBGColor( 0xffffffff );
	Box::SetPadding( WINPAD_TOP, WINPAD_LEFT, WINPAD_RIGHT, WINPAD_BOTTOM );

	M_MsgQ.Init();
	M_StateFlags.Init( STATE_VARIABLE );
	MPv_KWinList	= NULL;
}


/*******************************************************************************
	概要	：	ウィンドウ登録
	説明	：	システムコールでウィンドウクラスのポインタ(自ポインタ)を
				アプリケーション管理に登録します。
	Include	：	Window.h
	引数	：	ui ui_AppliID		アプリケーションID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Window::RegiWindow( ui ui_AppliID )
{
	Mui_AppliID = ui_AppliID;

	//システムコール
	SCD::RegiWindow	SCD_RegiWindow;
	SCD_RegiWindow.MP_Window	= this;
	SCD_RegiWindow.Mui_AppliID	= ui_AppliID;
	if( (s4)SC::SystemCall( SC::REGI_WINDOW, &SCD_RegiWindow ) < 0 )
		return ERROR_REGIFAILED;		//ウィンドウ登録失敗

	//全描画
	DrawingAll();

	MLStdLib::FullDrawUpdate();		//全再描画

	return SUCCESS;
}


/*******************************************************************************
	概要	：	オブジェクト領域更新
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Window::UpdateArea( void )
{
	ui		ui_PxWidth, ui_PxHeight;
	ui		ui_ResX, ui_ResY;
	ui		ui_MaxHeight;

	MLStdLib::GetResolution( &ui_ResX, &ui_ResY );		//解像度取得
	ui_MaxHeight = ui_ResY - TASK_BAR_HEIGHT;


	//幅、高さのどちらかがMINなら子のサイズを取得
	if( Object::M_Flags.CheckFlags( WIDTH_MIN ) || Object::M_Flags.CheckFlags( HEIGHT_MIN ) )
		GetMinSize( &ui_PxWidth, &ui_PxHeight );

	//幅タイプによる分岐
	switch( Object::M_Flags.GetFlags( WIDTH_MASK ) )
	{
	case WIDTH_MAX:
		ui_PxWidth		= ui_ResX;
		break;
	case WIDTH_PERCENT:	//親の子領域を100%(65536)として計算
		ui_PxWidth		= SIZE_PER2PX( Mui_Width, ui_ResX );
		break;
	case WIDTH_PIXEL:	//==Mui_Width
		ui_PxWidth		= Mui_Width;
		break;
	}
	//高さタイプによる分岐
	switch( Object::M_Flags.GetFlags( HEIGHT_MASK ) )
	{
	case HEIGHT_MAX:
		ui_PxHeight		= ui_MaxHeight;
		break;
	case HEIGHT_PERCENT:
		ui_PxHeight		= SIZE_PER2PX( Mui_Height, ui_MaxHeight );
		break;
	case HEIGHT_PIXEL:
		ui_PxHeight		= Mui_Height;
		break;
	}

	//解像度以上にしない。
	if( ui_PxWidth >= ui_ResX )
		ui_PxWidth = ui_ResX - 1;
	if( ui_PxHeight >= ui_ResY )
		ui_PxHeight = ui_ResY - 1;

	Object::M_Area.SetRect(
			Object::Msi_OffsetY, Object::Msi_OffsetX,
			Object::Msi_OffsetX, Object::Msi_OffsetY );

	//水平基点による分岐
	switch( Object::M_Flags.GetFlags( HBP_MASK ) )
	{
	case H_CENTER:
		Object::M_Area.Msi_Left		+= ( ui_ResX >> 1 ) - ( ui_PxWidth >> 1 );
		Object::M_Area.Msi_Right	+= ( ui_ResX >> 1 ) + ( ui_PxWidth >> 1 );
		break;
	case H_LEFT:
		Object::M_Area.Msi_Right	+= ui_PxWidth;
		break;
	case H_RIGHT:
		Object::M_Area.Msi_Left		-= ui_PxWidth;
		break;
	}
	//垂直基点による分岐
	switch( Object::M_Flags.GetFlags( VBP_MASK ) )
	{
	case V_CENTER:
		Object::M_Area.Msi_Top		+= ( ui_MaxHeight >> 1 ) - ( ui_PxHeight >> 1 );
		Object::M_Area.Msi_Bottom	+= ( ui_MaxHeight >> 1 ) + ( ui_PxHeight >> 1 );
		break;
	case V_TOP:
		Object::M_Area.Msi_Bottom	+= ui_PxHeight;
		break;
	case V_BOTTOM:
		Object::M_Area.Msi_Top		-= ui_PxHeight;
		break;
	}

	return SUCCESS;
}



/*******************************************************************************
	概要	：	アプリケーションID取得
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	ui
*******************************************************************************/
ui		Window::GetAppliID( void )
{
	return Mui_AppliID;
}


/*******************************************************************************
	概要	：	状態フラグ取得
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	Flag16
*******************************************************************************/
Flag16	Window::GetStateFlags( void )
{
	return M_StateFlags;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウクラス：パブリックメソッド(カーネル用)
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウリストポインタ設定
	説明	：
	Include	：	Window.h
	引数	：	void* Pv_WinList		設定するウィンドウリストポインタ(カーネルポインタ)
	戻り値	：	void
*******************************************************************************/
void	Window::SetKWinList( void* Pv_WinList )
{
	//CPLが0でなければ終了
	if( MLStdLib::CPLCheck() )
		return;

	MPv_KWinList = Pv_WinList;
}


/*******************************************************************************
	概要	：	ウィンドウリストポインタ取得
	説明	：
	Include	：	Window.h
	引数	：	void
	戻り値	：	void*		ウィンドウリストのポインタ(実際はWinList*型)(カーネルポインタ)
*******************************************************************************/
void*	Window::GetKWinList( void )
{
	return MPv_KWinList;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

