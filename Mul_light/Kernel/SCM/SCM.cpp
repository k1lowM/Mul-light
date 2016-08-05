/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/SCM/SCM.cpp
	概要		：	システムコール管理
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"SCM.h"
#include	"WM.h"
#include	"TM.h"
#include	"RDTM.h"
#include	"APM.h"

#include	<MLStdLib.h>

#include	<Objects/Object.h>
#include	<Objects/Window.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		CS_SELECTOR		= 0x0008,		//コードセグメントのセレクタ
		CG_SELECTOR		= 0x0018,		//コールゲートのセレクタ
		SC_NUM_ARG		= 2,			//システムコールの引数の数
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern SCM		G_SCM;
extern WM		G_WM;
extern TM		G_TM;
extern RDTM		G_RDTM;
extern APM		G_APM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
u4		SCEntry( SC::SCID SCID, SCDBase* P_SCD );



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	システムコール管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	システムコールハンドラ
	説明	：
	Include	：	SCM.h
	引数	：	SC::SCID SCID		システムコールID
				SCDBase* P_SCD		システムコールデータ
	戻り値	：	u4		戻り値
*******************************************************************************/
u4		SCM::SCHandler( SC::SCID SCID, SCDBase* P_SCD )
{
	void*	Pv_LocalBase = ( (Appli*)G_TM.GetNowTask() )->GetLocalBase();	//ローカルのベースアドレス
	s4		s4_Return;

	P_SCD = (SCDBase*)KStdLib::ConvL2K( P_SCD, Pv_LocalBase );		//システムコールデータのアドレス変換

	switch( SCID )
	{
	//ファイル関連


	//アプリケーション関連
	case SC::DRAW_OBJECT:
	{
		SCD::DrawObject* P_SCDDO = (SCD::DrawObject*)P_SCD;
		G_WM.DrawObject( P_SCDDO->MP_Object, P_SCDDO->M_ChArea, P_SCDDO->MAu4_Option, Pv_LocalBase );
		return 0;
	}

	case SC::REGI_WINDOW:
	{
		SCD::RegiWindow* P_SCDRW = (SCD::RegiWindow*)P_SCD;
		return G_WM.RegiWindow( P_SCDRW->Mui_AppliID, P_SCDRW->MP_Window );
	}

	case SC::APL_QUIT:
		G_APM.QuitAppli( ( (SCD::AplQuit*)P_SCD )->Mui_AppliID );
		return 0;

	//Mul light標準ライブラリ
	case SC::GET_RESOLUTION:
		( (SCD::GetResolution*)P_SCD )->Mui_ResX = G_GUI.GetResX();
		( (SCD::GetResolution*)P_SCD )->Mui_ResY = G_GUI.GetResY();
		return 0;

	case SC::SLEEP:
		if( ( (SCD::Sleep*)P_SCD )->Mui_MSec == 0 )		//指定ミリ秒数が0なら休止させる
			G_TM.SleepTask();
		else
			G_RDTM.WaitMSec( ( (SCD::Sleep*)P_SCD )->Mui_MSec );
		return 0;

	case SC::WAKEUP:
		s4_Return = G_TM.WakeupTask( G_APM.GetAppli( ( (SCD::Wakeup*)P_SCD )->Mui_AppliID ) );
		return s4_Return;

	case SC::FULL_DRAW_UPDATE:
		G_GUI.FullDrawUpdate();
		return 0;

	default:
		break;
	}

	return -1;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	システムコール管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	システムコール管理クラスの初期化
	説明	：	システムコールエントリのコールゲートをGDTに登録します。
	Include	：	SCM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	SCM::Init( void )
{
	GateInfo	CallGateInfo;

	//初期化
	CallGateInfo.u4_Offset		= (u4)SCEntry;
	CallGateInfo.SegmSelector	= CS_SELECTOR;				//コードセグメントセレクタ
	CallGateInfo.u1_ParamCount	= SC_NUM_ARG;
	CallGateInfo.u5b_Type		= G_IDT.TYPE_CALL_GATE_32;	//ディスクリプタタイプ：32bitコールゲート
	CallGateInfo.u2b_DPL		= G_IDT.DPL3;			//特権レベル3
	CallGateInfo.u1b_Present	= true;					//存在フラグ

	//GDTに登録
	G_GDT.SetGate( CG_SELECTOR >> 3, CallGateInfo );

	DP( "SCM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
