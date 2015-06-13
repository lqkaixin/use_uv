
TARGET=run.out

TEST_TARGET=test_run.out

BASE_SRC :=	tools/AES.cpp \
		tools/base64.cpp \
		tools/MD5.cpp \
		tools/rc4.c \
		tools/http_parser.c \
		xl/http_cdn_queryer.c \
		xl/hub_queryer.c \
		xl/shub_query.c \
		test/test_rg.cpp \
		range.cpp \
		file.cpp \
		http_conn.cpp \
		tcp_conn.cpp \

SRC = $(BASE_SRC) main.cpp

TEST_SRC = $(BASE_SRC) \
		test/test.c \
		test/test_base64.c \
		test/test_c_base.c \
		test/test_crash.c \
		test/test_free_disk.c \
		test/test_rc4.c \
		test/test_rename_file.c \
		test/test_rg.c \
		test/test_hub.c \

CXXFLAGS_ = -Itools -Iutility -Itest -I./

LIBS = -luv

OBJ_PATH=object

BIN_PATH=bin

SRC_PATH=src

DEBUG=@




CXXFLAGS = $(patsubst -I%,-I$(SRC_PATH)/%,$(CXXFLAGS_))

TMP = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(SRC))
OBJECT = $(patsubst %.c,$(OBJ_PATH)/%.o,$(TMP))

TMP1=$(patsubst %.cpp,$(OBJ_PATH)/%.d,$(SRC))
DEP=$(patsubst %.c,$(OBJ_PATH)/%.d,$(TMP1))


TEST_TMP = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(TEST_SRC))
TEST_OBJECT = $(patsubst %.c,$(OBJ_PATH)/%.o,$(TEST_TMP))

TEST_TMP1=$(patsubst %.cpp,$(OBJ_PATH)/%.d,$(TEST_SRC))
TEST_DEP=$(patsubst %.c,$(OBJ_PATH)/%.d,$(TEST_TMP1))


all:$(BIN_PATH)/$(TARGET)


$(BIN_PATH)/$(TARGET):$(OBJECT)
	@echo "[armeabi] SharedLibrary  : $@"
	@echo "[armeabi] Install        : $@ => $(BIN_PATH)/$(TARGET)"
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(DEBUG)$(CXX) -o $@ $^ $(LIBS)


test:$(BIN_PATH)/$(TEST_TARGET)


$(BIN_PATH)/$(TEST_TARGET):$(TEST_OBJECT)
	@echo "[armeabi] SharedLibrary  : $@"
	@echo "[armeabi] Install        : $@ => $(BIN_PATH)/$(TEST_TARGET)"
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(DEBUG)$(CXX) -o $@ $^ $(LIBS)




$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	@echo "[armeabi] Compile thumb  : $(TARGET) <= $@"
	$(DEBUG)$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJ_PATH)/%.o:$(SRC_PATH)/%.c
	@echo "[armeabi] Compile thumb  : $(TARGET) <= $@"
	$(DEBUG)$(CXX) -c $(CXXFLAGS) -o $@ $<


clean:
	@echo "[armeabi] Clean          : $(TARGET) [armeabi]"
	-$(DEBUG)rm -f $(BIN_PATH)/$(TARGET) $(BIN_PATH)/$(TEST_TARGET) $(OBJECT) $(DEP) $(TEST_OBJECT) $(TEST_DEP)

sinclude $(DEP)
sinclude $(TEST_DEP)

$(OBJ_PATH)/%.d: $(SRC_PATH)/%.c
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,`dirname $@`/\1.o $@: ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

$(OBJ_PATH)/%.d: $(SRC_PATH)/%.cpp
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,`dirname $@`/\1.o $@: ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$