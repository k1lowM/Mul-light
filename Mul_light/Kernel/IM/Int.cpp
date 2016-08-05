/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/IM/Int.cpp
	概要		：	割り込みルーチン
	詳細		：	割り込みルーチンのダミーです。
					割り込みルーチンを使う場合は、使用する番号の割り込みルーチンを
					コメントアウトして、実体をそれぞれのソースに記述すること。
					コメントアウトしたら、何に使うのか行コメントを記述しておくこと。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"IM.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	INT_FUNC( _NUMBER_ )\
	void	Int##_NUMBER_( ARG_CONST_REGI_STAT )\
	{\
		while( true )\
			;\
	}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//0*	例外
//INT_FUNC( 00 )		//例外
//INT_FUNC( 01 )		//例外
//INT_FUNC( 02 )		//例外
//INT_FUNC( 03 )		//例外
//INT_FUNC( 04 )		//例外
//INT_FUNC( 05 )		//例外
//INT_FUNC( 06 )		//例外
//INT_FUNC( 07 )		//例外
//INT_FUNC( 08 )		//例外
//INT_FUNC( 09 )		//例外
//INT_FUNC( 0A )		//例外
//INT_FUNC( 0B )		//例外
//INT_FUNC( 0C )		//例外
//INT_FUNC( 0D )		//例外
//INT_FUNC( 0E )		//例外
//INT_FUNC( 0F )		//例外

//1*	例外
//INT_FUNC( 10 )		//例外
//INT_FUNC( 11 )		//例外
//INT_FUNC( 12 )		//例外
//INT_FUNC( 13 )		//例外
//INT_FUNC( 14 )		//例外
//INT_FUNC( 15 )		//例外
//INT_FUNC( 16 )		//例外
//INT_FUNC( 17 )		//例外
//INT_FUNC( 18 )		//例外
//INT_FUNC( 19 )		//例外
//INT_FUNC( 1A )		//例外
//INT_FUNC( 1B )		//例外
//INT_FUNC( 1C )		//例外
//INT_FUNC( 1D )		//例外
//INT_FUNC( 1E )		//例外
//INT_FUNC( 1F )		//例外

//2*	ハードウェア割り込み
//INT_FUNC( 20 )	//PIT
//INT_FUNC( 21 )	//キーボード
INT_FUNC( 22 )
INT_FUNC( 23 )
INT_FUNC( 24 )
INT_FUNC( 25 )
INT_FUNC( 26 )
INT_FUNC( 27 )
INT_FUNC( 28 )
INT_FUNC( 29 )
INT_FUNC( 2A )
INT_FUNC( 2B )
//INT_FUNC( 2C )	//マウス割り込み
INT_FUNC( 2D )
//INT_FUNC( 2E )	//ATA(プライマリ)
//INT_FUNC( 2F )	//ATA(セカンダリ)

//3*
INT_FUNC( 30 )
INT_FUNC( 31 )
INT_FUNC( 32 )
INT_FUNC( 33 )
INT_FUNC( 34 )
INT_FUNC( 35 )
INT_FUNC( 36 )
INT_FUNC( 37 )
INT_FUNC( 38 )
INT_FUNC( 39 )
INT_FUNC( 3A )
INT_FUNC( 3B )
INT_FUNC( 3C )
INT_FUNC( 3D )
INT_FUNC( 3E )
INT_FUNC( 3F )

//4*
INT_FUNC( 40 )
INT_FUNC( 41 )
INT_FUNC( 42 )
INT_FUNC( 43 )
INT_FUNC( 44 )
INT_FUNC( 45 )
INT_FUNC( 46 )
INT_FUNC( 47 )
INT_FUNC( 48 )
INT_FUNC( 49 )
INT_FUNC( 4A )
INT_FUNC( 4B )
INT_FUNC( 4C )
INT_FUNC( 4D )
INT_FUNC( 4E )
INT_FUNC( 4F )

//5*
INT_FUNC( 50 )
INT_FUNC( 51 )
INT_FUNC( 52 )
INT_FUNC( 53 )
INT_FUNC( 54 )
INT_FUNC( 55 )
INT_FUNC( 56 )
INT_FUNC( 57 )
INT_FUNC( 58 )
INT_FUNC( 59 )
INT_FUNC( 5A )
INT_FUNC( 5B )
INT_FUNC( 5C )
INT_FUNC( 5D )
INT_FUNC( 5E )
INT_FUNC( 5F )

//6*
INT_FUNC( 60 )
INT_FUNC( 61 )
INT_FUNC( 62 )
INT_FUNC( 63 )
INT_FUNC( 64 )
INT_FUNC( 65 )
INT_FUNC( 66 )
INT_FUNC( 67 )
INT_FUNC( 68 )
INT_FUNC( 69 )
INT_FUNC( 6A )
INT_FUNC( 6B )
INT_FUNC( 6C )
INT_FUNC( 6D )
INT_FUNC( 6E )
INT_FUNC( 6F )

//7*
INT_FUNC( 70 )
INT_FUNC( 71 )
INT_FUNC( 72 )
INT_FUNC( 73 )
INT_FUNC( 74 )
INT_FUNC( 75 )
INT_FUNC( 76 )
INT_FUNC( 77 )
INT_FUNC( 78 )
INT_FUNC( 79 )
INT_FUNC( 7A )
INT_FUNC( 7B )
INT_FUNC( 7C )
INT_FUNC( 7D )
INT_FUNC( 7E )
INT_FUNC( 7F )

//8*
INT_FUNC( 80 )
INT_FUNC( 81 )
INT_FUNC( 82 )
INT_FUNC( 83 )
INT_FUNC( 84 )
INT_FUNC( 85 )
INT_FUNC( 86 )
INT_FUNC( 87 )
INT_FUNC( 88 )
INT_FUNC( 89 )
INT_FUNC( 8A )
INT_FUNC( 8B )
INT_FUNC( 8C )
INT_FUNC( 8D )
INT_FUNC( 8E )
INT_FUNC( 8F )

//9*
INT_FUNC( 90 )
INT_FUNC( 91 )
INT_FUNC( 92 )
INT_FUNC( 93 )
INT_FUNC( 94 )
INT_FUNC( 95 )
INT_FUNC( 96 )
INT_FUNC( 97 )
INT_FUNC( 98 )
INT_FUNC( 99 )
INT_FUNC( 9A )
INT_FUNC( 9B )
INT_FUNC( 9C )
INT_FUNC( 9D )
INT_FUNC( 9E )
INT_FUNC( 9F )

//A*
INT_FUNC( A0 )
INT_FUNC( A1 )
INT_FUNC( A2 )
INT_FUNC( A3 )
INT_FUNC( A4 )
INT_FUNC( A5 )
INT_FUNC( A6 )
INT_FUNC( A7 )
INT_FUNC( A8 )
INT_FUNC( A9 )
INT_FUNC( AA )
INT_FUNC( AB )
INT_FUNC( AC )
INT_FUNC( AD )
INT_FUNC( AE )
INT_FUNC( AF )

//B*
INT_FUNC( B0 )
INT_FUNC( B1 )
INT_FUNC( B2 )
INT_FUNC( B3 )
INT_FUNC( B4 )
INT_FUNC( B5 )
INT_FUNC( B6 )
INT_FUNC( B7 )
INT_FUNC( B8 )
INT_FUNC( B9 )
INT_FUNC( BA )
INT_FUNC( BB )
INT_FUNC( BC )
INT_FUNC( BD )
INT_FUNC( BE )
INT_FUNC( BF )

//C*
INT_FUNC( C0 )
INT_FUNC( C1 )
INT_FUNC( C2 )
INT_FUNC( C3 )
INT_FUNC( C4 )
INT_FUNC( C5 )
INT_FUNC( C6 )
INT_FUNC( C7 )
INT_FUNC( C8 )
INT_FUNC( C9 )
INT_FUNC( CA )
INT_FUNC( CB )
INT_FUNC( CC )
INT_FUNC( CD )
INT_FUNC( CE )
INT_FUNC( CF )

//D*
INT_FUNC( D0 )
INT_FUNC( D1 )
INT_FUNC( D2 )
INT_FUNC( D3 )
INT_FUNC( D4 )
INT_FUNC( D5 )
INT_FUNC( D6 )
INT_FUNC( D7 )
INT_FUNC( D8 )
INT_FUNC( D9 )
INT_FUNC( DA )
INT_FUNC( DB )
INT_FUNC( DC )
INT_FUNC( DD )
INT_FUNC( DE )
INT_FUNC( DF )

//E*
INT_FUNC( E0 )
INT_FUNC( E1 )
INT_FUNC( E2 )
INT_FUNC( E3 )
INT_FUNC( E4 )
INT_FUNC( E5 )
INT_FUNC( E6 )
INT_FUNC( E7 )
INT_FUNC( E8 )
INT_FUNC( E9 )
INT_FUNC( EA )
INT_FUNC( EB )
INT_FUNC( EC )
INT_FUNC( ED )
INT_FUNC( EE )
INT_FUNC( EF )

//F*
INT_FUNC( F0 )
INT_FUNC( F1 )
INT_FUNC( F2 )
INT_FUNC( F3 )
INT_FUNC( F4 )
INT_FUNC( F5 )
INT_FUNC( F6 )
INT_FUNC( F7 )
INT_FUNC( F8 )
INT_FUNC( F9 )
INT_FUNC( FA )
INT_FUNC( FB )
INT_FUNC( FC )
INT_FUNC( FD )
INT_FUNC( FE )
INT_FUNC( FF )



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

