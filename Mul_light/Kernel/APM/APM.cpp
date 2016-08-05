/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/APM.cpp
	概要		：	アプリケーション管理
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"APM.h"
#include	"VFS.h"
#include	"AMSI.h"

#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	アプリケーション構造情報構造体
*******************************************************************************/
class	AppliStructInfo
{
private:
	ui		ui_ClassSize;		//アプリケーションクラスのサイズ:sizeof (Appli)
	ui		ui_BodySize;		//アプリケーションの本体サイズ:AplHead.u4_AppliSize - sizeof (AppliHeader)
	ui		ui_HeapSize;		//ヒープ領域のサイズ
	ui		ui_StackSize;		//スタック領域のサイズ
	ui		ui_PL0StackSize;	//特権0のスタック領域のサイズ

	void*	Pv_Base;			//アプリケーションのベースアドレス

//メソッド
public:
	void	Init( const AppliHeader& CR_AplHead );

	//ゲッター
	void*	GetBase( void );			//ベースアドレス
	Appli*	GetAppliBase( void );		//アプリケーションクラスのベースアドレス
	void*	GetBodyBase( void );		//アプリケーション本体のベースアドレス
	void*	GetHeapBase( void );		//ヒープ領域のベースアドレス
	void*	GetStackBase( void );		//スタック領域のベースアドレス
	void*	GetPL0StackBase( void );	//特権0のスタック領域のベースアドレス

	u4		GetEIP( void );				//EIPに設定すべき値
	u4		GetESP( void );				//ESPに設定すべき値
	u4		GetESP0( void );			//ESP0に設定すべき値
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
Task*	GP_APMTask;			//アプリケーション管理タスク

extern Task*	GP_DesktopTask;

extern APM		G_APM;
extern TM		G_TM;
extern WM		G_WM;
extern VFS		G_VFS;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ウィンドウ管理タスク
	説明	：	デバイスデータの管理を専門に行うタスクです。
				GMQにデータが格納されたときに自動で起動され、仕事が終わり次第休止状態に入ります。
	Include	：	-
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	APM::Task( void )
{
	G_APM.Init();
	G_APM.Main();

	while( true )
		G_TM.SleepTask();
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	アプリケーション管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	アプリケーション管理クラスの初期化
	説明	：
	Include	：	APM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	APM::Init( void )
{
	memset( MAui_AppliGMID, 0, sizeof MAui_AppliGMID );
	M_QuitAppliQ.Init();
	DP( "APM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	アプリケーション管理クラスのメイン
	説明	：
	Include	：	APM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	APM::Main( void )
{
	u1		u1_AppliNo;

	//休止
_Sleep:
	G_TM.SleepTask();

	while( true )
	{
		//メッセージを取得。エラーがあれば休止。
		if( M_QuitAppliQ.Dequeue( &u1_AppliNo ) < 0 )
			goto _Sleep;

		//アプリケーション終了処理
		G_TM.DisableTS();		//タスクスイッチ禁止
		{
			ui		ui_AppliGMID;
			Appli*	P_Appli;

			ui_AppliGMID	= MAui_AppliGMID[u1_AppliNo];
			P_Appli			= (Appli*)G_GMA.GetBase( ui_AppliGMID );
			MAui_AppliGMID[u1_AppliNo] = 0;

			//ウィンドウ削除
			G_WM.DeleteWindow( No2ID( u1_AppliNo ) );

			//アプリケーション削除
			P_Appli->Finalizer();			//LDTとTSSをGDTから削除
			G_GMA.Delete( ui_AppliGMID );	//アプリケーションのメモリ領域の開放

			//タスクリスト切り離し
			DP( "%#x, %#x, %#x", G_TM.GetNowTask(), P_Appli, u1_AppliNo );
			G_TM.SleepTask( P_Appli );
		}
		G_TM.EnableTS();		//タスクスイッチ許可

		DP( "APM::Main()" );
		continue;		//もう一度実行。
	}
}


/*******************************************************************************
	概要	：	アプリケーション実行開始
	説明	：	ファイルパスで指定されたアプリケーションを起動します。
				アプリケーションヘッダにかかれている情報を元にメモリ領域を確保し、
				タスクの生成などを行った後、アプリケーションIDをアプリケーションのmain関数の引数として渡します。
	Include	：	APM.h
	引数	：	const char* CPc_AppliFP		アプリケーションのファイルパス
				[ui* Pui_AppliID = NULL]	アプリケーションIDの格納先
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		APM::StartAppli( const char* CPc_AppliFP, ui* Pui_AppliID )
{
	ui		ui_AppliNo;		//アプリケーションの配列番号
	ui		ui_AppliID;		//アプリケーションID
	ui		ui_AppliGMID;	//アプリケーションのメモリID(グローバル)
	AppliHeader	AplHead;	//アプリケーションヘッダ情報
	AMSI	AMSI;	//アプリケーション・メモリ構造情報


	//使っていない番号を検索
	if( ( ui_AppliNo = SearchFree() ) >= MAX_NUM_APPLI )
		return ERROR_APPLI_FULLUSED;		//ポインタ配列が空いていない

	//アプリヘッダの読み取り
	if( G_VFS.Read( CPc_AppliFP, 0, &AplHead, sizeof AplHead ) < 0 )
		return ERROR_READ_FAILED;		//読み取り失敗

	//アプリケーション構造情報初期化
	AMSI.Init( AplHead );

	//メモリ確保
	if( !( ui_AppliGMID = G_GMA.Create( AMSI.GetAllocSize() ) ) )
		return ERROR_GMA_FAILED;			//メモリ確保失敗

	//確保したメモリのベースをセット
	AMSI.SetBase( G_GMA.GetBase( ui_AppliGMID ) );

	//アプリケーション本体をメモリに展開
	if( G_VFS.Read( CPc_AppliFP, sizeof AplHead, AMSI.GetBodyBase(), AMSI.GetBodySize() ) < 0 )
	{
		G_GMA.Delete( ui_AppliGMID );
		return ERROR_READ_FAILED;		//読み取り失敗
	}

	//アプリケーションタスク初期化
	SegmInfo	A_SegmInfo[2];
	AMSI.GetLDTInfo( A_SegmInfo );
	if( AMSI.GetAplClassBase()->Init(
			AMSI.GetEIP(), AMSI.GetESP() - 8, AMSI.GetESP0(),
			CPc_AppliFP, A_SegmInfo ) < 0 )		//アプリケーションタスク初期化
		return ERROR_APPLIINIT_FAILED;

	//アプリケーションのメモリIDを格納
	MAui_AppliGMID[ui_AppliNo] = ui_AppliGMID;

	//アプリケーションをタスクリストに追加
	if( G_TM.AddTask( AMSI.GetAplClassBase() ) < 0 )
		return ERROR_ADDTASK_FAILED;

	//アプリケーションIDをアプリケーションに渡す
	ui_AppliID = No2ID( ui_AppliNo );
	*(ui*)( (u1*)AMSI.GetBodyBase() + AMSI.GetESP() - 4 ) = ui_AppliID;

	//アプリケーションIDを返却
	if( Pui_AppliID != NULL )
		*Pui_AppliID = ui_AppliID;

	return SUCCESS;
}


/*******************************************************************************
	概要	：	アプリケーション終了
	説明	：	アプリケーションを終了させます
	Include	：	APM.h
	引数	：	ui ui_AppliID	アプリケーションID
	戻り値	：	void
*******************************************************************************/
void	APM::QuitAppli( ui ui_AppliID )
{
	ui		ui_AppliNo;

	//IDから配列番号に変換
	ui_AppliNo = ID2No( ui_AppliID );

	//エラー処理
	if( ui_AppliNo & ~0xff )		//アプリケーションIDが無効
		return;

	G_TM.DisableTS();		//タスクスイッチ禁止
	M_QuitAppliQ.Enqueue( (u1)ui_AppliNo );		//アプリケーションの配列番号をキューイング
	G_TM.WakeupTask( GP_APMTask );
	G_TM.EnableTS();		//タスクスイッチ許可

	//自分自身なら返らない。
	if( G_TM.GetNowTask() == GetAppli( ui_AppliID ) )
		while( true );
}


/*******************************************************************************
	概要	：	アプリケーションクラスポインタ取得
	説明	：	アプリケーションIDから、アプリケーションクラスのポインタを取得します。
	Include	：	APM.h
	引数	：	ui ui_AppliID	アプリケーションID
	戻り値	：	Appli*
*******************************************************************************/
Appli*	APM::GetAppli( ui ui_AppliID )
{
	return (Appli*)G_GMA.GetBase( MAui_AppliGMID[ID2No( ui_AppliID )] );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	アプリケーション管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	空き検索
	説明	：	アプリケーションポインタの空きを検索します。
	Include	：	APM.h
	引数	：	void
	戻り値	：	ui		空き番号(一つも空いていなければ、MAX_NUM_APPLIを返す)
*******************************************************************************/
ui		APM::SearchFree( void )
{
	ui		ui_AppliSearchCount;		//アプリケーションポインタ配列の空き検索カウンタ

	//アプリケーションポインタ配列の空きを検索
	for( ui_AppliSearchCount = 0; ui_AppliSearchCount < MAX_NUM_APPLI && MAui_AppliGMID[ui_AppliSearchCount] != 0; ui_AppliSearchCount++ )
	{}

	return ui_AppliSearchCount;
}


/*******************************************************************************
	概要	：	配列要素番号からIDへ変換
	説明	：	配列の要素番号をアプリケーションを扱うためのIDへ変換します。
	Include	：	MM.h
	引数	：	ui ui_No		配列の要素番号
	戻り値	：	ui		アプリケーションを扱うためのID
*******************************************************************************/
ui		APM::No2ID( u1 u1_No )
{
	ui		ui_ID = u1_No;
	ui_ID	^= ID_NO_EOR;
	ui_ID	^= u1_No << 11;
	ui_ID	^= u1_No << 22;
	return ui_ID;
}


/*******************************************************************************
	概要	：	IDから配列要素番号へ変換
	説明	：	アプリケーションを扱うためのIDを配列の要素番号へ変換します。
	Include	：	MM.h
	引数	：	ui ui_ID		アプリケーションを扱うためのID
	戻り値	：	ui		配列の要素番号
*******************************************************************************/
ui		APM::ID2No( ui ui_ID )
{
	ui		ui_No = ui_ID;
	u1		u1_No;
	ui_No	^= ID_NO_EOR;
	u1_No	= ui_No;
	ui_No	^= u1_No << 11;
	ui_No	^= u1_No << 22;
	return ui_No;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

