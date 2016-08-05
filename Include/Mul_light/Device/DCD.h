/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	Include/Mul_light/Device/DCD.h
	概要		：	Mul light API - デバイス制御情報(Device Control Data)ヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	デバイス制御データクラス
	各種デバイス制御データクラスのベースクラスです。
*******************************************************************************/
class	DCD
{
//定数
public:
	//制御タイプ(0x8000****～はユーザ領域)
	enum	CtrlType
	{
		CTRL_READ		= 0x00010000,		//読み取り
		CTRL_WRITE		= 0x00020000,		//書き込み
	};

	//クラスタイプ(0x****8000～はユーザ領域)
	enum	ClassType
	{
		CLASS_CHAR = 1,		//キャラクタデバイス用
		CLASS_DATA,			//データデバイス用
	};

	//マスク
	enum	CtrlIDMask
	{
		CTRLTYPE_MASK		= 0xffff0000,		//制御タイプマスク
		CLASSTYPE_MASK		= 0x0000ffff,		//クラスタイプマスク
	};

//変数
protected:
	u4		Mu4_CtrlID;				//制御ID(31-12:任意の制御番号, 11-0:クラス番号)
	u1		Mu1_DeviceID;			//デバイスID
	u1		Mu1_Padding[sizeof (ui) - ( ( sizeof (u4) + sizeof (u1) ) % sizeof (ui) )];

//メソッド
public:
	u4		GetCtrlID( void );
	u1		GetDeviceID( void );
};



/*******************************************************************************
	クラスの種類
*******************************************************************************/
//マウスやキーボードなどのキャラクタデバイスに。
class	DCD_Char : public DCD
{
//変数
public:
	ui		ui_Data1;
	ui		ui_Data2;
	ui		ui_Data3;

//メソッド
public:
	void	Init( u4 u4_CtrlID, u1 u1_DeviceID );
};

//CDやHDDなどのデータデバイスに。
class	DCD_Data : public DCD
{
//変数
public:
	ui		ui_LBA;				//読み込み元の論理セクタ番号
	void*	Pv_Buffer;			//書き込み先のメモリアドレス(カーネルがアロックしたメモリなど)
	ui		ui_NumSector;		//処理する最大セクタ数(処理したセクタ数)

//メソッド
public:
	void	Init( u4 u4_CtrlID, u1 u1_DeviceID );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
