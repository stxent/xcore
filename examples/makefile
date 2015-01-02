#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

EXAMPLES_DIR = examples
EXAMPLES := $(shell find $(EXAMPLES_DIR) -mindepth 1 -maxdepth 1 -type d -printf "%f\n")
EXAMPLE_FILES = $(addprefix $(OUTPUTDIR)/bin/,$(EXAMPLES))
TARGETS += $(EXAMPLE_FILES)

define make-example
  $(1)_CSOURCES := $(shell find $(EXAMPLES_DIR)/$(1) -name *.c)
  CSOURCES += $$($(1)_CSOURCES)
  $(1)_CXXSOURCES := $(shell find $(EXAMPLES_DIR)/$(1) -name *.cpp)
  CXXSOURCES += $$($(1)_CXXSOURCES)
  $(1)_SOURCES := $$($(1)_CSOURCES) $$($(1)_CXXSOURCES)
  $(1)_OBJECTS := $$($(1)_SOURCES:%.c=$(OUTPUTDIR)/%.o)
endef

$(foreach entry,$(EXAMPLES),$(eval $(call make-example,$(entry))))

.SECONDEXPANSION:
$(EXAMPLE_FILES): $(LIBRARY_FILE) $$($$(notdir $$@)_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(addprefix test_,$(EXAMPLES)):
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 $(@:test_%=$(OUTPUTDIR)/bin/%)

test: $(addprefix test_,$(EXAMPLES))
