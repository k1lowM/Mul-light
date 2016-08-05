/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/MLStdLib.cpp
	概要		：	Mul light API - Mul light標準ライブラリクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"MLStdLib.h"
#include	"SC.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	Mul light標準ライブラリクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	解像度取得
	説明	：
	Include	：	MLStdLib.h
	引数	：	ui* Pui_ResX
				ui* Pui_ResY
	戻り値	：	void
*******************************************************************************/
void	MLStdLib::GetResolution( ui* Pui_ResX, ui* Pui_ResY )
{
	//システムコール
	SCD::GetResolution	SCD_GetResolution;
	SC::SystemCall( SC::GET_RESOLUTION, &SCD_GetResolution );

	//NULLポインタでなければ格納
	if( Pui_ResX != NULL )
		*Pui_ResX = SCD_GetResolution.Mui_ResX;
	if( Pui_ResY != NULL )
		*Pui_ResY = SCD_GetResolution.Mui_ResY;
}


/*******************************************************************************
	概要	：	タスクスリープ
	説明	：	指定ミリ秒数間タスクをスリープします。
				引数を指定しないか、0を指定した場合は
				何か他の契機があるまでスリープしつづけます。
	Include	：	MLStdLib.h
	引数	：	[ui ui_MSec = 0]		スリープするミリ秒
	戻り値	：	void
*******************************************************************************/
void	MLStdLib::Sleep( ui ui_MSec )
{
	//システムコール
	SCD::Sleep	SCD_Sleep;
	SCD_Sleep.Mui_MSec = ui_MSec;
	SC::SystemCall( SC::SLEEP, &SCD_Sleep );
}


/*******************************************************************************
	概要	：	タスク起動
	説明	：	自身のタスクを起動します。
	Include	：	MLStdLib.h
	引数	：	ui ui_AppliID		起動するアプリケーションのID
	戻り値	：	void
*******************************************************************************/
void	MLStdLib::Wakeup( ui ui_AppliID )
{
	//システムコール
	SCD::Wakeup	SCD_Wakeup;
	SCD_Wakeup.Mui_AppliID = ui_AppliID;
	SC::SystemCall( SC::WAKEUP, &SCD_Wakeup );
}


/*******************************************************************************
	概要	：	全再描画
	説明	：
	Include	：	MLStdLib.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	MLStdLib::FullDrawUpdate( void )
{
	//システムコール
	SCD::FullDrawUpdate	FullDrawUpdate;
	SC::SystemCall( SC::FULL_DRAW_UPDATE, &FullDrawUpdate );
}





/*******************************************************************************
	概要	：	現在の特権レベルを取得
	説明	：
	Include	：	MLStdLib.h
	引数	：	void
	戻り値	：	ui		現在の特権レベル
*******************************************************************************/
ui		MLStdLib::CPLCheck( void )
{
	u2		u2_CS;
	__asm__
	(
			"movw	%%cs,	%0;"
		: "=r"( u2_CS )
	);
	return u2_CS & 0x0003;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

