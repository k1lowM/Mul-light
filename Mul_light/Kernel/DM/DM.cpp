/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/DM.cpp
	概要		：	デバイス管理クラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"VFS.h"
#include	<string.h>


//ここにドライバのヘッダファイルのインクルードを記述。
//------------------------------------------------------------------------------
#include	"PIC/PIC.h"
#include	"PIT/PIT.h"
#include	"RTC/RTC.h"
#include	"PCI/PCI.h"

#include	"PS2Keyboard/PS2Keyboard.h"
#include	"PS2Mouse/PS2Mouse.h"
//#include	"ATA/BMIDE.h"
#include	"ATA/ATA.h"

//------------------------------------------------------------------------------



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
GDDM		G_GDDM;
GDDQ		G_GDDQ;

extern DM		G_DM;
extern VFS		G_VFS;



//ここにドライバの実体を宣言する。
//----------------------------------------------------------
PIC			G_PIC;
PIT			G_PIT;
RTC			G_RTC;
PCI			G_PCI;

PS2Keyboard	G_PS2Keyboard;
PS2Mouse	G_PS2Mouse;
//BMIDE		G_BMIDE;
ATA			G_PriATA;
ATA			G_SecATA;

//----------------------------------------------------------



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デバイス管理クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバイス管理クラスの初期化(システム)
	説明	：
	Include	：	DM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	DM::SysInit( void )
{
	//データベース初期化
	memset( M_DevInfo, 0, sizeof M_DevInfo );

	//システムデバイス登録
	RegiSysDev( SD_PIC, &G_PIC );
	RegiSysDev( SD_PIT, &G_PIT );
	RegiSysDev( SD_RTC, &G_RTC );
	RegiSysDev( SD_PCI, &G_PCI );

	//システム・デバイス初期化
	G_PIC.Init();
	G_PIT.Init();
	G_RTC.Init();
	G_PCI.Init();

	DP( "DM::SysInit()\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	デバイス管理クラスの初期化(標準)
	説明	：
	Include	：	DM.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	DM::Init( void )
{
	//ここにドライバの初期化を記述する。
	//----------------------------------------------------------
	G_PS2Keyboard.Init();
	G_PS2Mouse.Init();
//	G_BMIDE.Init();
	G_PriATA.Init( ATA::PRIMARY );
	G_SecATA.Init( ATA::SECONDARY );
	//----------------------------------------------------------

	DP( "DM::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：
	説明	：
	Include	：	DM.h
	引数	：	DCD* P_DCD			制御データポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DM::Ctrl( DCD* P_DCD )
{
	u1		u1_DeviceID = P_DCD->GetDeviceID();
	//エラー処理
	if( M_DevInfo[u1_DeviceID].P_Driver == NULL )
		return ERROR_DEVNOTHING;

	//とりあえず、今は何もしない。
	//switchでタイプを見て判断するか。。。
	return 0;
}


/*******************************************************************************
	概要	：	デバイス登録
	説明	：	デバイスドライバをデバイス管理に登録します。
	Include	：	DM.h
	引数	：	u1* Pu1_DeviceID			IDを返すポインタ
				Driver* P_Driver			デバイスドライバポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DM::RegiDevice( u1* Pu1_DeviceID, Driver* P_Driver )
{
	ui		ui_Count;

	//エラー処理
	if( Pu1_DeviceID == NULL || P_Driver == NULL )		//NULLポインタ
		return ERROR_NULLPOINTER;

	//空きを検索
	for( ui_Count = NUM_SD; M_DevInfo[ui_Count].P_Driver != NULL; ui_Count++ )
		if( ui_Count >= NUM_DEVICE )
			return ERROR_DITFULL;

	*Pu1_DeviceID = ui_Count;					//IDを返す
	M_DevInfo[ui_Count].P_Driver	= P_Driver;	//登録するデバイスのドライバを登録
	M_DevInfo[ui_Count].P_FS		= NULL;
	G_VFS.DistinctionFS( ui_Count );			//ファイルシステムの識別＆登録

	//デバッグ表示
	{
		char	c_Str[256];
		sprintf( c_Str, "Device ID:%#02x, Type:%#08x", ui_Count, P_Driver->GetDeviceType() );
		G_GUI.DebugPrintString( c_Str, 1, ui_Count + 12, 0xffffffff );
	}

	return SUCCESS;
}


/*******************************************************************************
	概要	：	デバイス情報更新
	説明	：	デバイスの情報を更新します。
				変更可能なものはドライバのポインタとデバイスの種類で、
				変更しない場合はNULLにします。
	Include	：	DM.h
	引数	：	u1 u1_DeviceID		デバイスID
				Driver* P_Driver	デバイスドライバポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DM::DevInfoUpdate( u1 u1_DeviceID, Driver* P_Driver )
{
	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	デバイス情報更新
	説明	：	デバイスの情報を更新します。
				変更可能なものは、ファイルシステムのメモリIDと
				ファイルシステムのポインタです。どちらも有効でなければいけません。
	Include	：	DM.h
	引数	：	u1 u1_DeviceID		デバイスID
				FS* P_FS			ファイルシステムクラスのポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DM::DevInfoUpdate( u1 u1_DeviceID, FS* P_FS )
{
	//エラー処理
	if( M_DevInfo[u1_DeviceID].P_Driver == NULL || P_FS == NULL )		//NULLポインタ
		return ERROR_NULLPOINTER;

	M_DevInfo[u1_DeviceID].P_FS = P_FS;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	デバイス抹消
	説明	：	デバイス管理からデバイスを抹消します。
	Include	：	DM.h
	引数	：	u1 u1_DeviceID			抹消するデバイスID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		DM::EraseDevice( u1 u1_DeviceID )
{
	//エラー処理
	if( u1_DeviceID >= NUM_DEVICE )
		return ERROR_OVERSIZEID;

	M_DevInfo[u1_DeviceID].P_Driver	= NULL;
	M_DevInfo[u1_DeviceID].P_FS		= NULL;

	return SUCCESS;
}



/*******************************************************************************
	概要	：	デバイス情報取得
	説明	：	デバイスの情報を取得します。
	Include	：	DM.h
	引数	：	u1 u1_DeviceID			デバイスID
	戻り値	：	DevInfo		デバイス情報
*******************************************************************************/
DevInfo	DM::GetDevInfo( u1 u1_DeviceID )
{
	return M_DevInfo[u1_DeviceID];
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デバイス管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	システムデバイス登録
	説明	：	システムデバイスのドライバをデバイス管理に登録します。
	Include	：	DM.h
	引数	：	u1 u1_DeviceID			デバイスID
				Driver* P_Driver		デバイスドライバポインタ
	戻り値	：	void
*******************************************************************************/
void	DM::RegiSysDev( u1 u1_DeviceID, Driver* P_Driver )
{
	M_DevInfo[u1_DeviceID].P_Driver			= P_Driver;		//登録するデバイスのドライバを登録
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
