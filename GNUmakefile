# To list the possible targets to compile, use
#   make help
# Compile with debugging information with 
#   make BUILD_TYPE=Debug

DIRS:=$(filter-out build%,$(patsubst %/,%,$(dir $(wildcard */))))
CLEANDIRS:=$(addprefix clean-,$(DIRS))

CMAKE_FLAGS:=
ifdef BUILD_TYPE
	CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
endif

ifndef SYSTEMINSTALL
  $(error Variable SYSTEMINSTALL is not defined!)
endif

BUILDDIR=build-$(SYSTEMINSTALL)

all: $(BUILDDIR)
	+@make -C $(BUILDDIR) install --no-print-directory $(MAKEFLAGS)

$(BUILDDIR): 
	@cmake -H. -B$(BUILDDIR) $(CMAKE_FLAGS) ${ARGS}

clean:
	rm -rf $(BUILDDIR)
	rm -rf lib-$(SYSTEMINSTALL)
	rm -rf bin-$(SYSTEMINSTALL)

.DEFAULT: $(BUILDDIR)
	+@make -C $(BUILDDIR) $@ --no-print-directory $(MAKEFLAGS)

$(DIRS): $(BUILDDIR)
	+@make -C $(BUILDDIR) $@ --no-print-directory $(MAKEFLAGS)

$(CLEANDIRS): $(BUILDDIR)
	rm -rf $(BUILDDIR)/$(patsubst clean-%,%,$@)
		
.PHONY: build_all clean $(DIRS) $(CLEANDIRS)   
