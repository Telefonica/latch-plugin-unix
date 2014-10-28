CC := cc
RM := rm
ROOT_DIR := ../..
LIB_DIR := $(ROOT_DIR)/lib

SRCEXT := c
BUILDEXT := o

SOURCES := $(shell find . -type f -name "*.$(SRCEXT)") $(LIB_DIR)/charset.c $(LIB_DIR)/util.c $(LIB_DIR)/drop_privs.c
OBJECTS := $(patsubst %.$(SRCEXT),%.$(BUILDEXT),$(SOURCES))
TARGET := test

CFLAGS += -g -Wall -I$(LIB_DIR) -I$(ROOT_DIR)
LDFLAGS += -lcunit

all: ${TARGET}

$(TARGET): $(OBJECTS)
	@echo " $(CC) -o $@ $(OBJECTS) $(LDFLAGS)"; $(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	@echo " $(CC) -c $(CFLAGS) -o $@ $<"; $(CC) -c $(CFLAGS) -o $@ $<
                
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -rf $(OBJECTS) $(TARGET)"; $(RM) -rf $(OBJECTS) $(TARGET)

setuid:
	@echo " chown root"; chown root test
	@echo " chmod 4755 test"; chmod 4755 test

