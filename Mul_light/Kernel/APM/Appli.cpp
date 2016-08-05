/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/Appli.cpp
	概要		：	アプリケーションクラス
	詳細		：	一つのアプリケーションを管理するクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Task.h"
#include	"Appli.h"
#include	"VFS.h"
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern VFS		G_VFS;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	アプリケーションクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	アプリケーションクラスの初期化
	説明	：
	Include	：	Appli.h
	引数	：	u4 u4_EIP
				u4 u4_ESP
				u4 u4_ESP0
				const char* CPc_AppliFP	アプリケーションのファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Appli::Init( u4 u4_EIP, u4 u4_ESP, u4 u4_ESP0, const char* CPc_AppliFP, SegmInfo* P_SegmInfoTable )
{
	s4		s4_Return;

	//ファイルパスコピー
	strncpy( MAc_FilePath, CPc_AppliFP, sizeof MAc_FilePath - 1 );

	//ファイルパス最適化
	if( G_VFS.OptimFP( MAc_FilePath ) < 0 )
		return ERROR_INVALID_FP;

	//ファイルパスからファイル名を取得
	char*	Pc_FileName;
	if( ( Pc_FileName = strrchr( MAc_FilePath, '/' ) ) == NULL )
		return ERROR_INVALID_FP;

	//タスク初期化
	TSS::Init();
	Mu4_EIP		= u4_EIP;
DP( "Mu4_EIP:%#x", Mu4_EIP );
	Mu4_ESP		= u4_ESP;
DP( "Mu4_ESP:%#x", Mu4_ESP );
	Mu4_ESP0	= u4_ESP0;
DP( "Mu4_ESP0:%#x", Mu4_ESP0 );
	M_SS0		= KERNEL_DS;
	M_Flag16.Init();		//フラグ初期化
	if( ( s4_Return = SetCaption( Pc_FileName + 1 ) ) < 0 )		//キャプション設定
		return s4_Return;
	SetAllottedTime();		//タスク持ち時間設定

	//TSSディスクリプタの作成
	SegmInfo	TSSInfo;		//TSSディスクリプタ作成用情報
	TSSInfo.u4_Base			= (u4)this;
	TSSInfo.u20b_Limit		= sizeof (Appli) - 1;
	TSSInfo.u5b_Type		= G_GDT.TYPE_TSS_32;
	TSSInfo.u2b_DPL			= G_GDT.DPL0;
	TSSInfo.u1b_Granularity	= G_GDT.GRA_BYTE;
	G_GDT.SetSegment( TSSInfo, &M_TSSSelector );		//TSSディスクリプタの作成

	//LDTセグメント・ディスクリプタの作成
	SegmInfo	LDTInfo;		//LDTディスクリプタ作成用情報
	LDTInfo.u4_Base			= (u4)&M_LDT;
	LDTInfo.u20b_Limit		= sizeof (LDT) - 1;
	LDTInfo.u5b_Type		= G_GDT.TYPE_LDT;
	LDTInfo.u2b_DPL			= G_GDT.DPL0;
	LDTInfo.u1b_Granularity	= G_GDT.GRA_BYTE;
	G_GDT.SetSegment( LDTInfo, &Mu2_LDT );		//LDTセグメント・ディスクリプタの作成

	//LDTのセットアップ、終了
	return SetupLDT( P_SegmInfoTable );
}


/*******************************************************************************
	概要	：	アプリケーションクラスの終了処理
	説明	：	LDTをGDTから削除し、タスクの終了処理を呼び出します。
	Include	：	Appli.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Appli::Finalizer( void )
{
	G_GDT.ClearDescriptor( Mu2_LDT );
	Task::Finalizer();
}


/*******************************************************************************
	概要	：	LDTのセットアップ
	説明	：	LDTを初期化し、指定の情報でLDTを設定します。
				コードセグメントやデータセグメントなどもついでにすべて設定します。
	Include	：	Task.h
	引数	：	SegmInfo* P_SegmInfoTable	セグメント情報テーブル(要素数は2)
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Appli::SetupLDT( SegmInfo* P_SegmInfoTable )
{
	SELECTOR	CS, DS;

s4	s4_Return;

	M_LDT.Init();		//LDT初期化

	if( ( s4_Return = M_LDT.SetSegment( 0, P_SegmInfoTable[0], &CS ) ) < 0 )
		return ERROR_LDTERROR;
	if( ( s4_Return = M_LDT.SetSegment( 1, P_SegmInfoTable[1], &DS ) ) < 0 )
		return ERROR_LDTERROR;

	M_CS = CS;
	M_ES = M_SS = M_DS = M_FS = M_GS = DS;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ローカルアドレス取得
	説明	：	アプリケーション上のアドレスをカーネル上のアドレスに変換します。
	Include	：	Task.h
	引数	：	void** PPv_Address		変換対象のアドレス
	戻り値	：	void*		ローカルのベースアドレス
*******************************************************************************/
void*	Appli::GetLocalBase( void )
{
	SegmInfo	DataSegmInfo = M_LDT.GetSegmInfo( M_GS );		//セグメント情報取得(DS,ESはカーネルになっている。)
	return (void*)DataSegmInfo.u4_Base;
}


/*******************************************************************************
	概要	：	ファイルパス取得
	説明	：	アプリケーションがあるファイルパスを返します
	Include	：	Appli.h
	引数	：	char* Pc_FPDest			ファイルパス格納先
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Appli::GetFilePath( char* Pc_FPDest )
{
	//エラー処理
	if( Pc_FPDest == NULL )
		return ERROR_NULLPOINTER;

	strcpy( Pc_FPDest, MAc_FilePath );
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

