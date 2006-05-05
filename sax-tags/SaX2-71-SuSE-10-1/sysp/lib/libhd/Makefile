TOPDIR      = $(CURDIR)
SUBDIRS		= hd ids
TARGETS		= touch $(LIBHD)
CLEANFILES	= $(LIBHD) $(LIBHD_D) *.a

.PHONY: touch

include $(TOPDIR)/Makefile.common

ifneq "$(findstring $(ARCH), i386 x86_64 ia64)" ""
ifneq ($(LIBHD_BASE), libhd_tiny)
SUBDIRS	+= x86emu int10
endif
endif

$(LIBHD): $(OBJS)
	ar r $@ $?
	@rm -f $(LIBHD_D)

subdirs: touch

touch:
	@touch -r $(LIBHD) $(LIBHD_D) 2>/dev/null || true
