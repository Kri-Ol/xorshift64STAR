# -*- Makefile -*-

#
# step #1 - system specific compilers, setttings etc
#

OSTYPE := $(shell uname -s)

#
#
#

ifeq ($(CONFIG),) # not defined

include build/GNUmakefile.config # default is debug

endif


#
# external libraries
#

LOCAL_INCLUDE =

#
# utilities
#

ECHO := /bin/echo

#
# defines
#

DEFS  :=

#
# compiler flags, depnLdncy generation and implicit build rules
#

-include build/GNUmakefile.compiler
-include build/GNUmakefile.ABI
-include build/GNUmakefile.depends
-include build/GNUmakefile.rules

#
# sources, objects and targets
#
# object files - done by string substitution
#

SRCS_XSS := main.cpp xorshift64star.cpp
OBJS_XSS := $(addsuffix .o, $(basename $(SRCS_XSS)))	
DEPS_XSS := $(addsuffix .d, $(basename $(SRCS_XSS)))

OBJS := $(OBJS_XSS)
DEPS := $(DEPS_XSS)

# all targets

XSS := xss

.PHONY: all tags dox clean realclean

all: $(XSS)

$(XSS): $(OBJS_XSS)
	$(LD) $(LDFLAGS) -o $@ $(wordlist 1,10000000,$^) -L/usr/local/lib $(SYSLIB)	

#dependencies
-include $(DEPS)

tags:
	@(find . -name "*.h" -print | etags -)
	@(find . -name "*.cpp" -print | etags -a -)
	@(find . -name "*.cc" -print | etags -a -)
	@/bin/echo "TAGS updated"

dox:
	doxygen

clean:
	-rm -rf $(XSS)
	-rm -rf $(OBJS)

realclean:
	-rm -rf $(XSS)
	-rm -rf $(OBJS)
	-rm -rf $(DEPS)
	-rm -rf TAGS
