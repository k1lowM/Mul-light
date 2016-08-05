/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/Image/Bmp.h
	概要		：
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Image.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	Bmpクラス
*******************************************************************************/
class	Bmp : public Image
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_READFAILED,			//ファイル読み取り失敗

		SUCCESS = _SUCCESS_,
	};

//構造体・クラス定義
public:
	struct	FileHead
	{
		u2		u2_Type;			//ファイルタイプ(OS/2, Windows Bitmap)
		u4		u4_Size;			//ファイルサイズ(byte)
		u2		u2_Reserved1;
		u2		u2_Reserved2;
		u4		u4_ImageOffset;		//ファイル先頭から画像までのオフセット
	}__attribute__((packed));

	struct	OS2BmpInfoHead
	{
		u4		u4_HeadSize;		//情報ヘッダサイズ
		s4		s4_Width;			//幅
		s4		s4_Height;			//高さ(正数：下->上、負数：上->下)
		u2		u2_Planes;			//プレーン数(1固定)
		u2		u2_BitCount;		//色数(1,4,8,16,24,32)
	};

	struct	WinBmpInfoHead : public OS2BmpInfoHead
	{
		u4		u4_Compression;		//圧縮形式
		u4		u4_SizeImage;		//
		s4		s4_XPixPerMeter;	//
		s4		s4_YPixPerMeter;	//
		u4		s4_ClrUsed;			//
		u4		u4_ClrImporant;		//
	}__attribute__((packed));

//メソッド
public:
	static s4	Read( const char* CPc_ImageFP, void* Pv_Dest, const ui Cui_BufSize, FileHead* P_FileHead = NULL, WinBmpInfoHead* P_InfoHead = NULL );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

