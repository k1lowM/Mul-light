/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/PCI/PCI.h
	概要		：	PCIクラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Driver.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace	CC
{
	enum	ClassCode
	{
		BC_MASK			= 0xff0000,			//ベースクラスマスク
		SC_MASK			= 0x00ff00,			//サブクラスマスク
		PIF_MASK		= 0x0000ff,			//プログラムインターフェースマスク

		//該当無し
		CLASS_OTHER				= 0xff0000,
	};
}


//●0x00:クラスコード適用前デバイス(古いデバイス)●
namespace	CC00
{
	enum	ClassCode00
	{
		BASECLASS				= 0x000000,
	};
}


//●0x01:大容量記憶デバイスコントローラ●
namespace	CC01
{
	enum	ClassCode01
	{
		BASECLASS				= 0x010000,
		BUSMASTER_IDE_MASK		= 0xffff80,
		NONBUSMASTER_IDE		= BASECLASS | 0x0100,
		BUSMASTER_IDE			= BASECLASS | 0x0180,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x02:ネットワークコントローラ●
namespace	CC02
{
	enum	ClassCode02
	{
		BASECLASS				= 0x020000,
		ETHERNET				= BASECLASS | 0x0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x03:VGA互換●
namespace	CC03
{
	enum	ClassCode03
	{
		BASECLASS				= 0x030000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x04:マルチメディアコントローラ●
namespace	CC04
{
	enum	ClassCode04
	{
		BASECLASS				= 0x040000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x05:メモリ・コントローラ●
namespace	CC05
{
	enum	ClassCode05
	{
		BASECLASS				= 0x050000,
		RAM						= BASECLASS | 0x0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x06:ブリッジ●
namespace	CC06
{
	enum	ClassCode06
	{
		BASECLASS				= 0x060000,
		HOST_BRIDGE				= BASECLASS | 0x0000,
		ISA_BRIDGE				= BASECLASS | 0x0100,
		PCI_PCI_BRIDGE			= BASECLASS | 0x0400,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x07:通信ポートコントローラ●
namespace	CC07
{
	enum	ClassCode07
	{
		BASECLASS				= 0x070000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x08:システムデバイス●
namespace	CC08
{
	enum	ClassCode08
	{
		BASECLASS				= 0x080000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x09:入力デバイス●
namespace	CC09
{
	enum	ClassCode09
	{
		BASECLASS				= 0x090000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0a:ドッキングステーション？●
namespace	CC0a
{
	enum	ClassCode0a
	{
		BASECLASS				= 0x0a0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0b:プロセッサ●
namespace	CC0b
{
	enum	ClassCode0b
	{
		BASECLASS				= 0x0b0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0c:シリアルバスコントローラ●
namespace	CC0c
{
	enum	ClassCode0c
	{
		BASECLASS				= 0x0c0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0d:ワイヤレスコントローラ●
namespace	CC0d
{
	enum	ClassCode0d
	{
		BASECLASS				= 0x0d0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0e:インテリジェントIOコントローラ●
namespace	CC0e
{
	enum	ClassCode0e
	{
		BASECLASS				= 0x0e0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x0f:衛星通信コントローラ●
namespace	CC0f
{
	enum	ClassCode0f
	{
		BASECLASS				= 0x0f0000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x10:暗号化／復号化コントローラ●
namespace	CC10
{
	enum	ClassCode10
	{
		BASECLASS				= 0x100000,
		OTHER					= BASECLASS | 0x8000,
	};
}


//●0x11:データ収集／信号処理コントローラ●
namespace	CC11
{
	enum	ClassCode11
	{
		BASECLASS				= 0x110000,
		OTHER					= BASECLASS | 0x8000,
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	PCIデバイス情報構造体
*******************************************************************************/
class	PCIDevInfo
{
//変数
public:
	u4		u4_CC;			//クラスコード
	u2		u2_BDF;			//アクセス情報(バス番号、デバイスID、機能番号)
	u1		u1_Padding[2];
};


/*******************************************************************************
	PCIクラス
*******************************************************************************/
class	PCI : public Driver
{
//定数
public:
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,		//ヌルポインタエラー

		SUCCESS = _SUCCESS_,
	};

private:
	enum
	{
		NUM_DEVICE		= 256,
	};

//変数
private:
	PCIDevInfo	M_PCIDevice[NUM_DEVICE];		//PCIデバイス情報

//メソッド
public:
	void	Init( void );
	s4		Ctrl( DCD* P_DCD );

	u4		Read( u1 u1_ID, u1 u1_Addr );				//レジスタ読み取り
	void	Write( u1 u1_ID, u1 u1_Addr, u4 u4_Data );	//レジスタ書き込み

	s4		SearchDevice( u4 u4_CC, u4 u4_CCMask, u1* Pu1_IDBuf, ui* Pui_NumID );		//デバイス検索・識別番号取得

private:
	u4		ConfAddr( bool b_DataEnable, u4 u4_BusNo, u4 u4_DevNo, u4 u4_FuncNo, u4 u4_RegiAddr );		//コンフィグレーション・レジスタ・アドレス計算
	u4		ConfAddr( u1 u1_ID, bool b_DataEnable, u4 u4_RegiAddr );
	void	RegiDevice( u4 u4_BusNo, u4 u4_DevNo, u4 u4_FuncNo );		//検索で発見したデバイスをデータベースに登録する。
	DT::DevType	CC2DT( u4 u4_CC );			//クラスコードをデバイスタイプに変換
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
