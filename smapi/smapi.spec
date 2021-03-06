%define reldate 20160316
%define reltype C
# may be one of: C (current), R (release), S (stable)

Name: smapi
Version: 2.5.%{reldate}%{reltype}
Release: 1
Group: Libraries/FTN
Summary: Squish Messagebase API
URL: http://husky.sf.net
License: GPL
BuildRequires: huskylib >= 1.9
Source: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
Squish Messagebase API library for the Husky Project software.

%prep
%setup -q -n %{name}

%build
make

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install
cp cvsdate.h %{buildroot}%{_includedir}/smapi
chmod -R a+rX,u+w,go-w %{buildroot}

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%{_includedir}/%{name}/*
%{_libdir}/*
