all:man

DTD_MAN=/usr/share/xml/docbook/stylesheet/nwalsh/1.65.1/manpages/docbook.xsl
DTD_HTML=/usr/share/xml/docbook/stylesheet/nwalsh/1.65.1/html/docbook.xsl

html:
	for i in `ls -1 *.xml`; do \
		xsltproc --output $$i.html ${DTD_HTML} $$i ;\
	done

man:
	for i in `ls -1 *.xml`; do \
		xsltproc ${DTD_MAN} $$i ;\
	done

clean:
	rm -f *.3
	rm -f *.html
