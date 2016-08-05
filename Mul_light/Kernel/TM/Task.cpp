/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/Task.cpp
	概要		：	タスククラス
	詳細		：	TSSを含む、タスクのクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Task.h"
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		NML_TASK_MINTIME	= 0x0001,						//通常タスクの最小持ち時間
		NML_TASK_MAXTIME	= NML_TASK_MINTIME + 0x1ff,		//通常タスクの最大持ち時間
		SYS_TASK_MINTIME	= 0x0800,						//システムタスクの最小持ち時間
		SYS_TASK_MAXTIME	= SYS_TASK_MINTIME + 0x1ff,		//システムタスクの最大持ち時間
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	タスククラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	タスククラスの初期化
	説明	：	システムのタスクとして初期化します。
	Include	：	Task.h
	引数	：	u4 u4_EIP
				u4 u4_ESP
				const char* CPc_Caption	タスク名
				u1 u1_AllottedTime		タスクの持ち時間
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Task::Init(
		u4 u4_EIP, u4 u4_ESP,
		const char* CPc_Caption, u1 u1_AllottedTime )
{
	s4		s4_Return;
	SegmInfo	TSSInfo;		//TSSディスクリプタ作成用情報

	//TSS初期化
	TSS::Init();

	//各種レジスタ設定
	Mu4_EIP = u4_EIP;
	Mu4_ESP = u4_ESP;
	M_CS = KERNEL_CS;
	M_ES = M_SS = M_DS = M_FS = M_GS = KERNEL_DS;

	M_Flag16.Init();		//フラグ初期化
	if( ( s4_Return = SetCaption( CPc_Caption ) ) < 0 )	//キャプションの設定
		return s4_Return;
	SetAllottedTime( u1_AllottedTime, true );			//タスク持ち時間の設定

	//TSSディスクリプタの作成
	TSSInfo.u4_Base			= (u4)this;
	TSSInfo.u20b_Limit		= sizeof (Task) - 1;
	TSSInfo.u5b_Type		= G_GDT.TYPE_TSS_32;
	TSSInfo.u2b_DPL			= G_GDT.DPL0;
	TSSInfo.u1b_Granularity	= G_GDT.GRA_BYTE;
	G_GDT.SetSegment( TSSInfo, &M_TSSSelector );		//TSSディスクリプタの作成

	return SUCCESS;
}


/*******************************************************************************
	概要	：	タスククラスの終了処理
	説明	：	TSSをGDTから削除します。
	Include	：	Task.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Task::Finalizer( void )
{
	G_GDT.ClearDescriptor( M_TSSSelector );
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	タスクの持ち時間を設定します。
	説明	：	■要調整■
				持ち時間の設定によって、パフォーマンスがかなり変わってくるので、
				最適な時間設定にする。
	Include	：	Task.h
	引数	：	[u1 u1_AllottedTime = 0x20]	持ち時間
				[bool b_System = false]		システムタスク(true)か、通常タスク(false)か
	戻り値	：	void
*******************************************************************************/
void	Task::SetAllottedTime( u1 u1_AllottedTime, bool b_System )
{
	Mui_AllottedTime = u1_AllottedTime << 1;

	if( b_System )
		Mui_AllottedTime += SYS_TASK_MINTIME;
	else
		Mui_AllottedTime += NML_TASK_MINTIME;
}


/*******************************************************************************
	概要	：	タスクのキャプションを設定します。
	説明	：
	Include	：	Task.h
	引数	：	char* Pc_Caption	キャプションのポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Task::SetCaption( const char* CPc_Caption )
{
	//NULLポインタエラー
	if( CPc_Caption == NULL )
		return ERROR_NULL_POINTER;

	strncpy( MAc_Caption, CPc_Caption, sizeof( MAc_Caption ) );

	MAc_Caption[sizeof( MAc_Caption ) - 1] = 0;
	return SUCCESS;
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


/*******************************************************************************
	概要	：	持ち時間の取得
	説明	：
	Include	：	Task.h
	引数	：	void
	戻り値	：	ui		タスクの持ち時間
*******************************************************************************/
ui		Task::GetAllottedTime( void )
{
	return Mui_AllottedTime;
}


/*******************************************************************************
	概要	：	TSSセレクタの取得
	説明	：	自身のTSSを指すTSSディスクリプタのセレクタを取得します。
				なぜかインライン関数にすると、バグるので、ここに書きました。
	Include	：	Task.h
	引数	：	void
	戻り値	：	SELECTOR	TSSセレクタ
*******************************************************************************/
SELECTOR	Task::GetTSSSelector( void )
{
	return M_TSSSelector;
}


/*******************************************************************************
	概要	：	キャプションの取得
	説明	：
	Include	：	Task.h
	引数	：	char* Pc_Dest		キャプションの格納先
	戻り値	：	void
*******************************************************************************/
void	Task::GetCaption( char* Pc_Dest )
{
	strcpy( Pc_Dest, MAc_Caption );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

