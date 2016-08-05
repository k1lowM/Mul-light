/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Define.h
	概要		：	Mul light APIの定数定義ヘッダです。
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	型定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
typedef	signed int				si;
typedef	signed char				s1;
typedef	signed short int		s2;
typedef	signed long int			s4;
typedef	signed long long int	s8;

typedef	unsigned int			ui;
typedef	unsigned char			u1;
typedef	unsigned short int		u2;
typedef	unsigned long int		u4;
typedef	unsigned long long int	u8;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	FALSE0	( 0 )
#define	TRUE1	( 1 )
#define	NULL	( 0 )

#define	PAGE	( 0x1000 )



/*******************************************************************************
	エラー情報
*******************************************************************************/
enum
{
	_ERROR_		= (s4)0x80000001,
	_SUCCESS_	= (s4)0x00000001,
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	構造体・共用体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//カラー構造体
union	Color4
{
	u4		u4_Color;
	struct
	{
		u1		u1_Blue;
		u1		u1_Green;
		u1		u1_Red;
		u1		u1_Alpha;
	};
};
//カラー構造体
struct	Color3
{
	u1		u1_Blue;
	u1		u1_Green;
	u1		u1_Red;
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
