/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/TM.h
	概要		：	タスク管理ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Task.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	タスク管理クラス
*******************************************************************************/
class	TM
{
//定数
public:
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_TS_DISABLE,			//TaskSwitch:タスクスイッチ禁止中。
		ERROR_DELETE_SELF,			//DeleteTask:自分で自分のタスクを削除しようとした。
		ERROR_IDLETASK_SLEEP,		//SleepTask:アイドルタスク一つの時にアイドルタスクをスリープさせようとした
		ERROR_INVALID_TASKID,		//タスクIDが無効
		ERROR_NULLPOINTER,			//NULLポインタ

		SUCCESS = _SUCCESS_,
		SUCCESS_TASK_ALREADY_ACTIVE,	//AddTask, WakeupTask:タスクが既に稼働中
		SUCCESS_TASK_ALREADY_SLEEP,	//SleepTask:タスクが既に休止中
		SUCCESS_TASK_ONLYONE,		//TaskSwitch:タスクが一つしか動作していない
	};

private:
	enum	SystemTask
	{
		ST_SYSTEMIDLE,		//システムアイドルタスク

		NUM_ST
	};

//変数
private:
	Task*	MP_NowTask;				//現在実行中のタスク(クラス)のポインタ。
	Task*	MP_SystemTask[NUM_ST];	//システムタスクのポインタ
	ui		Mui_NTTCache;			//現在実行中のタスクの持ち時間のキャッシュ。(Now Task Time Cache)
	ui		Mui_TSFlag;				//タスクスイッチ・フラグのビットスタック(0:許可、1:不許可)

//メソッド
public:
	static void	TaskManager( void );

	void	Init( void );

	Task*	CreateTask( void );				//タスク生成
	s4		AddTask( Task* P_Task );		//タスクリストへタスクを追加
	s4		DeleteTask( ui ui_TaskKMID = 0 );		//タスクリストからタスクを削除
	s4		WakeupTask( Task* P_Task );		//タスク起動
	s4		SleepTask( Task* P_Task = NULL );		//タスク休止

	void	EnableTS( void );		//タスクスイッチ許可
	void	DisableTS( void );		//タスクスイッチ不許可

	void	Scheduler( void );		//タスクの時間管理
	s4		TaskSwitch( void );		//次のタスクへ切り替え
	Task*	GetNowTask( void );		//現在実行中のタスクのポインタを取得

	void	ShowTaskList( void );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
