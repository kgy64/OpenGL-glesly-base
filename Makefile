# Makefile for my GLES-based library: Glesly
#

.PHONY: all
all:
	@(cd src && $(MAKE) -s)

.PHONY: clean
clean:
	@(cd src && $(MAKE) -s clean)
	@rm -rf doc

.PHONY: doc
doc:
	@doxygen Doxyfile

