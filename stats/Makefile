
%.dot	:	%.xml
	$(eval $TMP := $(shell mktemp XXXXXX))
	xml sel -t -c "/Individual/Genotype/*" $< > ${$TMP}
	xml tr xml2dot.xsl ${$TMP} > $@
	rm ${$TMP}

%.ps	:	%.dot
	dot -Tps $< -o $@

%.eps	:	%.dot
	dot -Teps $< -o $@

%.pdf	:	%.ps
	ps2pdf $< $@

all:	draw

clean:
	rm -f *.dat *.dot *.ps *.pdf

