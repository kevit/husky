#!/bin/bash
cd huskybse;
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i huskybase_2.0-4_all.deb
cd huskylib/
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i libhusky-dev_1.9-20090324-1_amd64.deb
dpkg -i libhusky1_1.9-20090324-1_amd64.deb
cd smapi
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i libsmapi2cur_2.5-20070915-1_amd64.deb
dpkg -i libsmapi2cur-dev_2.5-20070915-1_amd64.deb
cd fidoconf
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i libfidoconf1_1.9-20070915-1_amd64.deb
dpkg -i libfidoconf1-dev_1.9-20070915-1_amd64.deb
dpkg -i fidoconf-runtime_1.9-20070915-1_amd64.deb
cd areafix
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i libareafix1_1.9-20090324-1_amd64.deb
dpkg -i libareafix-dev_1.9-20090324-1_amd64.deb
cd hpt
dpkg-buildpackage -us -uc -b
cd ..
dpkg -i hpt_1.9-20090324-1_amd64.deb
