#
#
# == defines ==============================================================

MAKECMDGOALS ?= make

include ../../Makefile.env

MAKEFILES_DEP = $(shell find . -type f -name Makefile.dep | sed 's/^.\///g')

MAKE_ORDER_DEPS = $(patsubst %/Makefile.dep,%.dep,$(MAKEFILES_DEP))
ifeq "0" "$(PARALLEL)"
.NOTPARALLEL: $(MAKE_ORDER_DEPS)
endif

#.PHONY: $(MAKECMDGOALS)

#
#
# ==  GOALS  ==============================================================

$(MAKECMDGOALS): $(MAKE_ORDER_DEPS)

# must be declared under main goal
include $(MAKEFILES_DEP)

%.dep:
ifeq "0" "$(PARALLEL)"
	@$(ECHO) "\n\033[1m$(MAKECMDGOALS): $(patsubst %.dep,%,$@)\033[0m"
endif
	$(MAKE) -C $(patsubst %.dep,%,$@) $(MAKECMDGOALS)
