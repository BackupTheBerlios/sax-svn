# This file was automatically generated by SWIG
package XFree;
require Exporter;
require DynaLoader;
@ISA = qw(Exporter DynaLoader);
package XFreec;
bootstrap XFree;
package XFree;
@EXPORT = qw( );

# ---------- BASE METHODS -------------

package XFree;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package XFree;

*ReadConfigFile = *XFreec::ReadConfigFile;
*GetModulePath = *XFreec::GetModulePath;
*GetFontPath = *XFreec::GetFontPath;
*GetRgbPath = *XFreec::GetRgbPath;
*GetLogFile = *XFreec::GetLogFile;
*GetModuleSpecs = *XFreec::GetModuleSpecs;
*GetModuleSubSpecs = *XFreec::GetModuleSubSpecs;
*GetInputSpecs = *XFreec::GetInputSpecs;
*GetFlags = *XFreec::GetFlags;
*GetExtensions = *XFreec::GetExtensions;
*GetLayoutSpecs = *XFreec::GetLayoutSpecs;
*GetDeviceSpecs = *XFreec::GetDeviceSpecs;
*GetMonitorSpecs = *XFreec::GetMonitorSpecs;
*GetModesSpecs = *XFreec::GetModesSpecs;
*GetDisplaySpecs = *XFreec::GetDisplaySpecs;

# ------- VARIABLE STUBS --------

package XFree;

1;
