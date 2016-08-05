/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/ATA/ATA.cpp
	概要		：	ATAクラス
	詳細		：	ATA/ATAPIデバイス制御を管理するクラスです。
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■as
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"../../Common.h"
#include	"../DM.h"
#include	"../PCI/PCI.h"
#include	"../../TM/TM.h"
#include	"ATA.h"

//デバッグ用
#ifdef DP_DEBUG
	#include	<stdio.h>
	#include	"../../GUI/GUI.h"
#endif

#include	<string.h>


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define		DATASIZE	( 256 )
#define		CABLE_CALL	( 0x57 )

enum
{
	GET_DEV_STATE,
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	プロトタイプ宣言
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//割り込み
void	Int2E( ARG_CONST_REGI_STAT );	//プライマリ割り込みルーチン
void	Int2F( ARG_CONST_REGI_STAT );	//セカンダリ割り込みルーチン



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern DM			G_DM;
extern PIC		G_PIC;

extern ATA		G_PriATA;
extern ATA		G_SecATA;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	割り込みハンドラ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	プライマリ割り込みルーチン
	説明	：
	Include	：	ATA.h
	引数	：
	戻り値	：	void
*******************************************************************************/
void	Int2E( ARG_CONST_REGI_STAT )
{
	G_GUI.DebugPrintString( "PRIMARY Interrupt", 30, 31, 0xff0000ff );
	G_PIC.ACK( G_PIC.IRQ14_HDC1 );
}


/*******************************************************************************
	概要	：	セカンダリ割り込みルーチン
	説明	：
	Include	：	ATA.h
	引数	：
	戻り値	：	void
*******************************************************************************/
void	Int2F( ARG_CONST_REGI_STAT )
{
	G_GUI.DebugPrintString( "SECONDARY Interrupt", 30, 32, 0xff0000ff );
	G_PIC.ACK( G_PIC.IRQ15_HDC2 );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATAクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ATAクラスのコンストラクタ
	説明	：
	Include	：	ATA.h
	引数	：	u4_PSflg	プライマリ/セカンダリフラグ
	戻り値	：	-
*******************************************************************************/
ATA::ATA( void )
{
}


/*******************************************************************************
	概要	：	ATAクラスの初期化
	説明	：
	Include	：	ATA.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	ATA::Init( PriSec PSflg )
{
	s4		s4_Return = 0;
	u2		u2_IdentifyDeviceData[DATASIZE];
	u1		u4_DevCnt;

//	__asm__( "sti;" );

	//ベースアドレスなどの設定
	if( PSflg == PRIMARY )
	{
		Mu4_PriSecBase = 0x80;
		Mu4_CtrlRegsBase = 0x3F6;
		Mu1_BMOffset = PSflg;
	}
	else if( PSflg == SECONDARY )
	{
		Mu4_PriSecBase = 0;
		Mu4_CtrlRegsBase = 0x376;
		Mu1_BMOffset = PSflg;
	}

	//ソフトウェアリセット
	ATAReset();

	//接続確認(マスタ＆スレーブ)
	for( u4_DevCnt =0; u4_DevCnt < ATA_DEV_MAX; u4_DevCnt++ )
	{
#ifdef DP_DEBUG
		DP("ATA::Init");
#endif

		//シグネチャ発行
		s4_Return = Init_DeviceCheck( u4_DevCnt, u2_IdentifyDeviceData );
		if( s4_Return < 0 )
			continue;

#ifdef DP_DEBUG
		DP("Init_DeviceCheck");
#endif

		//デバイス転送モード判定
		s4_Return = Init_DeviceMode( u4_DevCnt, u2_IdentifyDeviceData );
		if( s4_Return < 0 )
			continue;

#ifdef DP_DEBUG
		DP("Init_DeviceMode");
#endif

		//デバイスをアイドルに移行
		if( Init_DeviceIdle( u4_DevCnt, u2_IdentifyDeviceData ) < 0 )
			continue;

#ifdef DP_DEBUG
		DP("Init_DeviceIdle");
#endif

		//デバイス登録
		switch( M_DevInfo[u4_DevCnt].u4_DeviceType & DEVICE_MASK )
		{
		case DEVICE_ATA:
			M_DeviceType = DT::HDD;		//ATAならHDD。
			break;
		case DEVICE_ATAPI:
			//ATAPIデバイス
			switch( M_DevInfo[u4_DevCnt].u4_DeviceType & DEV_MASK )
			{
			case DEV_CD_ROM:
				M_DeviceType = DT::CDD;		//CD
				break;
			default:
				M_DeviceType = DT::CLASS_OTHER;
				break;
			}
			break;
		default:
			M_DeviceType = DT::CLASS_OTHER;
			break;
		}

		//デバイス登録
		G_DM.RegiDevice( &M_DevInfo[u4_DevCnt].u1_DeviceID, this );
#ifdef DP_DEBUG
		DP( "RegiDevice %#x",M_DevInfo[u4_DevCnt].u1_DeviceID );
#endif
	}

//	DP( "[MASTER] u4_DeviceType:%#01x s4_TransferMode:%#01x",M_DevInfo[MASTER].u4_DeviceType, M_DevInfo[MASTER].s4_TransferMode);
//	DP( "[SLAVE] u4_DeviceType:%#01x s4_TransferMode:%#01x ",M_DevInfo[SLAVE].u4_DeviceType, M_DevInfo[SLAVE].s4_TransferMode);
}


/*******************************************************************************
	概要	：	デバイスコントロール
	説明	：
	Include	：	ATA.h
	引数	：	void
	戻り値	：
*******************************************************************************/
s4		ATA::Ctrl( DCD* P_DCD )
{
	DCD_Data*	P_DCData = (DCD_Data*)P_DCD;

	switch( P_DCData->GetCtrlID() )
	{
	case DCD::CTRL_READ | DCD::CLASS_DATA:
		return Read( P_DCData->GetDeviceID(), P_DCData->ui_LBA, P_DCData->Pv_Buffer, &P_DCData->ui_NumSector );
//	case GET_DEV_STATE:
//		GetDevState( P_DCData );
//		break;
	default:
		return ERROR_INVALIDCTRLID;		//無効な制御ID
	}
}


/*******************************************************************************
	概要	：	リード関数
	説明	：	ATAデバイスからデータを読み取ります。
	Include	：	ATA.h
	引数	：
	戻り値	：
*******************************************************************************/
s4		ATA::Read( u1 u1_DeviceID, ui ui_LBA, void* Pv_Buffer, ui* Pui_NumSector )
{
	s4		s4_Return,s4_DevNo;
	u4		u4_MaxLBA,u4_Size;

	//u1_DeviceIDのチェック
	if( ( s4_DevNo = IDComparNo(u1_DeviceID) ) == NON_CTRLID_ERR )
		return NON_CTRLID_ERR;

	//デバイスはリード可能か
	if( M_DevInfo[s4_DevNo].s4_TransferMode < 0 )
		return	NONCOMPLIANT_ERR;

	//デバイスの状態を確認
	if( ( s4_Return = MediaStatusCHK( s4_DevNo ) ) != MEDIA_RADDY)
		return s4_Return;

	//最大LBAとセクタサイズを取得
	if( (s4_Return = GetMediaInfo( s4_DevNo, &u4_MaxLBA, &u4_Size) ) < 0 )
		return s4_Return;

	//引数チェック
	if( ui_LBA > u4_MaxLBA )
		return ERROR_PARA;

	//セクタサイズ登録
	M_DevInfo[s4_DevNo].u4_DevSecSize = u4_Size;

	//リード処理
	return	PIO_Read( s4_DevNo, ui_LBA, Pv_Buffer, Pui_NumSector );

}



/*******************************************************************************
	概要	：	ATAの状態を返す
	説明	：
	Include	：	ATA.h
	引数	：	void
	戻り値	：
*******************************************************************************/
s4		ATA::GetDevState( DCD_Data* P_DCData )
{
	//未実装。。早くつくれ！！！
	return 0;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATAクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PIOリード関数
	説明	：	PIO転送方式でのデータ読み込み関数です。
	Include	：	ATA.h
	引数	：	u4_Device		デバイス選択
				ui ui_LBA		LBA
				Pv_Buffer		読み込んだデータを格納するバッファ
				ui_NumSector	セクタ数
	戻り値	：
*******************************************************************************/
s4		ATA::PIO_Read( u4 u4_Device, ui ui_LBA, void* Pv_Buffer, ui* Pui_NumSector )
{
	s4		s4_Return;

	s4_Return = PacketRead10( u4_Device, ui_LBA, Pv_Buffer, *Pui_NumSector );
	if( s4_Return < 0 )
	{
		u1		Au1_Buffer[19], u1_Cnt;

		//エラー戻り値
		DP( "Read10 ERR:%#x", s4_Return );

		//RequestSenseコマンド発行 エラー情報を表示
		PacketRequestSense( u4_Device, Au1_Buffer );
		for( u1_Cnt = 0; u1_Cnt < 18; u1_Cnt++ )
			DP( "Au1_Buffer[%02d]:%#x", u1_Cnt, Au1_Buffer[u1_Cnt] );

		//アイドル状態へ移行
		s4_Return = IdleDevice( u4_Device , DEVICE_ATAPI );
		if( s4_Return < 0 )
			return	IDLECMD_ERR;

		return	s4_Return;
	}

	*Pui_NumSector = s4_Return;

	//アイドル状態へ移行
	s4_Return = IdleDevice( u4_Device , DEVICE_ATAPI );
	if( s4_Return < 0 )
		return	IDLECMD_ERR;

	return	SUCCESS;
}


/*******************************************************************************
	概要	：	デバイスIDとデバイスナンバの比較
	説明	：	デバイスIDのデバイスがマスタかスレーブか調べます。
	Include	：	ATA.h
	引数	：	u1_DeviceID
	戻り値	：	エラー値/デバイスナンバ
*******************************************************************************/
s4		ATA::IDComparNo( u1 u1_DeviceID )
{
	if(M_DevInfo[MASTER].u1_DeviceID == u1_DeviceID)
		return MASTER;
	else if(M_DevInfo[SLAVE].u1_DeviceID == u1_DeviceID)
		return SLAVE;

	return	NON_CTRLID_ERR;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

