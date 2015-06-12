
TARGET=run.out

SRC :=	tools/AES.cpp \
		tools/base64.cpp \
		tools/md5.cpp \
		test/test_rg.cpp \
		range.cpp \
		main.cpp \
		file.cpp \
		http_conn.cpp \
		tcp_conn.cpp \
		http_cdn_queryer.c \
		http_parser.c \
		hub_queryer.c \

CXXFLAGS_ = -Itools -Iutility -Itest

LIBS = -luv

OBJ_PATH=object

BIN_PATH=bin

SRC_PATH=src

# DEBUG=@




CXXFLAGS = $(patsubst -I%,-I$(SRC_PATH)/%,$(CXXFLAGS_))

TMP = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(SRC))
OBJECT = $(patsubst %.c,$(OBJ_PATH)/%.o,$(TMP))

TMP1=$(patsubst %.cpp,$(OBJ_PATH)/%.dd,$(SRC))
DEP=$(patsubst %.c,$(OBJ_PATH)/%.d,$(TMP1))


all:$(BIN_PATH)/$(TARGET)


$(BIN_PATH)/$(TARGET):$(OBJECT)
	@echo "[armeabi] SharedLibrary  : $@"
	@echo "[armeabi] Install        : $@ => $(BIN_PATH)/$(TARGET)"
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
	-$(DEBUG)rm $(BIN_PATH)/$(TARGET) $(OBJECT) $(DEP)

sinclude $(DEP)

$(OBJ_PATH)/%.d: $(SRC_PATH)/%.c
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,`dirname $@`/\1.o $@: ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

$(OBJ_PATH)/%.dd: $(SRC_PATH)/%.cpp
	$(DEBUG)mkdir -p `dirname $@`
	$(DEBUG)$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,`dirname $@`/\1.o $@: ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$