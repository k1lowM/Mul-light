/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：
	概要		：	ATAクラスヘッダ
	詳細		：	ATA/ATAPIデバイス制御を管理するヘッダです。
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Driver.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	PORT_DATA			(Mu4_PriSecBase | 0x170)	//Dataレジスタ
#define	PORT_ERROR			(Mu4_PriSecBase | 0x171)	//Errorレジスタ
#define	PORT_FEATURES		(Mu4_PriSecBase | 0x171)	//Featuresレジスタ
#define	PORT_SCT_COUNT		(Mu4_PriSecBase | 0x172)	//Sector Countレジスタ
#define	PORT_INTREASON		(Mu4_PriSecBase | 0x172)	//Interrupt Reasonレジスタ(PACKETコマンド時)
#define	PORT_SCT_NUMBER		(Mu4_PriSecBase | 0x173)	//Sector Numberレジスタ
#define	PORT_CYL_LO			(Mu4_PriSecBase | 0x174)	//Cylinder Lowレジスタ
#define	PORT_BYTECNT_LO		(Mu4_PriSecBase | 0x174)	//Byte Count LSBレジスタ(PACKETコマンド時)
#define	PORT_CYL_HI			(Mu4_PriSecBase | 0x175)	//Cylinder Highレジスタ
#define	PORT_BYTECNT_HI		(Mu4_PriSecBase | 0x175)	//Byte Count MSBレジスタ(PACKETコマンド時)
#define	PORT_DEVHEAD		(Mu4_PriSecBase | 0x176)	//Device/Headレジスタ
#define	PORT_STATUS			(Mu4_PriSecBase | 0x177)	//Statusレジスタ
#define	PORT_COMMAND		(Mu4_PriSecBase | 0x177)	//Commandレジスタ
#define	PORT_ALTSTATUS		(Mu4_CtrlRegsBase)			//Alternate Statusレジスタ
#define	PORT_DEVCTRL		(Mu4_CtrlRegsBase)			//Device Controlレジスタ



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ATAクラス
*******************************************************************************/
class	ATA : public Driver
{
/*------------------------------------------------------------------------------
	構造体
------------------------------------------------------------------------------*/
private:
	//デバイス情報構造体
	struct		DeviceInfo
	{
	public:
		u4		u4_Flag;			//フラグ群(CHS/LBA デバイスの状態)
		u4		u4_DevSecSize;		//メディアのセクタサイズ(最大LBA)
		u4		u4_DeviceType;		//接続されているデバイスの種類
		s4		s4_TransferMode;	//転送モード
		u1		u1_DeviceID;		//デバイスのID
		u1		u1_Padding[3];		//パディング
	};

	//ATAコマンド発行用構造体
	struct	ATA_Cmd
	{
	public:
		u4		u4_Flag;			//フラグ群（DRDYチェックフラグ）
		u1		u1_Feature;			//フィ−チャーレジスタ
		u1		u1_SecCnt;			//セクタカウントレジスタ
		u1		u1_SecNo;			//セクタナンバレジスタ
		u1		u1_CylLo;			//シリンダ下位レジスタ
		u1		u1_CylHi;			//シリンダ上位レジスタ
		u1		u1_DevHed;			//デバイス/ヘッドレジスタ
		u1		u1_Command;			//コマンドレジスタ
	};

	//ATAPIパケットコマンド発行用構造体
	struct	ATAPI_Cmd : public ATA_Cmd
	{
	public:
		u1		u1_CmdPacket[12];	//コマンドパケット
	};

/*------------------------------------------------------------------------------
	定数
------------------------------------------------------------------------------*/
public:
	//エラー情報
	enum	Error
	{
		ERROR		= _ERROR_,	//0x80000001
		ERROR_TIMEOUT,			//タイムアウトエラー
		ERROR_DEVSELECT,		//デバイス選択エラー
		ERROR_PARA,				//引数エラー
		ERROR_CMD,				//コマンド実行エラー
		ERROR_PACKETCMD,		//パケットコマンド実行エラー
		ERROR_MEDIA,			//メディア状態を取得できない
		ERROR_NOATAPI,			//デバイスがATAPIではない
		ERROR_NODEVICE,			//デバイス未接続
		ERROR_ABORT,			//アボートエラー
		ERROR_NOTABORT,			//アボート以外のエラー
		IDP_ERR	,				//INITALIZE DEVICE PARAMATERSコマンドエラー
		IDLECMD_ERR,			//IDLEコマンドエラー
		UDMAMSET_ERR,			//ltraDMA転送設定エラー
		NON_CTRLID_ERR,			//制御IDが存在しない
		NONID_ERR,				//登録IDなし
		NONCOMPLIANT_ERR,		//非対応
		ERROR_INVALIDCTRLID,	//無効な制御ID

		//エラーレジスタマスク
		ERRORREGI_MASK = 0x80000100,

		SUCCESS = _SUCCESS_,		//0x00000001
	};

	enum	PriSec
	{
		PRIMARY		= 0,
		SECONDARY	= 8
	};

private:
	//デバイス情報構造体のフラグ群
	enum	DeviceInfoFlag
	{
		//アドレッシング方式フラグ
		FLAG_ADRING_MASK	= 0x00000001,
		FLAG_ADRING_CHS		= 0x00000000,	//CHS方式
		FLAG_ADRING_LBA		= 0x00000001,	//LBSA方式

		//デバイスレディフラグ
		FLAG_READY_MASK		= 0x00000002,
		FLAG_READY			= 0x00000000,	//レディ状態
		FLAG_NOTREADY		= 0x00000002,	//ノットレディ状態

		//割り込み処理フラグ
		FLAG_INT_MASK		= 0x00000004,
		FLAG_INT_ENABLE		= 0x00000000,	//割り込み処理有効
		FLAG_INT_DISABLE	= 0x00000004	//割り込み処理無効
	};

	//レジスタビット
	enum	StatusRegBit
	{
		//ステータスレジスタ
		STATUSBIT_BSY		= 0x80,	// Status reg. bit7
		STATUSBIT_DRDY		= 0x40,	// Status reg. bit6
		STATUSBIT_DRQ		= 8,	// Status reg. bit3
		STATUSBIT_CHK		= 1,	// Status reg. bit0 (ATAPI)
		STATUSBIT_ERR		= 1,	// Status reg. bit0 (ATA)

		//エラーレジスタ
		ERRORBIT_ABRT		= 4,	// Error reg.  bit2

		//インタラプトレジスタ
		INTERRUPBIT_IO		= 2,	// Interrup Reason reg. bit1
		INTERRUPBIT_CD		= 1		// Interrup Reason reg. bit0
	};

	//デバイスタイプ
	enum	DeviceType
	{
		//デバイスナンバー
		DEV_MASK			= 0x000000ff,
		DEV_DIRECT_ACCESS	= 0x00000000,	//ダイレクトアクセスデバイス
		DEV_SEQUENTIAL_ACCESS,				//シーケンシャルデバイス
		DEV_PRINTER,						//プリンタデバイス
		DEV_PROCESSOR,						//プロセッサデバイス
		DEV_WRITE_ONCE,						//ライトワンスデバイス
		DEV_CD_ROM,							//CD-ROMデバイス
		DEV_SCANNER,						//スキャナー
		DEV_OPTICAL_MEMORY,					//光記憶デバイス
		DEV_MEDIA_CHANGER,					//メディアチェンジデバイス
		DEV_CONNECTION,						//通信デバイス
		DEV_RESERVED_0A,					//予約
		DEV_RESERVED_0B,					//予約
		DEV_ARRAY_CONTROLLER ,				//アレイコントロールデバイス
		DEV_ENCLOSURE_SERVICE,				//エンクロージャサービスデバイス
		DEV_REDUCED_BLOCK_CMD,				//リデュースドブロックコマンドデバイス
		DEV_OPTICAL_CARD_RW,				//光カード リード/ライトデバイス
	//0x10~0x1E 予約
		DEV_OTHER			= 0x0000001f,	//その他のデバイス

		//メジャーバージョンマスク
		VER_MASK			= 0x0000ff00,

		//接続状態
		DEVICE_MASK			= 0x00070000,
		DEVICE_NON			= 0x00000000,	//未接続
		DEVICE_ATA			= 0x00010000,	//ATA
		DEVICE_UNKOWN		= 0x00020000,	//不明なデバイス
		DEVICE_ATAPI		= 0x00040000,	//ATAPI

		//サポートフラグ
		SUP_REMOVABLE		= 0x01000000,	//リムーバブルフラグ
		SUP_AUTOEJECTt		= 0x02000000,	//オートイジェクト機能あり
		SUP_LOCK_UNLOCK		= 0x04000000,	//ロック/アンロック機能あり
		SUP_GMS_CMD			= 0x08000000	//GET MEDIA STATUSコマンドサポート
	};

	//デバイスナンバー
	enum	DevNo
	{
		MASTER,
		SLAVE,
		ATA_DEV_MAX
	};

	//転送モード
	enum	TransferMode
	{
		UNKOWN_MODE = 0x80000001,
		NON_MODE,

		PIO_MODE_MSK	= 0x0000000f,
		PIO_MODE_0		= 0x00000000,
		PIO_MODE_1		= 0x00000001,
		PIO_MODE_2		= 0x00000002,
		PIO_MODE_3		= 0x00000003,
		PIO_MODE_4		= 0x00000004,

		UDMA_MODE_MSK	= 0x000004f0,
		UDMA_MODE_0 	= 0x00000400,
		UDMA_MODE_1		= 0x00000410,
		UDMA_MODE_2		= 0x00000420,
		UDMA_MODE_3		= 0x00000430,
		UDMA_MODE_4		= 0x00000440,
		UDMA_MODE_5		= 0x00000450,
	};

	enum	INT_FLAG
	{
		//割り込みフラグ
		INTRPT_OFF		= 0x00000000,
		INTRPT_ON		= 0x00000001,
		SLEEP			= 0x00008000,
		WAKEUP			= 0x00004000,
	};

	enum	MEDIA_STATE
	{
		//メディア状態定義
		MEDIA_RADDY		= 0,	//アクセスレディ状態
		MEDIA_WP		= 0x01,	//メディアがライトプロテクト状態である

		MEDIA_NO_DISK	= 0x02,	//メディアがない
		MEDIA_CHG		= 0x04,	//メディアがチェンジされた
		MEDIA_CHG_REQ	= 0x08,	//メディアチェンジが要求された

		MEDIA_NOT_READY	= 0x40,	//ノットレディ状態
	};

	//色々定数定義
	enum
	{
		//タイムアウト
		TIMEOUT		= 0x100000,
		RETRY_MAX	= 8,
	};


/*------------------------------------------------------------------------------
	変数
------------------------------------------------------------------------------*/
private:
	DeviceInfo	M_DevInfo[ATA_DEV_MAX];	//デバイス情報構造体
	u4		Mu4_CtrlRegsBase;		//コントロールブロックレジスタベースアドレス
	u4		Mu4_PriSecBase;			//コマンドブロックレジスタベースアドレス
	u4		Mu1_IntFlag;			//割り込みフラグ
	u1		Mu1_BMOffset;			//バスマスタIDEレジスタオフセット
	u1		Mu1_Padding[2];			//パディング


/*------------------------------------------------------------------------------
	メソッド
------------------------------------------------------------------------------*/
public:
	ATA( void );						//コンストラクタ
	void	Init( void ){};				//デバイス初期化
	void	Init( PriSec PSflg );		//デバイス初期化(オーバーロード)
	s4		Ctrl( DCD* P_DCD );			//デバイスコントロール

	//Ctrlで呼ばれる関数
	s4		GetDevState( DCD_Data* P_DCData );	//デバイスの状態を取得する関数
	s4		Read( u1 u1_DeviceID, ui ui_LBA, void* Pv_Buffer, ui* Pui_NumSector );		//リード関数

private:
	//Init内処理関数
	s4		Init_DeviceCheck( u4 u4_Device ,u2* Pu2_IDdata );
	s4		Init_DeviceCheck_Sub( u4 u4_Device, u4 u4_Interim, u2* Pu2_IDdata );
	s4		Init_DeviceMode( u4 u4_Device, const u2* Pu2_IDdata );
	s4		Init_DeviceMode_Sub( u4 u4_Device, const u2* Pu2_IDdata );
	s4		Init_DeviceIdle( u4 u4_Device ,const u2* Pu2_IDdata );
	s4		MediaStatusCHK( u4 u4_Device );
	s4		ATAPI_DeviceReady( u4 u4_Device );
	s4		GetMediaInfo( u4 u4_Device, u4* Pu4_LBA, u4* Pu4_Size );

	//リード関数
	s4		PIO_Read( u4 u4_Device, ui ui_LBA, void* Pv_Buffer, ui* Pui_NumSector );		//PIOリード関数
	s4		IDComparNo( u1 u1_DeviceID );		//デバイスIDとデバイスナンバの比較

	//コマンド発行レイヤ
	s4		IdentifyDevice( u4 u4_Device, s4 s4_Flg, u2 *Pu2_IDdata );		//IDENTIFY DEVICE/PACKET DEVICEコマンド発行
	s4		IdleDevice( u4 u4_Device ,u4 DevFlg );						//IDLE IMMEDIATEコマンド
	s4		SetFeatures( u4 u4_Device, u1 u1_SubCom, u1 u1_Mode );			//SET FEATURESコマンド
	s4		GetMediaStatus( u4 u4_Device );									//GET MEDIA STATUSコマンド
	s4		ChkPowerMode( u4 u4_Device );									//CHECK POWER MODEコマンド
	s4		DeviceReset( u4 u4_Device );									//DEVICE RESETコマンド
	s4		FlushChack( u4 u4_Device );										//FLUSH CACHEコマンド
	s4		Sleep( u4 u4_Device );											//SLEEPコマンド
	s4		ExeDevDiagnostic( void );										//EXECUTE DEVICE DIAGNOSTICコマンド
	s4		StandbyImmediate( u4 u4_Device );								//STANDBY IMMEDIATEコマンド
	s4		InitDevParamaters( u4 u4_Device, u1 u1_Head, u1 u1_MaxSector );	//INITALIZE DEVICE PARAMATERSコマンド
	s4		TestUnitReady( u4 u4_Device, u1 u1_Data );						//TEST UNIT READYパケットコマンドコマンド
	s4		PacketStartUnit( u4 u4_Device, u1 u1_Data );								//START UNITパケットコマンドコマンド
	s4		PacketRequestSense( u4 u4_Device, void* Pv_Buff );							//REQUEST SENSEパケットコマンド
	s4		PacketRead10( u4 u4_Device, ui ui_LBA, void *Pv_buff, ui ui_NumSector );	//READ(10)パケットコマンド
	s4		PackeReadCapacity(u4 u4_Device, void *PV_Buff);								//Read Capacityパケットコマンド
	s4		PacketTestUnit( u4 u4_Device );												//TEST UNITコマンドパケット発行

	//コマンドプロコトルレイヤ
	s4		DeviceSelect( u1 u1_DeviceHead );											//デバイスセレクション プロトコル
	s4		NonData_CMD( ATA_Cmd *P_AtaCmd );											//Non Data Command プロトコル
	s4		PIO_DataIn_CMD( ATA_Cmd *P_AtaCmd, u2 u2_Count, void *Pv_Buff );			//PIO Data In Command プロトコル
	s4		PacketPIO_CMD( ATAPI_Cmd *P_AtapiCmd, u2 u2_Limit, void const* PCv_Buff );	//PACKET PIO Data Command プロトコル

	s4		PackeCMDRecvWait(void);		//コマンドパケット受信開始待ち

	//IOアクセスレイヤ
	void	BlockDataRead( u2 u2_Len, void* Pv_Buff );	//ブロックデータ書き込み
	void	BlockDataWrite( u2 u2_Len, void* Pv_Buff );	//ブロックデータ読み込み
	void	Wait( void );								//ウェイト
	void	Wait_DevReadySET(void);						//デバイスレディセット
	s4		Wait_BsyCHK(void);							//ビジーチェックウェイト
	void	ATAReset( void );							//ソフトウェアリセット
	s4		Signature( u4 u4_Device, u1* Pu1_CylLo, u1* Pu1_CylHi );	//シグネチャ取得
	void	CommandOUT( ATA_Cmd *P_AtaCmd );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
