CC = gcc
CFLAGS = -std=c99

AR = ar -rcu

GOS := .6
GOC := go tool 6g
GOL := go tool 6l
LUA := lua

RM := rm -rf
CD := cd
MKDIR := mkdir -p

CLIB_SRCS := base64.c
CLIB_NAME := libbase64.a
LUALIB_SRCS := base64_lua.c
LUALIB_FILE := base64.so
TESTSRCS := base64_test.c
LUATEST := base64_test.lua

CGENSRCS = genmode.c
GGENSRCS = gentest.go

all : gen lualib clib test

BUILD = build
BUILD_O = $(BUILD)/o
BUILD_TOOL = $(BUILD)/tool
BUILD_TEST = $(BUILD)/test
BUILD_D = $(BUILD_O) $(BUILD_TEST)

$(BUILD) : $(BUILD_D)

$(BUILD_D) :
	$(MKDIR) $@

define build_o
BAS := $(basename $(notdir $(1)))
OBJ := $(BUILD_O)/$$(BAS).o
$$(OBJ) : | $(BUILD_O)
$$(OBJ) : $(1)
	$(CC) $(CFLAGS) -fPIC -c -o $$@ $$^
endef

define build_cgen
TAR := $(BUILD)/$(basename $(notdir $(1)))
$$(TAR) : | $(BUILD)
$$(TAR) : $(1)
	$(CC) $(CFLAGS) -o $$@ $$^
endef

define build_test
TAR := $(BUILD_TEST)/$(basename $(notdir $(1)))
$$(TAR) : | $(BUILD_TEST)
$$(TAR) : $(1)
	$(CC) $(CFLAGS) -o $$@ $$^ -L/usr/local/lib -L$(BUILD) -lbase64
endef

define build_ggen
BAS := $(basename $(notdir $(1)))
TAR := $(BUILD)/$$(BAS)
OBJ := $(BUILD_O)/$$(BAS)$(GOS)
$$(TAR) : | $(BUILD)
$$(OBJ) : | $(BUILD_O)
$$(OBJ) : $(1)
	$(GOC) -o $$@ $$^
$$(TAR) : $$(OBJ)
	$(GOL) -o $$@ $$^
endef

$(foreach src, $(CGENSRCS), $(eval $(call build_cgen, $(src))))
$(foreach src, $(GGENSRCS), $(eval $(call build_ggen, $(src))))
$(foreach src, $(CLIB_SRCS), $(eval $(call build_o, $(src))))

del :
	$(RM) mode64.c base64_test.h
gen : del mode64.c base64_test.h
mode64.c : $(BUILD)/genmode
	./$<
base64_test.h: $(BUILD)/gentest
	./$<

CLIB_OBJS := $(addprefix $(BUILD_O)/, $(addsuffix .o, $(basename $(CLIB_SRCS))))
CLIB_FILE = $(BUILD)/$(CLIB_NAME)
clib : $(CLIB_FILE) | mode64.c
$(CLIB_FILE) : $(CLIB_OBJS)
	@$(AR) $@ $^

lualib : clib $(LUALIB_FILE)
$(LUALIB_FILE) : $(LUALIB_SRCS)
	$(CC) $(CFLAGS) -shared -fPIC -o $(LUALIB_FILE) $^ -Lbuild -lbase64

test : ctest luatest

$(foreach src, $(TESTSRCS), $(eval $(call build_test, $(src))))
TESTEXCS := $(addprefix $(BUILD_TEST)/, $(basename $(TESTSRCS)))
ctest : clib base64_test.h $(TESTEXCS)
#	$(foreach t, $(TESTEXCS), $(eval $(call shell, $(t))))
	./$(BUILD_TEST)/base64_test

luatest : lualib
	$(LUA) $(LUATEST)

clean :
	-$(RM) $(BUILD) base64.so

.PHONY : all gen clib lualib test clean
