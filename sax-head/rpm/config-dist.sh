#!/bin/sh

echo "Preparing host for build process..."

#========================================
# Functions...
#----------------------------------------
function removeMonoDependency {
	cat sax2.spec | \
		sed -e s@mono-devel-packages@@g \
	> sax2.spec.new
	mv sax2.spec.new sax2.spec
}


#========================================
# magic...
#----------------------------------------
case `uname -m` in
	ia64|ppc64)
		echo "There is no mono system available for this architecture"
		removeMonoDependency
	;;
	s390|s390x)
		echo "There is no business case for SaX :-)"
		exit 1
	;;
	*)
		echo "Everything is fine :-)"
	;;
esac

exit 0
