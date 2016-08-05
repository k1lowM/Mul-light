/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/APM/Appli.h
	概要		：	アプリケーションクラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Task.h"
#include	"LDT.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	アプリケーションヘッダ構造体
*******************************************************************************/
struct	AppliHeader
{
public:
	u4		u4_AppliSize;			//アプリケーションのサイズ(Byte)
	char	Ac_Signature[8];		//シグネチャ("MLAppli\0")
	u4		u4_StackSize;			//スタックサイズ(Page, DefSize:2Page:8KiB, Max:256Page:1MiB, Min:1Page:4KiB)
	u4		u4_HeapSize;			//ヒープサイズ(Page, DefSize:0Page:0Byte, Max:0x2000Page:32MiB, Min:0Page:0Byte)
	u1		u1_Reserved[0x0c];		//予約
};


/*******************************************************************************
	アプリケーションクラス
*******************************************************************************/
class	Appli : public Task
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//NULLポインタ
		ERROR_INVALID_FP,			//無効ファイルパス
		ERROR_FPTOOBELONG,			//ファイルパス超過
		ERROR_LDTERROR,				//LDTエラー

		SUCCESS = _SUCCESS_,
	};

private:

//変数
private:
	char	MAc_FilePath[256];		//アプリケーションのファイルパス(絶対パス)
	LDT		M_LDT;					//LDT

//メソッド
public:
	s4		Init( u4 u4_EIP, u4 u4_ESP, u4 u4_ESP0, const char* CPc_AppliFP, SegmInfo* P_SegmInfoTable );		//初期化
	void	Finalizer( void );

	s4		SetupLDT( SegmInfo* P_SegmInfoTable );		//LDTのセットアップ

	void*	GetLocalBase( void );		//ローカルアドレス取得
	s4		GetFilePath( char* Pc_FPDest );		//ファイルパス取得
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
