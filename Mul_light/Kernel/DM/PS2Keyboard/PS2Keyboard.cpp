/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PS2Keyboard/PS2Keyboard.cpp
	概要		：	PS2キーボードドライバ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	■デバイスデータ仕様
	Mu4_Data[0]	：	キーデータ
	Mu4_Data[1]	：	予約
	Mu4_Data[2]	：	予約
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■as
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"PS2Keyboard.h"
#include	"TM.h"
#include	<Math.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		//I/Oポート
		PORT_KBC_DATA		= 0x0060,		//データポート
		PORT_KBC_STATUS		= 0x0064,		//ステータス
		PORT_KBC_COMMAND	= 0x0064,		//制御コマンド
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	Task*	GP_KeyReadTask;				//PS2キーボードリードタスクのポインタ
}

extern PS2Keyboard	G_PS2Keyboard;
extern TM		G_TM;
extern PIC		G_PIC;
extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2キーボード割り込みルーチン
	説明	：	PS2キーボードの割り込みルーチンです。
	Include	：	-
	引数	：	ARG_CONST_REGI_STAT
	戻り値	：	void
*******************************************************************************/
void	Int21( ARG_CONST_REGI_STAT )
{
	G_PIC.ACK( G_PIC.IRQ1_KEYBOARD );		//IRQ1の受付完了をPICに通知
	G_PS2Keyboard.Enqueue( IO::In1( PORT_KBC_DATA ) );		//キー情報取得＆バッファに格納
	G_TM.WakeupTask( GP_KeyReadTask );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PS2キーボードクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2キーボードリードタスク
	説明	：	割り込みが発生したときに、自動でGDDQにデータを格納するためのタスク。
	Include	：	PS2Keyboard
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PS2Keyboard::Task( void )
{
	ui		ui_KeyData;
	DD		DeviceData;

	//休止
_Sleep:
	G_TM.SleepTask();

	while( true )
	{
		//データ取得
		if( G_PS2Keyboard.Read( &ui_KeyData ) < 0 )
			goto _Sleep;

		//デバイス・データ準備
		DeviceData.Init( G_PS2Keyboard.GetDeviceID() );
		DeviceData.Mu4_Data[0] = ui_KeyData;

		G_GDDQ.Enqueue( DeviceData );		//データをグローバル・デバイス・データ・キューに格納

		continue;		//もう一度キューを読む
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PS2キーボードクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2キーボードクラスの初期化
	説明	：
	Include	：	PS2Keyboard.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PS2Keyboard::Init( void )
{
	M_DeviceType = DT::KEYBOARD;				//デバイスタイプ設定
	M_KeyDataQ1.Init();							//キュー初期化
	G_DM.RegiDevice( &Mu1_DeviceID, this );		//デバイス登録

	//キーリードタスクセットアップ
	{
		GP_KeyReadTask	= G_TM.CreateTask();	//タスク生成
		GP_KeyReadTask->Init(					//タスク初期化
				(u4)PS2Keyboard::Task,
				(u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ),
				"PS2Keyboard read" );
		G_TM.AddTask( GP_KeyReadTask );			//タスク追加
	}

	G_PIC.IntEnable( G_PIC.IRQ1_KEYBOARD );		//PS2キーボード割り込み許可
}


/*******************************************************************************
	概要	：	デバイス制御
	説明	：	カーネルからのさまざまな要求に対応する関数です。
	Include	：	PS2Keyboard.h
	引数	：	DCD* P_DCD			制御に関する構造体のポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Keyboard::Ctrl( DCD* P_DCD )
{
	DCD_Char*	P_DCDChar = (DCD_Char*)P_DCD;

	switch( P_DCDChar->GetCtrlID() )
	{
	//リード、キャラクタデバイス
	case DCD::CTRL_READ | DCD::CLASS_CHAR:
		return Read( &P_DCDChar->ui_Data1 );
	}

	//無効な制御ID
	return ERROR_INVALIDDCID;
}


/*******************************************************************************
	概要	：	PS2キーボード：読み取り
	説明	：	PS2キーボードのキューバッファからデータを読み取ります
				▲コマンドの実装や、タスクスイッチ禁止にすべき。▲
	Include	：	PS2Keyboard.h
	引数	：	ui* Pui_KeyDataBuf		キーデータを格納するバッファ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Keyboard::Read( ui* Pui_KeyDataBuf )
{
	ui		ui_KeyDataBuf = 0;
	u1		u1_QueueBuf;

	//エラー処理
	if( Pui_KeyDataBuf == NULL )
		return ERROR_NULLPOINTER;

	//キーデータ読み取り
	if( M_KeyDataQ1.Read( &u1_QueueBuf ) < 0 )
		return ERROR_NODATA;


	switch( u1_QueueBuf )
	{
	//拡張キーコード(E0)
	case 0xe0:
		if( M_KeyDataQ1.GetNumData() < 2 )
			return ERROR_DATAINSUFFICIENCY;			//データが不十分
		for( ui ui_Count = 0; ui_Count < 2; ui_Count++ )
		{
			//キーデータ取得
			if( M_KeyDataQ1.Dequeue( &u1_QueueBuf ) < 0 )
				return ERROR_NODATA;
			ui_KeyDataBuf = ( ui_KeyDataBuf << 8 ) | u1_QueueBuf;
		}
		break;
	//拡張キーコード(E1)
	case 0xe1:
		if( M_KeyDataQ1.GetNumData() < 3 )
			return ERROR_DATAINSUFFICIENCY;			//データが不十分
		for( ui ui_Count = 0; ui_Count < 3; ui_Count++ )
		{
			//キーデータ取得
			if( M_KeyDataQ1.Dequeue( &u1_QueueBuf ) < 0 )
				return ERROR_NODATA;
			ui_KeyDataBuf = ( ui_KeyDataBuf << 8 ) | u1_QueueBuf;
		}
		break;
	//通常キーコード
	default:
		//キーデータ取得
		if( M_KeyDataQ1.Dequeue( &u1_QueueBuf ) < 0 )
			return ERROR_NODATA;
		ui_KeyDataBuf = ( ui_KeyDataBuf << 8 ) | u1_QueueBuf;
		break;
	}

	*Pui_KeyDataBuf = ui_KeyDataBuf;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	エンキュー
	説明	：	バッファにデータをエンキューします。
	Include	：	PS2Keyboard.h
	引数	：	u1 u1_Data
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Keyboard::Enqueue( u1 u1_Data )
{
	return M_KeyDataQ1.Enqueue( u1_Data );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
