all:man html

DTD_MAN=/usr/share/xml/docbook/stylesheet/nwalsh/current/manpages/docbook.xsl
DTD_HTML=/usr/share/xml/docbook/stylesheet/nwalsh/current/html/docbook.xsl

html:
	for i in `ls -1 *.xml`; do \
		base=`echo $$i | sed -e s@.xml@@` ;\
		xsltproc --output $$base.html ${DTD_HTML} $$i ;\
	done

man:
	for i in `ls -1 *.xml`; do \
		xsltproc ${DTD_MAN} $$i ;\
	done

clean:
	rm -f *.3
	rm -f *.html
