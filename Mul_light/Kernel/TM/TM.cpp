/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/TM.cpp
	概要		：	タスク管理
	詳細		：	タスクの管理を統括して行います。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"TM.h"
#include	"Desktop.h"
#include	"WM.h"
#include	"APM.h"
#include	<Math.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;		//タスク管理

//タスク
Task*	GP_TMTask;			//タスクマネージャータスク
extern Task*	GP_APMTask;			//アプリケーション管理タスク
extern Task*	GP_WMTask;			//ウィンドウ管理タスク
extern Task*	GP_GDDMTask;		//グローバル・デバイス・データ管理タスク
extern Task*	GP_DesktopTask;		//デスクトップタスクのポインタ



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	タスク管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	タスク管理タスク
	説明	：
	Include	：	-
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::TaskManager( void )
{
	while( true )
	{
//		G_TM.SleepTask( 1 );
		G_TM.SleepTask();
	}

	while( true )
		G_TM.SleepTask();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	タスク管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	タスク管理クラスの初期化
	説明	：
	Include	：	TM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::Init( void )
{
	MP_NowTask		= NULL;
	Mui_NTTCache	= 0;
	Mui_TSFlag		= (ui)-1;		//全ビット不許可

	Mui_NTTCache *= 4;

	//タスク生成
	MP_SystemTask[ST_SYSTEMIDLE]	= CreateTask();		//システムアイドルタスク生成
	GP_TMTask			= CreateTask();		//タスクマネージャタスク生成
	GP_APMTask			= CreateTask();		//アプリケーション管理タスク生成
	GP_WMTask			= CreateTask();		//ウィンドウ管理タスク生成
	GP_GDDMTask			= CreateTask();		//グローバル・デバイス・データ管理タスク生成
	GP_DesktopTask		= CreateTask();		//デスクトップタスク生成

	//タスク初期化( 初期EIP, 初期ESP, タスクのキャプション, 持ち時間 )
	MP_SystemTask[ST_SYSTEMIDLE]->Init( NULL, NULL, "System idle" );
	MP_SystemTask[ST_SYSTEMIDLE]->SetAllottedTime( 0, false );		//アイドルタスクの持ち時間を最低に。
	GP_TMTask->Init( (u4)TM::TaskManager, (u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ), "Task manager" );
	GP_APMTask->Init( (u4)APM::Task, (u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ), "Application manager" );
	GP_WMTask->Init( (u4)WM::Task, (u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ), "Window manager", 0x20 );
	GP_GDDMTask->Init( (u4)GDDM::Task, (u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ), "Global device data manager" );
	GP_DesktopTask->Init( (u4)Desktop::Task, (u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ), "Desktop" );
	GP_DesktopTask->SetAllottedTime( 0x20, false );

	//タスク追加
	AddTask( MP_SystemTask[ST_SYSTEMIDLE] );

	DP( "TM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	タスク生成
	説明	：	タスククラスを動的確保して、タスクのポインタを返します。
				動的確保には、KMAが使用されます。
	Include	：	TM.h
	引数	：	void
	戻り値	：	Task*		生成したタスクのポインタ
*******************************************************************************/
Task*	TM::CreateTask( void )
{
	return (Task*)G_KMA.GetBase( G_KMA.Create( sizeof (Task) ) );
}


/*******************************************************************************
	概要	：	タスク追加
	説明	：	TSSディスクリプタを作成し、タスクリストにタスクを追加します。
				現在実行中のタスクの前(最後尾)に追加します。
	Include	：	TM.h
	引数	：	Task* P_Task		追加するタスクのポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		TM::AddTask( Task* P_Task )
{
	//エラー処理
	{
		if( P_Task == NULL )
			return ERROR_NULLPOINTER;
		if( P_Task->M_Flag16.CheckFlags( Task::STATE_ACTIVE ) )		//既に稼働中
			return SUCCESS_TASK_ALREADY_ACTIVE;
		//セグメント・レジスタエラー
		//スタック・ポインタエラー
	}


	//タスクが1つも無い状態(初期状態)
	if( MP_NowTask == NULL )
	{
		//追加するタスクのリストポインタを設定
		P_Task->MP_PrevTask = P_Task->MP_NextTask = P_Task;
		//現在実行中のタスクを追加するタスクに設定。
		MP_NowTask = P_Task;		//この段階ではまだタスクスイッチしません。2つ以上タスクができてからです。
		//稼働状態にフラグをセット
		P_Task->M_Flag16.SetFlags( Task::STATE_ACTIVE );
		//タスクレジスタを設定
		asm
		(
				"ltr	%%ax;"
				:
				: "a"( MP_NowTask->GetTSSSelector() )
		);
	}
	//タスクが1つ以上ある状態
	else
	{
		//タスクの連結
		/*▼▽▼タスクスイッチ禁止▼▽▼*/
		DisableTS();
		{
			//現在のタスク → の前のタスク → の次ポインタ を追加するタスクに設定
			MP_NowTask->MP_PrevTask->MP_NextTask	= P_Task;
			//追加するタスクのリストポインタを設定
			P_Task->MP_PrevTask		= MP_NowTask->MP_PrevTask;		//前ポインタ
			P_Task->MP_NextTask		= MP_NowTask;					//次ポインタ
			//現在のタスク → の前ポインタ を追加するタスクに設定
			MP_NowTask->MP_PrevTask	= P_Task;

			//稼働状態にフラグをセット
			P_Task->M_Flag16.SetFlags( Task::STATE_ACTIVE );

			//システムアイドルタスクを休止
			if( P_Task != MP_SystemTask[ST_SYSTEMIDLE] )
				SleepTask( MP_SystemTask[ST_SYSTEMIDLE] );

		}
		EnableTS();
		/*▲△▲タスクスイッチ許可▲△▲*/
	}

	return SUCCESS;
}


/*******************************************************************************
	概要	：	タスク削除
	説明	：	タスクをタスクリストから除外し、更に、メモリからも削除します。
				自分自身のタスクも削除可能です。
	Include	：	TM.h
	引数	：	[ui ui_TaskKMID = 0]		タスクリストから除外するタスクのID
	戻り値	：	s4	エラー情報
*******************************************************************************/
s4		TM::DeleteTask( ui ui_TaskKMID )
{
	Task*	P_Task = (Task*)G_KMA.GetBase( ui_TaskKMID );

	//エラー処理、他。
	if( ui_TaskKMID == 0 )		//指定がなければ、現在実行中(自分自身)のタスクを削除する。
		P_Task = MP_NowTask;
	if( P_Task == NULL )			//タスクIDで取得したベースアドレスがNULL
		return ERROR_INVALID_TASKID;
	if( P_Task->MP_NextTask == P_Task )		//タスクが一つしか動いていない場合
		WakeupTask( MP_SystemTask[ST_SYSTEMIDLE] );		//システムアイドルタスクを起動


	/*▼▽▼タスクスイッチ禁止▼▽▼*/
	DisableTS();
	{
		//アクティブなタスクなら、まず、タスクリストから除外
		if( P_Task->M_Flag16.CheckFlags( Task::STATE_ACTIVE ) )
		{
			//タスクをリストから外す
			//除外するタスクの前と後ろを連結
			P_Task->MP_PrevTask->MP_NextTask	= P_Task->MP_NextTask;
			P_Task->MP_NextTask->MP_PrevTask	= P_Task->MP_PrevTask;
		}

		G_KMA.Delete( ui_TaskKMID );		//メモリ領域開放
	}
	EnableTS();
	/*▲△▲タスクスイッチ許可▲△▲*/

	return SUCCESS;
}


/*******************************************************************************
	概要	：	タスク起動
	説明	：	休止中のタスクをタスクリストに追加します。
	Include	：	TM.h
	引数	：	Task* P_Task	起動するタスクのポインタ
	戻り値	：	s4	エラー情報
*******************************************************************************/
s4		TM::WakeupTask( Task* P_Task )
{
	return AddTask( P_Task );
}


/*******************************************************************************
	概要	：	タスク休止
	説明	：	タスクリストからタスクを外し、タスクを休止状態にさせます。
				自分自身のタスクを休止させることができます。
	Include	：	TM.h
	引数	：	Task* P_Task	休止するタスクのポインタ
	戻り値	：	s4	エラー情報
*******************************************************************************/
s4		TM::SleepTask( Task* P_Task )
{
	s4		s4_Return;

	//指定がなければ、現在実行中(自分自身)のタスクを休止する。
	if( P_Task == NULL )
		P_Task = MP_NowTask;
	//指定のタスクが既に休止中なら終了
	if( P_Task->M_Flag16.CheckFlags( Task::STATE_SLEEP ) )
		return SUCCESS_TASK_ALREADY_SLEEP;

	DisableTS();		//タスクスイッチ禁止
	{
		//タスクが一つしか動いていない場合
		if( P_Task->MP_NextTask == P_Task )
			s4_Return = WakeupTask( MP_SystemTask[ST_SYSTEMIDLE] );		//システムアイドルタスクを起動

		//タスクをリストから外す
		P_Task->MP_PrevTask->MP_NextTask	= P_Task->MP_NextTask;
		P_Task->MP_NextTask->MP_PrevTask	= P_Task->MP_PrevTask;

		//スリープ状態にフラグをセット
		P_Task->M_Flag16.SetFlags( Task::STATE_SLEEP );
	}
	EnableTS();			//タスクスイッチ許可


	//自分自身のタスクをsleepさせる場合、これ以上実行してはいけない。
	if( P_Task == MP_NowTask )
	{
		Mui_TSFlag = 0;		//タスクスイッチ強制許可
		TaskSwitch();
	}

	return SUCCESS;
}


/*******************************************************************************
	概要	：	タスクスイッチ許可
	説明	：
	Include	：	TM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::EnableTS( void )
{
	Mui_TSFlag >>= 1;
}


/*******************************************************************************
	概要	：	タスクスイッチ不許可
	説明	：
	Include	：	TM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::DisableTS( void )
{
	Mui_TSFlag <<= 1;
	Mui_TSFlag |= 1;		//最下位ビットを不許可
}


/*******************************************************************************
	概要	：	スケジューラ
	説明	：	タイマ割り込みに呼ばれ、タスクの時間や切り替えを管理します。
	Include	：	TM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::Scheduler( void )
{
	//持ち時間が0になったらタスクスイッチ。
	if( !( Mui_NTTCache-- ) )
		TaskSwitch();
}


/*******************************************************************************
	概要	：	タスク切り替え
	説明	：	タスクを次のタスクに切り替えます。
	Include	：	TM.h
	引数	：	void
	戻り値	：	s4	エラー情報
*******************************************************************************/
s4		TM::TaskSwitch( void )
{
	struct		//タスクスイッチ時のセグメント・セレクタと、オフセット
	{
		u4		u4_Offset;
		u2		u2_Selector;
	}TS;
	Task*	P_NextTask;

	//タスクスイッチ禁止中(全て0で無い限り)ならエラー
	if( Mui_TSFlag )
	{
		Mui_NTTCache = 0;
		return ERROR_TS_DISABLE;
	}

	//次のタスクが現在のタスク(タスクが一つ)ならタスクスイッチは行わない
	if( MP_NowTask == ( P_NextTask = MP_NowTask->MP_NextTask ) )
	{
		Mui_NTTCache	= 0;
		return SUCCESS_TASK_ONLYONE;
	}

	//次に実行するタスクの持ち時間をキャッシュに読み込み
	Mui_NTTCache = P_NextTask->GetAllottedTime();

	TS.u4_Offset = 0;
	TS.u2_Selector = P_NextTask->GetTSSSelector();

	//次のタスクポインタを設定
	MP_NowTask = P_NextTask;

	asm
	(
			"ljmp		*(%0);"
			:
			: "r"( &TS )
	);
	return SUCCESS;
}


/*******************************************************************************
	概要	：	現在実行中のタスクのポインタを取得
	説明	：
	Include	：	TM.h
	引数	：	void
	戻り値	：	Task*	タスククラスのポインタ
*******************************************************************************/
Task*	TM::GetNowTask( void )
{
	return MP_NowTask;
}


/*******************************************************************************
	概要	：	タスクリスト表示
	説明	：	タスクを一覧表示します。
	Include	：	TM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TM::ShowTaskList( void )
{
	char	Ac_String[64];

	Task*	P_Task = GetNowTask();
	Task*	P_FirstTask = P_Task;
	ui		ui_CountY = 24;

	DisableTS();		//タスクスイッチ禁止

	while( true )
	{
		char	Ac_Caption[64];
		P_Task->GetCaption( Ac_Caption );
		sprintf( Ac_String, "Task:%s             ", Ac_Caption );
		G_GUI.DebugPrintString( Ac_String, 1, ui_CountY++, 0xffff00ff );

		P_Task = P_Task->MP_NextTask;

		if( P_Task == P_FirstTask )
			break;
	}

	G_GUI.DebugPrintString( "                                           ", 1, ui_CountY++, 0xffff00ff );
	EnableTS();			//タスクスイッチ許可
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

