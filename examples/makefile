#Copyright (C) 2015 xent
#Project is distributed under the terms of the GNU General Public License v3.0

ifeq ($(CONFIG_EXAMPLES),y)
  PROJECT_FLAGS += CONFIG_DEBUG

  EXAMPLES_DIR = examples
  EXAMPLES := $(shell find $(EXAMPLES_DIR) -mindepth 1 -maxdepth 1 -type d -printf "%f\n")
  EXAMPLE_FILES = $(addprefix $(OUTPUT_DIR)/bin/,$(EXAMPLES))
  TARGETS += $(EXAMPLE_FILES)
endif

define make-example
  $(1)_CSOURCES := $(wildcard $(EXAMPLES_DIR)/$(1)/*.c)
  CSOURCES += $$($(1)_CSOURCES)
  $(1)_CXXSOURCES := $(wildcard $(EXAMPLES_DIR)/$(1)/*.cpp)
  CXXSOURCES += $$($(1)_CXXSOURCES)
  $(1)_SOURCES := $$($(1)_CSOURCES) $$($(1)_CXXSOURCES)
  $(1)_OBJECTS := $$($(1)_SOURCES:%.c=$(OUTPUT_DIR)/%.o)
endef

$(foreach entry,$(EXAMPLES),$(eval $(call make-example,$(entry))))

.SECONDEXPANSION:
$(EXAMPLE_FILES): $(LIBRARY_FILE) $$($$(notdir $$@)_OBJECTS)
	@echo "$(CXX): $@"
	@mkdir -p $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(addprefix test_,$(EXAMPLES)):
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=32 $(@:test_%=$(OUTPUT_DIR)/bin/%)

test: $(addprefix test_,$(EXAMPLES))
