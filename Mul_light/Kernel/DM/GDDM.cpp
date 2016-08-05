/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/GDDM.cpp
	概要		：	グローバル・デバイス・データ管理クラス
	詳細		：	GDDQに格納されたデバイスデータをメッセージに変換し、GMQに格納します。
					Drivers -> GDDQ -> GDDM -> GMQ -> WM
					GDDQ(グローバル・デバイス・データ・キュー)
					GMQ(グローバル・メッセージ・キュー)
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GDDM.h"
#include	"GDDQ.h"
#include	"DM.h"
#include	"TM.h"
#include	"GMQ.h"
//#include	"WM.h"

#include	"Keyboard.h"
#include	"Mouse.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
Task*	GP_GDDMTask;		//グローバル・デバイス・データ管理タスク

extern GDDM		G_GDDM;
extern GDDQ		G_GDDQ;
extern GMQ		G_GMQ;

extern DM		G_DM;
extern TM		G_TM;
//extern WM		G_WM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル・デバイス・データ管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバイス・データ管理タスク
	説明	：
	Include	：	GDDM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GDDM::Task( void )
{
	G_GDDM.Init();
	G_GDDM.Main();

	while( true )
		G_TM.SleepTask();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル・デバイス・データ管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	グローバル・デバイス・データ管理クラスの初期化
	説明	：
	Include	：	GDDM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GDDM::Init( void )
{
	G_GDDQ.Init();
}


/*******************************************************************************
	概要	：	グローバル・デバイス・データ管理クラスのメイン
	説明	：	GDDQからデータを取り出し、ファイルシステムに伝えて加工した後、
				メッセージに変換し、GMQにメッセージを格納します。
	Include	：	GDDM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GDDM::Main( void )
{
	DD		DeviceData;
	DevInfo	DeviceInfo;
	Msg		Message;

	//休止
_Sleep:
	G_TM.SleepTask();

	while( true )
	{
		//デバイスデータを取得。エラーがあれば休止。
		if( G_GDDQ.Dequeue( &DeviceData ) < 0 )
			goto _Sleep;

		//デバイス情報取得
		DeviceInfo = G_DM.GetDevInfo( DeviceData.GetDeviceID() );

		//ファイルシステムへ渡す。(データの加工と管理)
		switch( DeviceInfo.P_FS->GetFSType() )
		{
		//キーボード
		case FST::KEYBOARD:
			{
				Keyboard*	P_Keyboard = (Keyboard*)( DeviceInfo.P_FS );
				P_Keyboard->PassDD( &DeviceData );
				break;
			}
		//マウス
		case FST::MOUSE:
			{
				Mouse*		P_Mouse = (Mouse*)( DeviceInfo.P_FS );
				P_Mouse->PassDD( &DeviceData );
				break;
			}

		//その他(これ以上何も処理できないので、次のデータを読む。)
		default:
			continue;		//もう一度実行。
		}

		//デバイスデータをメッセージに変換
		if( DD2Msg( DeviceData, &Message ) < 0 )
			continue;		//もう一度実行。

		//GMQへメッセージを格納
		G_GMQ.Enqueue( Message );

		continue;		//もう一度実行。
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
