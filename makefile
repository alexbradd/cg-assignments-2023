SUBDIRS := A00 A01 A02 A03 A04 A05 A06 A07 A09 A10 A11

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C ./$@ all

.PHONY: all $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
