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
	ia64)
		echo "There is no mono available for this architecture"
		removeMonoDependency
	;;
	*)
		echo "Everything is fine :-)"
	;;
esac

exit 0
