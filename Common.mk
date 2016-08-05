#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル名	：	/Common.mk
#	概要		：	共通Makefile
#	詳細		：	すべてのMakefileが必ずインクルードするサブMakefileです。
#					各Makefileの先頭にルートのパスを指定後、インクルードしてください。
#	責任者		：	佐合 秀昭
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#


#Common.mk利用時の使い方、注意事項
#・ROOTは必ず定義してください。
#・NAMEを定義すると、以下の定義が利用できます。
#	LS_NAME		$(NAME).ls		リンカスクリプトファイル
#	MAP_NAME	$(NAME).map		マッピングファイル
#	BIN_NAME	$(NAME).bin		バイナリファイル
#	IMG_NAME	$(NAME).img		イメージファイル
#	ISO_NAME	$(NAME).iso		isoファイル
#	LIB_NAME	lib$(NAME).a	ライブラリファイル
#・ENTRYPOINT, SOURCE, LIB_SOURCEのいずれかを定義すると、以下の定義が利用できます。
#	ALL_SOURCE	全ソースファイル
#	FILES		ALL_SOURCEの拡張子を削除したもの
#	HYDEFILES	FILESの先頭に'.'をつけたもの(隠しファイル)
#	OBJECTS		HYDEFILESに拡張子'.o'を付加したもの
#	DEPENDS		HYDEFILESに拡張子'.d'を付加したもの


#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	変数定義
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#OS名
OS_NAME				= Mul_light

#ライブラリ名
CSTD_LIB_NAME		= cStdLib
CSTD_LIB			= lib$(CSTD_LIB_NAME).a
ML_LIB_NAME			= Mul_light
ML_LIB				= lib$(ML_LIB_NAME).a

#ディレクトリパス
OS_DIR				= $(ROOT)$(OS_NAME)/
INC_DIR				= $(ROOT)Include/
INC_DIR_CSTD		= $(INC_DIR)$(CSTD_LIB_NAME)
INC_DIR_ML			= $(INC_DIR)$(ML_LIB_NAME)
DATA_DIR			= $(ROOT)Data/
APPLI_DIR			= $(ROOT)Application/
TOOLS_DIR			= $(ROOT)Tools/

#オブジェクトファイル
ifneq ($(strip $(ENTRYPOINT) $(SOURCE) $(LIB_SOURCE)),)
ALL_SOURCE	= $(strip $(ENTRYPOINT) $(SOURCE) $(LIB_SOURCE))
FILES		= $(basename $(ALL_SOURCE))
HYDEFILES	= $(addprefix ., $(FILES))
OBJECTS		= $(addsuffix .o, $(HYDEFILES))
DEPENDS		= $(addsuffix .d, $(HYDEFILES))
endif

#各種ファイル
ifneq ($(strip $(NAME)),)
LS_NAME				= $(NAME).ls
MAP_NAME			= $(NAME).map
BIN_NAME			= $(NAME).bin
IMG_NAME			= $(NAME).img
ISO_NAME			= $(NAME).iso
LIB_NAME			= lib$(NAME).a
endif

#基本依存ファイル
BASIC_DEPEND_FILES	= Makefile $(ROOT)Common.mk


#ディレクトリ探索
ifneq ($(strip $(DIRECTORY)),)
vpath % $(DIRECTORY)
endif

#アセンブルオプション。
#-a=					: アセンブリリストを出力する。
#-Wall					: 可能な全ての警告オプションを有効にします。
#-c 					: コンパイラドライバにコンパイル、アセンブルを指示します。
AS_OPTIONS			+= -c
AS_OPTIONS			+= -a=_$*.lst
AS_OPTIONS			+= -Wall


#コンパイルオプション
#-nostdinc				: 標準のヘッダファイルがあるシステムディレクトリを検索しない。
#-I						: インクルードパスを指定する。
#-Wa,-a=				: アセンブリリストを出力する。
#-Wall					: 可能な全ての警告オプションを有効にします。
#-c 					: コンパイラドライバにコンパイル、アセンブルを指示します。
#-fno-common			: externをつけずに同一の変数を宣言した2つのコンパイルに対して、リンク時にエラーを発生させます。
#-fno-builtin			: ビルトイン関数のうち、__で始まるもの以外を認識しなくなります。現在、この指定は_exit, abort, abs, alloca, cos, exit, fabs, labs, memcmp, memcpy, sin, sqrt, strcmp, strcpy, strlen の関数に影響を及ぼします。
#-O2					: 最適化レベル2。詳細はサイトを参照のこと。
C_OPTIONS			+= -nostdinc
C_OPTIONS			+= -I./
C_OPTIONS			+= $(addprefix -I, $(DIRECTORY))
C_OPTIONS			+= -I$(INC_DIR_ML)
C_OPTIONS			+= -I$(INC_DIR_CSTD)
C_OPTIONS			+= -Wa,-a=_$*.lst
C_OPTIONS			+= -Wall
C_OPTIONS			+= -Wwrite-strings
C_OPTIONS			+= -c
C_OPTIONS			+= -fno-common
C_OPTIONS			+= -fno-builtin
C_OPTIONS			+= -O0


#C++コンパイルオプション。
#-nostdinc				: 標準のヘッダファイルがあるシステムディレクトリを検索しない。
#-I						: インクルードパスを指定する。
#-Wa,-a=				: アセンブリリストを出力する。
#-Wall					: 可能な全ての警告オプションを有効にします。
#-c 					: コンパイラドライバにコンパイル、アセンブルを指示します。
#-fno-common			: externをつけずに同一の変数を宣言した2つのコンパイルに対して、リンク時にエラーを発生させます。
#-fno-builtin			: ビルトイン関数のうち、__で始まるもの以外を認識しなくなります。現在、この指定は_exit, abort, abs, alloca, cos, exit, fabs, labs, memcmp, memcpy, sin, sqrt, strcmp, strcpy, strlen の関数に影響を及ぼします。
#-O2					: 最適化レベル2。詳細はサイトを参照のこと。
CPP_OPTIONS			+= -nostdinc
CPP_OPTIONS			+= -I./
CPP_OPTIONS			+= $(addprefix -I, $(DIRECTORY))
CPP_OPTIONS			+= -I$(INC_DIR_ML)
CPP_OPTIONS			+= -I$(INC_DIR_CSTD)
CPP_OPTIONS			+= -Wa,-a=_$*.lst
CPP_OPTIONS			+= -Wall
CPP_OPTIONS			+= -Wwrite-strings
CPP_OPTIONS			+= -c
CPP_OPTIONS			+= -fno-common
CPP_OPTIONS			+= -fno-builtin
CPP_OPTIONS			+= -O0


#依存関係ファイル作成オプション
#-nostdinc				: 標準のヘッダファイルがあるシステムディレクトリを検索しない。
#-I						: インクルードパスを指定する。
#-Wall					: 可能な全ての警告オプションを有効にします。
DEP_OPTIONS			+= -nostdinc
DEP_OPTIONS			+= -I./
DEP_OPTIONS			+= $(addprefix -I, $(DIRECTORY))
DEP_OPTIONS			+= -I$(INC_DIR_ML)
DEP_OPTIONS			+= -I$(INC_DIR_CSTD)
DEP_OPTIONS			+= -Wall


#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	コマンド
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#*******************************************************************************
#	デフォルト動作
#*******************************************************************************
PHONY += commondefault _commondefault
commondefault:
	@make -s _$@
_commondefault: _default


#*******************************************************************************
#	バイナリファイル作成
#*******************************************************************************
PHONY += binary bin _binary _bin
binary bin:
	@make -s _$@
_binary _bin: $(BIN_NAME)


#*******************************************************************************
#	ごみファイル削除
#	存在していてもしょうがないファイルをイメージ作成後に削除します。
#	バックアップファイル(~)とオブジェクトファイル(.o)を削除します。
#*******************************************************************************
PHONY += trashdelete td _trashdelete _td
trashdelete td:
	@make -s _$@
_trashdelete _td:
	@echo -e "\trm -f\t*~"
	rm -f *~ */*~ */*/*~ */*/*/*~ */*/*/*/*~


#*******************************************************************************
#	削除
#*******************************************************************************
PHONY += delete del _delete _del
delete del:
	@make -s _$@
_delete _del: _trashdelete
	@echo -e "\trm -f\t*.o"
	rm -f *.o */*.o */*/*.o */*/*/*.o */*/*/*/*.o
	rm -f .*.o */.*.o */*/.*.o */*/*/.*.o */*/*/*/.*.o
	@echo -e "\trm -f\t*.d"
	rm -f *.d */*.d */*/*.d */*/*/*.d */*/*/*/*.d
	rm -f .*.d */.*.d */*/.*.d */*/*/.*.d */*/*/*/.*.d
	@echo -e "\trm -f\t*.bin"
	rm -f *.bin */*.bin */*/*.bin */*/*/*.bin */*/*/*/*.bin
	@echo -e "\trm -f\t*.img"
	rm -f *.img */*.img */*/*.img */*/*/*.img */*/*/*/*.img
	@echo -e "\trm -f\t*.iso"
	rm -f *.iso */*.iso */*/*.iso */*/*/*.iso */*/*/*/*.iso


#*******************************************************************************
#	ソースファイル以外ほぼすべて削除
#*******************************************************************************
PHONY += alldelete ad _alldelete _ad
alldelete ad:
	@make -s _$@
_alldelete _ad: _delete
	@echo -e "\trm -f\t*.lst"
	rm -f *.lst */*.lst */*/*.lst */*/*/*.lst */*/*/*/*.lst
	@echo -e "\trm -f\t*.map"
	rm -f *.map */*.map */*/*.map */*/*/*.map */*/*/*/*.map
	@echo -e "\trm -fr\t$(DATA_DIR)*"
	rm -fr $(DATA_DIR)*



#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	ファイル生成規則
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#*******************************************************************************
#	依存関係ファイル作成
#*******************************************************************************
.%.d: %.S $(BASIC_DEPEND_FILES)
	@gcc -M $(DEP_OPTIONS) $< | sed -e "s/\($*\)\.o[ :]*/\.\1.o $@: /g" > $@
.%.d: %.c $(BASIC_DEPEND_FILES)
	@gcc -M $(DEP_OPTIONS) $< | sed -e "s/\($*\)\.o[ :]*/\.\1.o $@: /g" > $@
.%.d: %.cpp $(BASIC_DEPEND_FILES)
	@gcc -M $(DEP_OPTIONS) $< | sed -e "s/\($*\)\.o[ :]*/\.\1.o $@: /g" > $@


#*******************************************************************************
#	アセンブル		アセンブリ(.S) -> オブジェクト(.o)
#*******************************************************************************
.%.o: %.S $(BASIC_DEPEND_FILES)
	@echo -e "\tas\t$<"
	gcc $(AS_OPTIONS) $< -o $@


#*******************************************************************************
#	コンパイル		c(.c) -> オブジェクト(.o)
#*******************************************************************************
.%.o: %.c $(BASIC_DEPEND_FILES)
	@echo -e "\tgcc\t$<"
	gcc $(C_OPTIONS) $< -o $@


#*******************************************************************************
#	コンパイル		c++(.cpp) -> オブジェクト(.o)
#*******************************************************************************
.%.o: %.cpp $(BASIC_DEPEND_FILES)
	@echo -e "\tg++\t$<"
	g++ $(CPP_OPTIONS) $< -o $@


#*******************************************************************************
#	バイナリファイル作成
#*******************************************************************************
$(BIN_NAME): $(OBJECTS)
	@echo -e "\tld\t$@"
	ld -o $@ $^ $(LINK_OPTIONS)



.PHONY:	$(PHONY)
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
#	End of file
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■#
