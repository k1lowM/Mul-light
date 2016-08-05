/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PCI/PCI.cpp
	概要		：	PCIクラス
	詳細		：	PCIを管理するクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	デバイスデータ仕様
	u4_Type		：
	u4_Data1	：
	u4_Data2	：	予約
	u4_Data3	：	予約
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■as
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"PCI.h"

#include	<string.h>

//デバッグ用
#include	<stdio.h>
#include	"GUI/GUI.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	I/Oポート
*******************************************************************************/
#define	PORT_PCI_ADDR		( 0x0cf8 )		//アドレス指定レジスタ
#define	PORT_PCI_DATA		( 0x0cfc )		//データレジスタ


//コンフィグアドレス指定
#define	REGI_ADDR_MASK	( 0x000000fc )
#define	FUNC_NO_MASK	( 0x00000700 )
#define	DEV_NO_MASK		( 0x0000f800 )
#define	BUS_NO_MASK		( 0x00ff0000 )


//0x0c~0x0f
#define	MULTI_FUNC_MASK	( 0x00800000 )
#define	MULTI_FUNC		( 0x00800000 )



#define	NUM_FUNC		( 8 )		//ファンクション数


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//PCI		G_PCI;				//PCIクラスの実体

extern DM	G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PCIクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	PCIクラスの初期化
	説明	：
	Include	：	PCI.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	PCI::Init( void )
{
	u4		u4_DevNo, u4_BusNo, u4_FuncNo;

	//データベース初期化
	memset( M_PCIDevice, 0xff, sizeof M_PCIDevice );

	//PCIデバイスを検索
	//----------------------------------------------------------
		//バス番号検索
		for( u4_BusNo = 0; u4_BusNo < 0x100; u4_BusNo++ )
		{
			//デバイスID検索
			for( u4_DevNo = 0; u4_DevNo < 0x20; u4_DevNo++ )
			{
				//ベンダID、デバイスIDチェック
				IO::Out4( PORT_PCI_ADDR, ConfAddr( true, u4_BusNo, u4_DevNo, 0, 0x00 ) );		//アドレス指定
				if( IO::In4( PORT_PCI_DATA ) == 0xffffffff )		//ベンダID、デバイスIDともに0xffffなら、接続なし。次のデバイスへ。
					continue;

				//データベースに登録
				RegiDevice( u4_BusNo, u4_DevNo, 0 );

				//マルチファンクションデバイスチェック
				IO::Out4( PORT_PCI_ADDR, ConfAddr( true, u4_BusNo, u4_DevNo, 0, 0x0c ) );		//アドレス指定
				if( ( IO::In4( PORT_PCI_DATA ) & MULTI_FUNC_MASK ) != MULTI_FUNC )		//単機能なら次のデバイスへ。
					continue;

				//他の機能番号も調べる
				for( u4_FuncNo = 1; u4_FuncNo < NUM_FUNC; u4_FuncNo++ )
				{
					//ベンダID、デバイスIDチェック
					IO::Out4( PORT_PCI_ADDR, ConfAddr( true, u4_BusNo, u4_DevNo, u4_FuncNo, 0x00 ) );		//アドレス指定
					if( IO::In4( PORT_PCI_DATA ) == 0xffffffff )		//ベンダID、デバイスIDともに0xffffなら、接続なし。次のファンクションへ。
						continue;

					//データベースに登録
					RegiDevice( u4_BusNo, u4_DevNo, u4_FuncNo );
				}
			}
		}
	//----------------------------------------------------------
}


/*******************************************************************************
	概要	：	デバイス制御
	説明	：	デバイスの制御を統括して行う関数です。
	Include	：	PCI.h
	引数	：	DCD* P_DCD
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PCI::Ctrl( DCD* P_DCD )
{
	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	レジスタ読み取り
	説明	：	レジスタ・アドレスを指定し、データを読み取ります。
	Include	：	PCI.h
	引数	：	u1 u1_ID		デバイスID
				u1 u1_addr		PCIコンフィグレーションレジスタ・アドレス
	戻り値	：	u4		読み取りデータ
*******************************************************************************/
u4		PCI::Read( u1 u1_ID, u1 u1_Addr )
{
	IO::Out4( PORT_PCI_ADDR, ConfAddr( u1_ID, true, u1_Addr ) );		//アドレス指定
	return IO::In4( PORT_PCI_DATA );
}


/*******************************************************************************
	概要	：	レジスタ書き込み
	説明	：	レジスタ・アドレスを指定し、データを書き込みます。
	Include	：	PCI.h
	引数	：	u1 u1_ID		デバイスID
				u1 u1_Addr		PCIコンフィグレーションレジスタ・アドレス
				u4 u4_Data		書き込むデータ
	戻り値	：	void
*******************************************************************************/
void	PCI::Write( u1 u1_ID, u1 u1_Addr, u4 u4_Data )
{
	IO::Out4( PORT_PCI_ADDR, ConfAddr( u1_ID, true, u1_Addr ) );		//アドレス指定
	IO::Out4( PORT_PCI_DATA, u4_Data );
}


/*******************************************************************************
	概要	：	デバイス検索・識別番号取得
	説明	：	クラスコードのうち、クラスコードマスクの部分が一致するデバイスのIDを返します。
				サイズは、IDを返却するバッファのサイズで、終了時はヒットした数になります。
				IDは、以降の読み取りや書き込みで利用します。
	Include	：	PCI.h
	引数	：	u4 u4_CC			クラスコード
				u4 u4_CCMask		クラスコードマスク
				u1* Pu1_IDBuf		ID返却ポインタ(配列)out：ID
				ui* Pui_NumID		in：配列の最大要素数、out：ヒット数
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		PCI::SearchDevice( u4 u4_CC, u4 u4_CCMask, u1* Pu1_IDBuf, ui* Pui_NumID )
{
	ui		ui_No;
	ui		ui_MaxSize;

	//エラー処理
	if( Pu1_IDBuf == NULL && Pui_NumID == NULL )
		return ERROR_NULLPOINTER;

	ui_MaxSize	= *Pui_NumID;
	*Pui_NumID	= 0;

	//検索
	for( ui_No = 0; ui_No < NUM_DEVICE; ui_No++ )
	{
		//ヒット！
		if( !( ( M_PCIDevice[ui_No].u4_CC ^ u4_CC ) & u4_CCMask ) )
		{
			//バッファのサイズ未満なら番号を入れる
			if( ui_MaxSize > *Pui_NumID )
				Pu1_IDBuf[*Pui_NumID] = ui_No;

			*Pui_NumID += 1;
		}
	}

	//指定のクラスコードのデバイスは無い。
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	PCIクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	コンフィグアドレスの計算
	説明	：	引数のパラメータからコンフィグアドレスを作成します。
				引数の値が大きい場合、オーバーフローした部分の情報は削除されます。
	Include	：	PCI.h
	引数	：	bool b_DataEnable	データレジスタ有効フラグ
				u4 u4_BusNo			バス番号			0～255
				u4 u4_DevNo			デバイスID		0～31
				u4 u4_FuncNo		機能番号			0～7
				u4 u4_RegiAddr		レジスタアドレス	0～255(4の倍数のみ)
	戻り値	：	u4		コンフィグレーション・レジスタ・アドレス
*******************************************************************************/
u4		PCI::ConfAddr( bool b_DataEnable, u4 u4_BusNo, u4 u4_DevNo, u4 u4_FuncNo, u4 u4_RegiAddr )
{
	u4		u4_ConfAddr = 0;
	u4_ConfAddr |= (u4)b_DataEnable << 31;
	u4_ConfAddr |= ( u4_BusNo << 16 ) & BUS_NO_MASK;
	u4_ConfAddr |= ( u4_DevNo << 11 ) & DEV_NO_MASK;
	u4_ConfAddr |= ( u4_FuncNo << 8 ) & FUNC_NO_MASK;
	u4_ConfAddr |= u4_RegiAddr & REGI_ADDR_MASK;
	return u4_ConfAddr;
}
/*******************************************************************************
	概要	：	コンフィグアドレスの計算
	説明	：	引数のパラメータからコンフィグアドレスを作成します。
	Include	：	PCI.h
	引数	：	u1 u1_ID			デバイスID
				bool b_DataEnable	データレジスタ有効フラグ
				u4 u4_RegiAddr		レジスタアドレス	0～255(4の倍数のみ)
	戻り値	：	u4		コンフィグレーション・レジスタ・アドレス
*******************************************************************************/
u4		PCI::ConfAddr( u1 u1_ID, bool b_DataEnable, u4 u4_RegiAddr )
{
	u4		u4_ConfAddr = 0;
	u4_ConfAddr |= (u4)b_DataEnable << 31;
	u4_ConfAddr |= M_PCIDevice[u1_ID].u2_BDF << 8;
	u4_ConfAddr |= u4_RegiAddr & REGI_ADDR_MASK;
	return u4_ConfAddr;
}


/*******************************************************************************
	概要	：	デバイスデータの登録
	説明	：	検索して発見したデバイスをデータベースに登録します。
	Include	：	PCI.h
	引数	：	u4 u4_BusNo		バス番号
				u4 u4_DevNo		デバイスID
				u4 u4_FuncNo	機能番号
	戻り値	：	void
*******************************************************************************/
void	PCI::RegiDevice( u4 u4_BusNo, u4 u4_DevNo, u4 u4_FuncNo )
{
	u4		u4_ConfAddr = ConfAddr( true, u4_BusNo, u4_DevNo, u4_FuncNo, 0x08 );
	u4		u4_CC;
	u1		u1_DeviceID;

	IO::Out4( PORT_PCI_ADDR, u4_ConfAddr );		//アドレス指定
	u4_CC = IO::In4( PORT_PCI_DATA ) >> 8;		//クラスコード取得
	M_DeviceType = CC2DT( u4_CC );				//デバイスの種類を登録
	G_DM.RegiDevice( &u1_DeviceID, this );		//デバイス管理に登録、ID取得

	M_PCIDevice[u1_DeviceID].u4_CC		= u4_CC;
	M_PCIDevice[u1_DeviceID].u2_BDF		= u4_ConfAddr >> 8;

//	DP( "DevNo:%#02x, Bus:%#02x, Dev:%#02x, Func:%#02x, CC:%06x", u1_DeviceID, u4_BusNo, u4_DevNo, u4_FuncNo, M_PCIDevice[u1_DeviceID].u4_CC );
}


/*******************************************************************************
	概要	：	クラスコードからデバイスタイプに変換
	説明	：	クラスコードをMul lightのデバイスタイプに変換します。
				デバイスタイプは、APIのDevice.hを参照のこと。
	Include	：	PCI.h
	引数	：	u4 u4_CC		クラスコード
	戻り値	：	DT::DeviceType	デバイスタイプ
*******************************************************************************/
DT::DevType	PCI::CC2DT( u4 u4_CC )
{
	switch( u4_CC & CC::BC_MASK )
	{
	//大容量記憶デバイスコントローラ
	case CC01::BASECLASS:
		if( ( u4_CC & CC01::BUSMASTER_IDE_MASK ) == CC01::BUSMASTER_IDE )
			return DT::BUSMASTER_IDE;
		break;

	//ネットワークコントローラ
	case CC02::BASECLASS:
		switch( u4_CC )
		{
		case CC02::ETHERNET:
			return DT::ETHERNET;
		}
		break;
	}

	//該当無し
	return DT::CLASS_OTHER;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

