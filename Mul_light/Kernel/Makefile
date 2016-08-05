#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル名	：	/Mul_light/Kernel/Makefile
#	概要		：	カーネルの生成規則です。
#	詳細		：
#	責任者		：	佐合 秀昭
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	変数定義
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
ROOT				= ../../

NAME				= Kernel

#ソースファイル
ENTRYPOINT			= EntryPoint.cpp

SOURCE				+= Main.cpp
SOURCE				+= MM.cpp GMA.cpp KMA.cpp
SOURCE				+= DT.cpp GDT.cpp IDT.cpp LDT.cpp
SOURCE				+= IM.cpp IntEntry.S Int.cpp Exception.cpp
SOURCE				+= TM.cpp TSS.cpp Task.cpp
SOURCE				+= SCM.cpp SCEntry.S
SOURCE				+= RDTM.cpp
SOURCE				+= APM.cpp AMSI.cpp Appli.cpp
SOURCE				+= GMQ.cpp
SOURCE				+= WM.cpp
SOURCE				+= WMSM_Main.cpp WMSM_MouseMove.cpp WMSM_MouseLPress.cpp WMSM_MouseLRelease.cpp
SOURCE				+= WMDO_Main.cpp WMDO_Window.cpp WMDO_Button.cpp WMDO_Text.cpp
SOURCE				+= GUI.cpp GUI_Debug.cpp Desktop.cpp DebugFont.cpp

SOURCE				+= $(DR_SOURCE)
SOURCE				+= $(FS_SOURCE)

#ライブラリソース
LIB_SOURCE			+= KStdLib.cpp
LIB_SOURCE			+= IO.cpp
LIB_SOURCE			+= Image.cpp Bmp.cpp

#ディレクトリ情報
DIRECTORY			+= APM
DIRECTORY			+= DM
DIRECTORY			+= DTM
DIRECTORY			+= FS
DIRECTORY			+= GUI
DIRECTORY			+= IM
DIRECTORY			+= MM
DIRECTORY			+= RDTM
DIRECTORY			+= SCM
DIRECTORY			+= TM
DIRECTORY			+= WM WM/DrawObject WM/SendMessage

DIRECTORY			+= KLib
DIRECTORY			+= KLib/Image

DIRECTORY			+= $(addprefix DM/, $(DR_DIRECTORY))
DIRECTORY			+= $(addprefix FS/, $(FS_DIRECTORY))


#リンクオプション
#-nostdlib	: 標準のシステムライブラリとスタートアップファイルを使用しない。
#-static	: 動的リンクをしない。
#-L			: ライブラリを探索するディレクトリのリストに追加する。
#-l			: リンクするファイルリストに加える。lib〜.aではさまれたファイル名になる。
#-Map		: リンクマップを出力する。
LINK_OPTIONS		+= -T $(LS_NAME)
LINK_OPTIONS		+= -nostdlib
LINK_OPTIONS		+= -static
LINK_OPTIONS		+= -L$(INC_DIR_ML)
LINK_OPTIONS		+= -L$(INC_DIR_CSTD)
LINK_OPTIONS		+= -l$(ML_LIB_NAME)
LINK_OPTIONS		+= -l$(CSTD_LIB_NAME)
LINK_OPTIONS		+= -Map $(MAP_NAME)



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	インクルード
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
include	DM/Makefile
include	FS/Makefile
include	$(ROOT)Common.mk
-include $(DEPENDS)



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	コマンド
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#*******************************************************************************
#	デフォルトコマンド
#*******************************************************************************
PHONY += default _default
default:
	@make -s _$@
_default: _binary



.PHONY: $(PHONY)
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	End of file
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#

