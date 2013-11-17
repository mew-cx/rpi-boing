# $Id: GNUmakefile,v 1.20 2004/02/14 18:16:59 jimb Exp $
# Author:	Jim E. Brooks  http://www.jimbrooks.org
# Desc:		Top-level makefile.
# Notes:	Parallel make is supported.
#           Using "&&" between make -C ensures dependent subdirs are made first.
#           "+ $(makeall)" suppresses the "Add + to parent make rule" warning.
# =============================================================================

define MAKE_ALL
	$(MAKE) -C gfx $@ &&\
	$(MAKE) -C boing $@
endef

all .DEFAULT: CHECK_DEPS
	+ $(MAKE_ALL)
	@[ -x boing/out/boing ] && echo "-- Boing executable is at:"
	@[ -x boing/out/boing ] && echo "-- `pwd`/boing/out/boing"

# cc-config.sh just finds OpenGL include dirs.
.PHONY: CHECK_DEPS
CHECK_DEPS:
	@if ( ! gfx/cc-config.sh >/dev/null ); then echo "Requires OpenGL (or Mesa) development package."; exit 1; fi

define CLEAN
	@+ $(MAKE_ALL)                >/dev/null
	@rm -f core core.* gmon.out   >/dev/null
endef

clean:
	@echo "Cleaning."
	@$(CLEAN)

realclean distclean clean-all cleanall:
	@echo "Cleaning thoroughly."
	@$(CLEAN)
