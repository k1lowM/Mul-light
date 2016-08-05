/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/ISO9660/ISO9660.h
	概要		：	ISO9660(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"FS.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ISO9660クラス
	レベル2まで対応
*******************************************************************************/
class	ISO9660 : public FS
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,		//ヌルポ
		ERROR_NOTSUPPORT,		//サポート対象外
		ERROR_READ_FAILED,		//読み取り失敗
		ERROR_PT_SIZE_0,		//パステーブルサイズが0
		ERROR_FILENOTFOUND,		//ファイルが見つからない
		ERROR_MEMORYALLOC,		//メモリ領域確保エラー
		ERROR_UNINITIALIZED,	//未初期化

		SUCCESS = _SUCCESS_,
	};

//クラス・構造体定義
private:
	//パスデータ構造体
	struct	PathData
	{
		u4		u4_LBA;			//ディレクトリ情報のLBA
		u2		u2_FirstChiled;	//子ディレクトリの先頭番号
		u2		u2_NumChiled;	//子ディレクトリ数
		char	Ac_DirName[32];	//ディレクトリ名(MAX:31文字+'\0')
	};

protected:
	//その他定義
	enum
	{
		SECTOR_SIZE	= 0x800,	//1ブロックのサイズ

		LBA_PVD		= 0x10,		//PVDの論理ブロックアドレス(0x8000 / 2048)
	};

//変数
protected:
	ui		Mui_PDT_KMID;		//パスデータテーブルのメモリID

//メソッド
public:
	s4		Init( u1 u1_DeviceID, ui ui_FSMemID );

	s4		CheckFS( u1 u1_DeviceID );		//ファイルシステム確認

	s4		Rename( const char* CPc_FilePath, const char* CPc_NewFileName );				//ファイル名変更
	s4		Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//読み取り
	s4		Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//書き込み

private:
	s4		Read( u1 u1_DeviceID, ui ui_LBA, ui ui_Offset, void* Pv_Addr, ui ui_Size );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
