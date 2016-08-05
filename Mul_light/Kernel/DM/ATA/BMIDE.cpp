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
#include	"BMIDE.h"

//デバッグ用
#ifdef DP_DEBUG
	#include	<stdio.h>
	#include	"../../GUI/GUI.h"
#endif



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
enum
{
	//レジスタオフセット
	OFFSET_BAR			= 0x20,			//ベースアドレスレジスタ
	OFFSET_CLASSCODE	= 0x8,			//クラスコード
	OFFSET_IEDCONF		= 0x54,			//上位IDEI/Oコンフィグレーションレジスタ
	BSADDR_MASK			= 0x0000fff0,	//ベースアドレス取得後のマスク
	PRICABLE_MASK		= 0x00000020,	//プライマリケーブルマスク
	SECCABLE_MASK		= 0x00000080,	//セカンダリケーブルマスク
};


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern Dr::BMIDE	G_BMIDE;
extern Dr::PCI		G_PCI;


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	BMIDEクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	BMIDEクラスのコンストラクタ
	説明	：
	Include	：	BMIDE.h
	引数	：	void
	戻り値	：	-
*******************************************************************************/
Dr::BMIDE::BMIDE( )
{
	//基本はコントローラなし
	Mu4_BusMasterBase = BMIDE_NON;
}


/*******************************************************************************
	概要	：	BMIDEクラスの初期化
	説明	：
	Include	：	BMIDE.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Dr::BMIDE::Init( void )
{
	ui		ui_NumID = BMIDE_NUM;			//IDの数
	u1		u1_IDBuf[BMIDE_NUM];
	u4		u4_CC = 0;
	u1		u1_Count,u1_CCcnt = 0;


	//バスマスタIDEの検索
	G_PCI.SearchDevice( CC01::BUSMASTER_IDE, CC01::BUSMASTER_IDE_MASK, u1_IDBuf, &ui_NumID );

	if( ui_NumID == 0 )
	{
		Mu4_BusMasterBase = BMIDE_NON;
		return;
	}

	//見つかったデバイスのクラスコードを検索
	for(u1_Count = 0; u1_Count < ui_NumID; u1_Count++)
	{
		u4_CC = Access( u1_IDBuf[u1_Count], OFFSET_CLASSCODE ) >> 8;
		if( u4_CC == CC_UDMA33 || u4_CC == CC_ATA66 )
			u1_CCcnt++;
	}

	if( u1_CCcnt == BMIDE_ONE)
	{
		Mui_ID = u1_IDBuf[u1_Count - 1];
		Mu4_BusMasterBase = Access( Mui_ID, OFFSET_BAR ) & BSADDR_MASK;
	}
	else
		Mu4_BusMasterBase = BMIDE_NON;

//	G_PCI.Write( Mui_ID, 0x3c, 0x0000000f );
//	DP("Int %#x",Access( Mui_ID, 0x3c ));
}


/*******************************************************************************
	概要	：	コントロール
	説明	：
	Include	：	BMIDE.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
s4		Dr::BMIDE::Ctrl( DCD* P_DCD )
{
	return 0;
}


/*******************************************************************************
	概要	：	リード
	説明	：
	Include	：	BMIDE.h
	引数	：	u1_Offset
	戻り値	：	void
*******************************************************************************/
u1	Dr::BMIDE::Read( u1 u1_Offset )
{
	return IO::In1( Mu4_BusMasterBase + u1_Offset );
}


/*******************************************************************************
	概要	：	ライト
	説明	：
	Include	：	BMIDE.h
	引数	：	u1_Offset
				u1_Data
	戻り値	：	void
*******************************************************************************/
void	Dr::BMIDE::Write( u1 u1_Offset, u1 u1_Data )
{
	if(Mu4_BusMasterBase == BMIDE_NON)
		return ;

	IO::Out1( Mu4_BusMasterBase + u1_Offset, u1_Data );
}


/*******************************************************************************
	概要	：	接続されているケーブルの種類を通知
	説明	：	接続されているケーブルの種類を通知します。
	Include	：	BMIDE.h
	引数	：	u1_PriSec
	戻り値	：	u1	ケーブルの種類
*******************************************************************************/
s4		Dr::BMIDE::GetCableType( u1 u1_PriSec )
{
	u4		u4_Cable;

	if(Mu4_BusMasterBase == BMIDE_NON)
		return ERROR_NOBMID;

	u4_Cable = Access( Mui_ID, OFFSET_IEDCONF );

	if( u1_PriSec == ATA::PRIMARY )
	{
		//プライマリケーブル判定
		if( u4_Cable & PRICABLE_MASK )
			return	CABLE80;
		else
			return	CABLE40;
	}
	else if( u1_PriSec == ATA::SECONDARY )
	{
		//セカンダリケーブル判定
		if( u4_Cable & SECCABLE_MASK )
			return CABLE80;
		else
			return CABLE40;
	}

	return	1;
}

/*******************************************************************************
	概要	：	PRDセットアップ
	説明	：	バスマスタIEDレジスにPRDを設定します。
	Include	：	BMIDE.h
	引数	：	u1_PriSec	プライマリ/セカンダリ オフセット
				P_Prd		PRD設定テーブル構造体ポインタ
	戻り値	：	エラー情報
*******************************************************************************/
s4		Dr::BMIDE::UDMA_PrdSetup( u1 u1_PriSec, PRD_Table* P_Prd )
{
	u4		u4_BMB = Mu4_BusMasterBase + u1_PriSec;

	if(Mu4_BusMasterBase == BMIDE_NON)
		return ERROR_NOBMID;;

	IO::Out1( u4_BMB + 4, ( u4 )( P_Prd ) );
	IO::Out1( u4_BMB + 5, ( u4 )( P_Prd ) >> 8 );
	IO::Out1( u4_BMB + 6, ( u4 )( P_Prd ) >> 16 );
	IO::Out1( u4_BMB + 7, ( u4 )( P_Prd ) >> 24 );

	return SUCCESS;
}


/*******************************************************************************
	概要	：	バスマスタIEDレジスタセットアップ
	説明	：	バスマスタ転送の転送方向設定とエラー&割り込みビットをクリアします。
	Include	：	BMIDE.h
	引数	：	u1_PriSec	プライマリ/セカンダリ オフセット
				u1_RWctrl
	戻り値	：	エラー情報
*******************************************************************************/
s4		Dr::BMIDE::UDMA_RegiSetup( u1 u1_PriSec, RW RWctrl )
{
	u4		u4_BMB  = Mu4_BusMasterBase + u1_PriSec;

	if(Mu4_BusMasterBase == BMIDE_NON)
		return ERROR_NOBMID;

	//転送方向設定
	IO::Out1( u4_BMB, RWctrl );

	//エラー&割り込みビットクリア
	IO::Out1( u4_BMB + 2, IO::In1( u4_BMB + 2) | 0x66 );

	return SUCCESS;
}


/*******************************************************************************
	概要	：	バスマスタ転送スタート
	説明	：	バスマスタIEDレジスタの転送Start/Stopビットを操作します。
	Include	：	BMIDE.h
	引数	：	u1_PriSec		プライマリ/セカンダリ オフセット
				u1_StartStop	スタート/ストップ フラグ
	戻り値	：	エラー情報
*******************************************************************************/
s4		Dr::BMIDE::UDMA_TranStart( u1 u1_PriSec, StartStop StartStop_Flg )
{
	u4		u4_BMB;
	u1		u1_InByte;

	//エラー処理
	if(Mu4_BusMasterBase == BMIDE_NON)
		return ERROR_NOBMID;

	u4_BMB = Mu4_BusMasterBase + u1_PriSec;

	u1_InByte = ( IO::In1( u4_BMB ) & ~START_STOP_MASK ) | StartStop_Flg;

	IO::Out1( u4_BMB, u1_InByte );

	return SUCCESS;
}


/*******************************************************************************
	概要	：	転送完了を通知
	説明	：	UDMAの転送完了を通知します。
	Include	：	BMIDE.h
	引数	：
	戻り値	：	エラー値
*******************************************************************************/
s4		Dr::BMIDE::GetActive( u1 u1_PriSec )
{
	u4		u4_BMB;
	u1		u1_InByte;

	if(Mu4_BusMasterBase == BMIDE_NON)
		return ERROR_NOBMID;

	u4_BMB = Mu4_BusMasterBase + u1_PriSec;

	u1_InByte = IO::In1( u4_BMB + 2 );
	if( (u1_InByte & 0x01) == 0 )
		return SUCCESS;

	return ERROR;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	BMIDEクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	バスマスタIDEレジスタアクセス
	説明	：	バスマスタIDEレジスタ内の指定されたビットにアクセスする関数です。
	Include	：	BMIDE.h
				u1_OffSet	バスマスタIDEレジスタのオフセット
	戻り値	：	u4			レジスタ値orエラー値
*******************************************************************************/
u4		Dr::BMIDE::Access( ui ui_ID, u1 u1_OffSet )
{
	u4	u4_Return;

	u4_Return = G_PCI.Read( ui_ID, u1_OffSet );

	return	u4_Return;
}


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

