SUBDIRS := A00

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C ./$@ all

.PHONY: all $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
