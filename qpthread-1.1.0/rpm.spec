%define name qpthread
%define major_ver 1
%define minor_ver 1
%define release_ver 0
%define version %{major_ver}.%{minor_ver}.%{release_ver}
%define libname libqpthr
%define lib_so_name %{libname}.so.%{version}
%define rel 1
%define incname qpthr

Summary:   C++ library for threads 
Name:      %{name}
Version:   %{version}
Release:   %{rel}
Copyright: LGPL
Packager:  Pavel Krauz <kra@gncz.cz>
Vendor:    Pavel Krauz <kra@gncz.cz>
Group:     System Environment/Libraries
Source:    ftp://ftp.gncz.cz/pub/qpthread/qpthread-%{version}.tgz
#Source:    qpthread-%{version}.tgz
URL:       http://www.gncz.cz/kra/index.html
BuildRoot: /tmp/rpmbuild_%{name}
Docdir:    /usr/doc

%description 
This is the shared library for programs using the QpThread library.

The QpThread Library provides object oriented framework in C++ for threads 
and Unix signals on top of system level threads (currently POSIX Threads). 
Moreover it supports deferred cancellation through C++ exception, timer, 
producent/consumer containers, thread pool and much more. 

If you would like to develop applications using the QpThread library, you'll
also need to install qpthread-devel.


%package devel
Summary:   QpThread library development.
Group:     Development/Libraries
Requires:  %{name} = %{PACKAGE_VERSION}

%description devel
Libraries and include files you can use for developing applications using
the QpThread library.

The QpThread Library provides object oriented framework in C++ for threads 
and Unix signals on top of system level threads (currently POSIX Threads). 
Moreover it supports deferred cancellation through C++ exception, timer, 
producent/consumer containers, thread pool and much more. 

You should install the qpthread-devel package only if you would like to develop
applications using the QpThread library or you would like to compile programs
that use the QpThread library.

%prep
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}
%setup

%build
./configure
make all

%install
# dynamic linked library
#strip ./%{lib_so_name}
install -d $RPM_BUILD_ROOT/usr/lib/
install ./%{lib_so_name} $RPM_BUILD_ROOT/usr/lib/
ln -s %{lib_so_name} $RPM_BUILD_ROOT/usr/lib/%{libname}.so.%{major_ver}
ln -s %{lib_so_name} $RPM_BUILD_ROOT/usr/lib/%{libname}.so
# static linked library
#strip ./%{libname}.a
install ./%{libname}.a $RPM_BUILD_ROOT/usr/lib/ 
# include files
install -d $RPM_BUILD_ROOT/usr/include/%{incname}/
install --mode=0444 ./qpthr/*.h $RPM_BUILD_ROOT/usr/include/%{incname}/

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
/usr/lib/lib*.so*
%doc CHANGES COPYING INSTALL README README.html TODO  Make.def Make.def.base ex

%files devel
%defattr(-,root,root)
%dir /usr/include/%{incname}
/usr/include/%{incname}/*.h
/usr/lib/*.a
#/usr/lib/%{lib_so_name}
