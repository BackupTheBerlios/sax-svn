SaX2 profile mode:
------------------
To be able to react on special situations SaX2 provides a batch
mode which is able to set/reset values via interactive invocation
or via a script called profile. Such a profile contains information
in a special format used for SaX2 only. Some profiles are called
automatically and others are so called unbound profiles which needs
manual invocation.

This file gives you an overview about the unbound profiles and their
benefits to society :-)

#===========================
# NVidia_Twinview
#===========================
If you have a NVidia (dualhead) card and have installed the binary only
driver from NVidia using YaST online update you can use the TwinView mode
instead of the traditional multihead configuration. One of the advantages
is the capability of using 3D on all screens. To enable this feature just
call SaX2 with the following profile:

	sax2 -b /usr/X11R6/lib/sax/profile/NVidia_Twinview

#===========================
# laptop
#===========================
If you have a NoteBook with an integrated Intel, NVIDIA, or ATI chipset
you can use the laptop profile to statically activate the external
output used to connect e.g. to a beamer. It will always be driven with
60Hz and the same resolution as the internal display. Use the krandr KDE
applet or xrandr to select the appropriate resolution. To enable this
feature call SaX2 with the following profile:

	sax2 -b /usr/X11R6/lib/sax/profile/laptop

If you want to use 3D graphics with newer radeon chips you have to use
the binary driver from ati (note that you most likely will loose
suspend/resume, and switching video modes with randr does not work):

	sax2 -m 0=fglrx -b /usr/X11R6/lib/sax/profile/laptop

--
Remember to have fun :-)
Marcus
