/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：
	概要		：	バスマスタIDEクラスヘッダ
	詳細		：	バスマスタIDEコントローラを制御を管理するヘッダです。
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	構造体
*******************************************************************************/
namespace	Dr
{
	struct		PRD_Table
	{
	public:
		u4		u4_DestHeadAddr;
		u2		u2_ForwardSize;
		u2		u2_EndofTable;
	};
}

/*******************************************************************************
	バスマスタIDEクラス
*******************************************************************************/
namespace	Dr
{

	class	BMIDE : public Driver
	{
	//定数
	public:
		//エラー情報
		enum	Error
		{
			ERROR		= _ERROR_,	//0x80000001
			ERROR_NOBMID,

			SUCCESS		= _SUCCESS_,	//0x00000001
		};

		enum	CableType
		{
			BMIDE_NON	= 0,		//バスマスタIDEコントローラなし
			CABLE40,
			CABLE80
		};

		enum	RW
		{
			MEMORY_READ = 0x0,
			MEMORY_WRITE = 0x8,
		};

		enum	StartStop
		{
			STOP = 0x00,
			START = 0x01,
			START_STOP_MASK = 0x01,
		};

	private:
		enum	ClassCode
		{
			CC_MASK		= 0x0000000a,

			CC_UDMA33	= 0x00010180,	//bus master IDE controller (UDMA33)
			CC_ATA66	= 0x0001018a,	//bus master IDE controller (ATA66)
	//		CC_OTHER	= 0x0001018f,
		};

		enum
		{
			BMIDE_ONE = 1,				//バスマスタIDEコントローラが１つ
			BMIDE_NUM = 8,				//バスマスタIDEコントローラが１つ以上
		};


	//変数
	private:
		u4		Mu4_BusMasterBase;	//バスマスタIDEレジスタのベースアドレス
		u1		Mui_ID;				//PCIのID

	//メソッド
	public:
		BMIDE( void );					//コンストラクタ
		void	Init( void );			//初期化
		s4		Ctrl( DCD* P_DCD );			//デバイスコントロール
		u1		Read( u1 u1_Offset );
		void	Write( u1 u1_Offset, u1 u1_Data );

		s4		GetCableType( u1 u1_PriSec );	//ケーブルの種類(40芯or80芯)
		s4		UDMA_PrdSetup( u1 u1_PriSec, PRD_Table* P_Prd );
		s4		UDMA_RegiSetup( u1 u1_PriSec, RW RWctrl );
		s4		UDMA_TranStart( u1 u1_PriSec, StartStop StartStop_Flg );
		s4		GetActive(u1 u1_PriSec );

	private:
		u4		Access( ui ui_ID, u1 u1_OffSet );	//バスマスタIDEレジスタアクセス

	};
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
