encoder ?= astcrt

pngs = $(wildcard *.png)
diffs = $(patsubst %.png,%.diff.tga,$(pngs))
psnrs = $(patsubst %.png,%.psnr.csv,$(pngs))

all: $(diffs)

psnr.csv: $(psnrs)
	echo "file,PSNR" > $@
	cat $^ | sort -k1 -t, >> $@

clean:
	$(RM) *.tga *.astc *.csv

.SECONDARY:

%.resized.tga: %.png
	make-divisible.sh --add-alpha $< $@

%.astc: %.resized.tga
	$(encoder) $< $@

%.decoded.tga: %.astc
	astcenc -d $< $@ -silentmode

%.diff.tga: %.decoded.tga %.resized.tga
	gm compare -file $@ $^ || if [[ $$? -eq 1 ]]; then exit 0; fi

%.psnr.csv: %.resized.tga %.decoded.tga
	@echo -n $*.png > $@
	@echo -n "," >> $@
	compare -metric PSNR $(word 1,$^) $(word 2,$^) /dev/null 2>> $@ || if [[ $$? -eq 1 ]]; then exit 0; fi
	@echo >> $@

.PHONY: all clean
