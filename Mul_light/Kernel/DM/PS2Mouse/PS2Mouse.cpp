/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PS2Mouse/PS2Mouse.h
	概要		：	PS2マウスドライバ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	■デバイスデータ仕様
	Mu4_Data[0]	：	31-19:予約, 18:中央ボタン, 17:右ボタン, 16:左ボタン, 15-8:横方向移動量, 7-0:縦方向移動量
	Mu4_Data[1]	：	予約
	Mu4_Data[2]	：	予約
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"PS2Mouse.h"
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
		PORT_RW_KBC_DATA		= 0x0060,		//データポート
		PORT_R_KBC_STATUS		= 0x0064,		//ステータス
		PORT_W_KBC_COMMAND		= 0x0064,		//制御コマンド
	};

	//ステータスフラグ
	enum	StatusFlags
	{
		DATA_READY_MASK			= 0x01,			//データ準備マスク
		BUSY_MASK				= 0x02,			//ビジーフラグマスク
	};

	//コマンド・レジスタを用いたコマンド
	enum	Command
	{
		CMD_READ_MODE			= 0x20,			//コマンド・バイト・レジスタ　リードモード
		CMD_WRITE_MODE			= 0x60,			//コマンド・バイト・レジスタ　ライトモード

		CMD_SENDTO_MOUSE		= 0xd4,			//マウスへコマンド送信
	};

	//コマンド・バイト・レジスタ・フラグ
	enum	CmdByteFlags
	{
		KEYBOARD_INT_DISABLE	= 0x00,		//キーボード割り込み無効
		KEYBOARD_INT_ENABLE		= 0x01,		//キーボード割り込み有効
		MOUSE_INT_DISABLE		= 0x00,		//マウス割り込み無効
		MOUSE_INT_ENABLE		= 0x02,		//マウス割り込み有効
		SYS_0					= 0x00,		//システムフラグ0
		SYS_1					= 0x04,		//システムフラグ1

		KEYBOARD_ENABLE			= 0x00,		//キーボード有効化
		KEYBOARD_DISABLE		= 0x10,		//キーボード無効化
		MOUSE_ENABLE			= 0x00,		//マウス有効化
		MOUSE_DISABLE			= 0x20,		//マウス無効化

		TRANSLATE_DISABLE		= 0x00,		//キーコード変換なし
		TRANSLATE_ENABLE		= 0x40,		//キーコード変換(キーコード1に変換)
	};

	//その他コマンド
	enum
	{
		MOUSECMD_ENABLE			= 0xf4,
	};

	//マウスのボタン情報
	enum
	{
		MOUSE_LEFT_BUTTON		= 0x01,			//左ボタン
		MOUSE_RIGHT_BUTTON		= 0x02,			//右ボタン
		MOUSE_CENTER_BUTTON		= 0x04,			//中央ボタン
		MOUSE_BUTTON_MASK		= 0x07,			//ボタンマスク
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	Task*	GP_MouseReadTask;				//PS2マウスリードタスクのポインタ
}

extern PS2Mouse	G_PS2Mouse;
extern TM		G_TM;
extern PIC		G_PIC;
extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2マウス割り込みルーチン
	説明	：	PS2マウスの割り込みルーチンです。
	Include	：	-
	引数	：	ARG_CONST_REGI_STAT
	戻り値	：	void
*******************************************************************************/
void	Int2C( ARG_CONST_REGI_STAT )
{
	G_PIC.ACK( PIC::IRQ12_MOUSE );			//IRQ12の受付完了をPICに通知
	G_PIC.ACK( PIC::IRQ2_PIC1 );			//IRQ2の受付完了をPICに通知
	G_PS2Mouse.Enqueue( IO::In1( PORT_RW_KBC_DATA ) );		//マウス情報取得＆バッファに格納
	G_TM.WakeupTask( GP_MouseReadTask );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PS2マウスクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2マウスリードタスク
	説明	：	割り込みが発生したときに、自動でGDDQにデータを格納するためのタスク。
	Include	：	PS2Mouse
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PS2Mouse::Task( void )
{
	u1		u1_MouseXDist, u1_MouseYDist;
	u1		u1_MouseButton;
	DD		DeviceData;

	//休止
_Sleep:
	G_TM.SleepTask();

	while( true )
	{
		//データ取得
		if( G_PS2Mouse.Read( &u1_MouseXDist, &u1_MouseYDist, &u1_MouseButton ) < 0 )
			goto _Sleep;

		//デバイス・データ準備
		DeviceData.Init( G_PS2Mouse.GetDeviceID() );
		DeviceData.Mu4_Data[0] = ( u1_MouseButton << 16 ) | ( u1_MouseXDist << 8 ) | ( u1_MouseYDist );

		G_GDDQ.Enqueue( DeviceData );		//データをグローバル・デバイス・データ・キューに格納

		continue;		//もう一度キューを読む
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PS2マウスクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PS2マウスクラスの初期化
	説明	：
	Include	：	PS2Mouse.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PS2Mouse::Init( void )
{
	M_DeviceType = DT::MOUSE;					//デバイスタイプ設定
	M_MouseDataQ1.Init();						//キュー初期化
	G_DM.RegiDevice( &Mu1_DeviceID, this );		//デバイス登録

	//マウスリードタスクセットアップ
	{
		GP_MouseReadTask	= G_TM.CreateTask();	//タスク生成
		GP_MouseReadTask->Init(						//タスク初期化
				(u4)PS2Mouse::Task,
				(u4)G_GMA.GetBase( G_GMA.Create( PAGE2BYTE( 4 ) ) ) + PAGE2BYTE( 4 ),
				"PS2Mouse read" );
		G_TM.AddTask( GP_MouseReadTask );			//タスク追加
	}

	//デバイス初期化
	{
		WaitKBCBusy();
		IO::Out1( PORT_W_KBC_COMMAND, CMD_WRITE_MODE );
		WaitKBCBusy();
		IO::Out1( PORT_RW_KBC_DATA, KEYBOARD_INT_ENABLE | MOUSE_INT_ENABLE | SYS_1 | KEYBOARD_ENABLE | MOUSE_ENABLE | TRANSLATE_ENABLE );

		WaitKBCBusy();
		IO::Out1( PORT_W_KBC_COMMAND, CMD_SENDTO_MOUSE );
		WaitKBCBusy();
		IO::Out1( PORT_RW_KBC_DATA, MOUSECMD_ENABLE );
	}

	G_PIC.IntEnable( G_PIC.IRQ2_PIC1 );			//スレーブPIC割り込み許可
	G_PIC.IntEnable( G_PIC.IRQ12_MOUSE );		//PS2マウス割り込み許可
}


/*******************************************************************************
	概要	：	デバイス制御
	説明	：	カーネルからのさまざまな要求に対応する関数です。
	Include	：	PS2Mouse.h
	引数	：	DCD* P_DCD			制御に関する構造体のポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Mouse::Ctrl( DCD* P_DCD )
{
//	DCD_Char*	P_DCDChar = (DCD_Char*)P_DCD;
//
//	switch( P_DCDChar->GetCtrlID() )
//	{
//	//リード、キャラクタデバイス
//	case DCD::CTRL_READ | DCD::CLASS_CHAR:
//		return Read( &P_DCDChar->ui_Data1 );
//	}
//
//	//無効な制御ID
//	return ERROR_INVALIDDCID;

	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	PS2マウス：読み取り
	説明	：	PS2マウスのバッファからデータを読み取ります
	Include	：	PS2Mouse.h
	引数	：	u1* Pu1_XDistanceBuf		横方向移動量格納バッファ
				u1* Pu1_YDistanceBuf		縦方向移動量格納バッファ
				u1* Pu1_ButtonBuf			ボタン類フラグバッファ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Mouse::Read( u1* Pu1_XDistanceBuf, u1* Pu1_YDistanceBuf, u1* Pu1_ButtonBuf )
{
	u1		u1_QueueBuf;

	//バッファにデータが3つ以上なければエラー
	if( M_MouseDataQ1.GetNumData() < 3 )
		return ERROR_DATAINSUFFICIENCY;		//データが不十分

	//データ取得
	if( M_MouseDataQ1.Dequeue( &u1_QueueBuf ) < 0 )
		return ERROR_NODATA;		//データなし

	//データ正誤チェック(6,7bit目が0で、3bit目が1ならOK！)
	if( ( u1_QueueBuf & 0b11001000 ) != 0b00001000 )
		return ERROR_INVALILDDATA;

	//データ取得
	{
		//1バイト目(データ)
		*Pu1_ButtonBuf			= u1_QueueBuf & MOUSE_BUTTON_MASK;

		//2バイト目(x)
		if( M_MouseDataQ1.Dequeue( Pu1_XDistanceBuf ) < 0 )		//データ取得
			return ERROR_NODATA;		//データなし

		//3バイト目(y)
		if( M_MouseDataQ1.Dequeue( Pu1_YDistanceBuf ) < 0 )		//データ取得
			return ERROR_NODATA;		//データなし
	}

	return SUCCESS;
}


/*******************************************************************************
	概要	：	エンキュー
	説明	：	バッファにデータをエンキューします。
	Include	：	PS2Mouse.h
	引数	：	u1 u1_DeviceID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PS2Mouse::Enqueue( u1 u1_Data )
{
	return M_MouseDataQ1.Enqueue( u1_Data );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PS2マウスクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	キーボードコントローラのビジー待ち
	説明	：
	Include	：	PS2Mouse.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PS2Mouse::WaitKBCBusy( void )
{
	while( IO::In1( PORT_R_KBC_STATUS ) & BUSY_MASK )
	{}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
